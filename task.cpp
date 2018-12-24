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
    connect(&watcher, SIGNAL(finished()), this, SLOT(index_files()));
    connect(&index_watcher, SIGNAL(finished()), this, SLOT(index_finished()));
}

void Task::run(QString dir) {
    qDebug() << QString(__func__) << " from work thread: " << QThread::currentThreadId();
    future = QtConcurrent::run(this, &Task::get_files, dir);
    watcher.setFuture(future);
}

std::vector<QString> Task::get_files(QString dir) {
    qDebug() << QString(__func__) << " from work thread: " << QThread::currentThreadId();
    QDirIterator it(dir, QDir::Files | QDir::NoDotAndDotDot | QDir::Readable, QDirIterator::Subdirectories);
    std::vector<QString> files;
    while (it.hasNext()) {
        QString file_path = it.next();
        files.push_back(file_path);
    }



    return files;
}

void Task::index_files() {
    qDebug() << QString(__func__) << " from work thread: " << QThread::currentThreadId();
    emit send(watcher.result());
    files.clear();
    for (auto& s: watcher.result()) {
        //Index *i = new Index(s);
        //files.push_back(i);
        files.append(s);
    }
    timer = new QElapsedTimer;
    timer->start();
    index_future = QtConcurrent::map(files, &Index::index_file);
    index_watcher.setFuture(index_future);
    //index_future = QtConcurrent::run(files[0], &Index::index_file);
}

void Task::index_finished() {
    qDebug() << QString(__func__) << " from work thread: " << QThread::currentThreadId() <<
                " in" << QString::number(timer->elapsed());
    for (auto &f: files) {
        qDebug() << f.file;
        /*for (auto it: f.trigrams) {
            //qDebug() << QString::fromStdString(std::bitset<32>(it).to_string());
            qDebug() << it;
        }*/
        qDebug() << QString::number(f.trigrams.size());
    }
}

void Task::serch(QString s) {

}
