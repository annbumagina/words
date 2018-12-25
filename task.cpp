#include "task.h"

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

Task::Task() : canceled(false) {}

void Task::cancel() {
    canceled = true;
    for (auto& f: files) {
        f.canceled = true;
    }
}

void Task::index_files(QString dir) {
    qDebug() << QString(__func__) << " from work thread: " << QThread::currentThreadId();
    QDirIterator it(dir, QDir::Files | QDir::NoDotAndDotDot | QDir::Readable, QDirIterator::Subdirectories);
    files.clear();
    while (it.hasNext()) {
        if (canceled) return;
        QString file_path = it.next();
        files.append(file_path);
    }
    for (auto& f: files) {
        if (canceled) return;
        f.index_file();
    }
}

std::vector< std::pair<QString, long long> > Task::search(QString wordQ) {
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

    std::vector< std::pair<QString, long long> > m;
    long long cnt;
    std::regex sp { R"([-[\]{}()*+?.,\^$|#\s\\])" };
    std::string sanitized = regex_replace(word, sp, R"(\$&)");
    for (auto& f: files) {
        if (canceled) return m;
        cnt = f.search(sanitized, tr);
        if (cnt > 0) {
            m.push_back({f.file, cnt});
        }
    }
    return m;
}
