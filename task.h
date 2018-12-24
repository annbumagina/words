#ifndef TASK_H
#define TASK_H

#include "index.h"

#include <QObject>
#include <QFuture>
#include <QFutureWatcher>
#include <QList>
#include <QElapsedTimer>

class Task : public QObject
{
    Q_OBJECT
public:
    Task();

signals:
    void send(std::vector<QString>);

public slots:
    void index_files(QString);
    void run(QString);
    void index_finished();
    void serch(QString);

private:
    QFutureWatcher<void> watcher;
    QFuture<void> future;
    QList<Index> files;
    QElapsedTimer *timer;

};

#endif // TASK_H
