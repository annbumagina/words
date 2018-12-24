#include "index.h"

#include <set>
#include <QThread>
#include <QDebug>
#include <fstream>
#include <QtConcurrent/QtConcurrentMap>
#include <QtConcurrent/QtConcurrentRun>
#include <QElapsedTimer>
#include <future>

Index::Index(const QString &s) : file(s)
{}

void Index::index_file() {
    qDebug() << QString(__func__) << " from work thread: " << QThread::currentThreadId();
    std::ifstream in(file.toStdString().c_str());
    const int N = 1024 * 32;
    char bf[N];
    unsigned mask = 0x00FFFFFF;

    in.read(bf, 2);
    unsigned x = (unsigned(bf[0]) << 8) | unsigned(bf[1]);
    while (!in.eof()) {
        in.read(bf, N);
        int len = in.gcount();
        for (int i = 0; i < len; i++) {
            x = (((x << 8) & mask) | bf[i]);
            trigrams.insert(x);
        }
    }
}
