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
    QDirIterator it(dir, QDir::Files | QDir::NoDotAndDotDot | QDir::Readable, QDirIterator::Subdirectories);
    files.clear();
    char bf[12];
    while (it.hasNext()) {
        if (canceled) { canceled = false; return; }
        QString file_path = it.next();
        std::ifstream in(file_path.toStdString().c_str());
        in.read(bf, 12);
        if (MagicNumber::check_type(bf, in.gcount())) {
            files.append({this, file_path});
            //qDebug() << file_path << " good";
        }
    }
    for (auto& f: files) {
        if (canceled) { canceled = false; return; }
        f.index_file();
    }
    canceled = false;
}

std::vector< std::pair<QString, std::vector<long long> > > Task::search(QString wordQ) {
    qDebug() << QString(__func__) << " from work thread: " << QThread::currentThreadId();
    unsigned mask = 0x00FFFFFF;
    std::vector<unsigned> tr;
    std::string word = wordQ.toStdString();
    if (word.size() > 2) {
        tr.reserve(word.size() - 2);
        unsigned x = (unsigned(word[0]) << 8) | unsigned(word[1]);
        for (size_t i = 2; i < word.size(); i++) {
            x = (((x << 8) & mask) | word[i]);
            tr.push_back(x);
            //qDebug() << QString::number(x);
        }
    }
    std::sort(tr.begin(), tr.end());
    tr.resize(std::unique(tr.begin(), tr.end()) - tr.begin());

    std::vector< std::pair<QString, std::vector<long long> > > m;
    std::regex sp { R"([-[\]{}()*+?.,\^$|#\s\\])" };
    std::string sanitized = regex_replace(word, sp, R"(\$&)");
    std::regex r(sanitized);
    for (auto& f: files) {
        if (canceled) { canceled = false; return m; }
        std::vector<long long> pos = f.search(word.size(), r, tr);
        if (pos.size() > 0) {
            m.push_back({f.file, pos});
        }
    }
    canceled = false;
    return m;
}
