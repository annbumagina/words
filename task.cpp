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

Task::Task() {
    timer = new QElapsedTimer;
    connect(&watcher, SIGNAL(finished()), this, SLOT(index_finished()));
}

void Task::run(QString dir) {
    qDebug() << QString(__func__) << " from work thread: " << QThread::currentThreadId();
    timer->start();
    future = QtConcurrent::run(this, &Task::index_files, dir);
    watcher.setFuture(future);
}

void Task::index_files(QString dir) {
    qDebug() << QString(__func__) << " from work thread: " << QThread::currentThreadId();
    QDirIterator it(dir, QDir::Files | QDir::NoDotAndDotDot | QDir::Readable, QDirIterator::Subdirectories);
    //std::vector<QString> files;
    files.clear();
    while (it.hasNext()) {
        QString file_path = it.next();
        files.append(file_path);
    }
    for (auto& f: files) {
        f.index_file();
    }
}

void Task::index_finished() {
    qDebug() << QString(__func__) << " from work thread: " << QThread::currentThreadId() <<
                " in" << QString::number(timer->elapsed());
    for (auto &f: files) {
        qDebug() << f.file;
        qDebug() << QString::number(f.trigrams.size());
    }
}

void Task::serch(QString s) {

}
