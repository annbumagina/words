#include "trigram.h"
#include <QDebug>
#include <QThread>
#include <QString>
#include <unordered_set>

Trigram::Trigram()
{}

void Trigram::text_to_tris() {
    //qDebug() << QString(__func__) << " " << file << " " << QString::number(id) <<" from work thread: " << QThread::currentThreadId();
    unsigned mask = 0x00FFFFFF;
    unsigned x = (unsigned(bf[0]) << 16) | (unsigned(bf[1]) << 8) | unsigned(bf[2]);
    tris.insert(x);
    //qDebug() << QString(__func__) << " " << file << " " << QString::number(mask);
    for (int i = 1; i < len - 2; i++) {
        //unsigned x = (unsigned(bf[i]) << 16) | (unsigned(bf[i + 1]) << 8) | unsigned(bf[i + 2]);
        x = (((x << 8) & mask) | bf[i + 2]);
        tris.insert(x);
        //tris.insert(x);

    }
    //qDebug() << QString(__func__) << " " << file << " " << QString::number(tris.size());
}
