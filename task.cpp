#include "task.h"
#include "magicnumber.h"

#include <vector>
#include <QString>
#include <QDebug>
#include <QDirIterator>
#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrentRun>
#include <QtConcurrent/QtConcurrentMap>
#include <QThread>
#include <bitset>
#include <string>
#include <regex>
#include <fstream>

Task::Task() : canceled(false) {}

void Task::cancel() {
    qDebug() << QString(__func__) << " from work thread: " << QThread::currentThreadId();
    canceled = true;
}

void Task::index_files(QString dir) {
    qDebug() << QString(__func__) << " from work thread: " << QThread::currentThreadId();
    QDirIterator it2(dir, QDir::Files | QDir::NoDotAndDotDot | QDir::Readable, QDirIterator::Subdirectories);
    int files_cnt = 0;
    while (it2.hasNext()) {
        if (canceled) { canceled = false; return; }
        it2.next();
        files_cnt++;
    }
    emit setRange(0, std::max(1, files_cnt));

    QDirIterator it(dir, QDir::Files | QDir::NoDotAndDotDot | QDir::Readable, QDirIterator::Subdirectories);
    int cnt = 0;
    files.clear();
    char bf[12];
    while (it.hasNext()) {
        if (canceled) { canceled = false; return; }
        QString file_path = it.next();
        std::ifstream in(file_path.toStdString().c_str());
        in.read(bf, 12);
        in.close();
        if (MagicNumber::check_type(bf, in.gcount())) {
            if (canceled) { canceled = false; return; }
            files.append({this, file_path});
            if (!files.back().index_file()) {
                files.pop_back();
            }
        }
        cnt++;
        if (cnt % 50 == 0)
            emit setValue(cnt);
    }
    emit setValue(cnt);
    canceled = false;
}

std::pair< std::string, std::vector< std::pair<QString, long long> > > Task::search(QString wordQ) {
    qDebug() << QString(__func__) << " from work thread: " << QThread::currentThreadId();
    unsigned mask = 0x00FFFFFF;
    std::vector<unsigned> tr;
    std::string word = wordQ.toStdString();
    if (word.size() > 2) {
        tr.reserve(word.size() - 2);
        unsigned x = ((unsigned(uchar(word[0])) << 8) | unsigned(uchar(word[1])));
        for (size_t i = 2; i < word.size(); i++) {
            x = (((x << 8) & mask) | (unsigned(uchar(word[i]))));
            tr.push_back(x);
            //qDebug() << QString::number(x);
        }
    }
    std::sort(tr.begin(), tr.end());
    tr.resize(std::unique(tr.begin(), tr.end()) - tr.begin());

    std::vector< std::pair<QString, long long> > m;
    std::regex sp { R"([-[\]{}()*+?.,\^$|#\s\\])" };
    std::string sanitized = regex_replace(word, sp, R"(\$&)");
    std::regex r(sanitized);
    emit setRange(0, std::max(1, files.size()));
    int cnt = 0;
    for (auto& f: files) {
        if (canceled) { canceled = false; return {word, m}; }
        long long pos = f.search(word.size(), r, tr);
        if (pos > 0) {
            m.push_back({f.file, pos});
        }
        cnt++;
        if (cnt % 50 == 0)
            emit setValue(cnt);
    }
    canceled = false;
    return {word, m};
}
