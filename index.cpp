#include "index.h"

#include <set>
#include <QThread>
#include <QDebug>
#include <fstream>
#include <QtConcurrent/QtConcurrentMap>
#include <QtConcurrent/QtConcurrentRun>
#include <QElapsedTimer>

Index::Index(const QString &s) : file(s)
{
    //QThreadPool::globalInstance()->setMaxThreadCount(20);
}

void Index::index_file() {
    qDebug() << QString(__func__) << " from work thread: " << QThread::currentThreadId();
    std::ifstream in(file.toStdString().c_str());
    const int N = 1024 * 32;
    char bf[TH][N];
    char a, b;
    int k = 0;
    /*for (int i = 0; i < TH; i++) {
        in.read(bf[i], N);
        tris[i].bf = bf[i];
        tris[i].len = in.gcount();
        //tris[i].id = i;
        //tris[i].file = file;
        future[i] = QtConcurrent::run(tris[i], &Trigram::text_to_tris);
    }*/
    int t1 = 0, t2 = 0;
    QElapsedTimer ti;
    while (!in.eof()) {
        for (int i = 0; i < TH; i++) {
            /*ti.restart();
            future[i].waitForFinished();
            t2 += ti.elapsed();*/
            ti.restart();
            in.read(bf[i], N);
            t1 += ti.elapsed();
            tris[i].bf = bf[i];
            tris[i].len = in.gcount();
            //tris[i].id = i;
            tris[i].file = file;
            future[i] = QtConcurrent::run(tris[i], &Trigram::text_to_tris);
        }
        //future = QtConcurrent::map(std::begin(tris), std::end(tris), &Trigram::text_to_tris);
        //future.waitForFinished();
        ti.restart();
        for (int i = 0; i < TH; i++) {
            future[i].waitForFinished();
        }
        t2 += ti.elapsed();
        /*in.read(bf, N);
        int len = in.gcount();
        if (k != 0) {
            if (len > 0) {
                unsigned x = (unsigned(a) << 16) | (unsigned(b) << 8) | unsigned(bf[0]);
                trigrams.insert(x);
            }
            if (len > 1) {
                unsigned x = (unsigned(b) << 16) | (unsigned(bf[0]) << 8) | unsigned(bf[1]);
                trigrams.insert(x);
            }
        }
        for (int i = 0; i < len - 2; i++) {
            unsigned x = (unsigned(bf[i]) << 16) | (unsigned(bf[i + 1]) << 8) | unsigned(bf[i + 2]);
            trigrams.insert(x);
        }
        if (len > 2) {
            a = bf[len - 2];
            b = bf[len - 1];
        }
        k++;*/
    }
    qDebug() << QString::number(t1) << ' ' << QString::number(t2);
    qDebug() << QString::number(tris[0].tris.size());
}
