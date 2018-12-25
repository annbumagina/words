#ifndef TASK_H
#define TASK_H

#include "index.h"

#include <QObject>
#include <QFuture>
#include <QFutureWatcher>
#include <QList>
#include <QElapsedTimer>
#include <atomic>

class Task : public QObject
{
    Q_OBJECT
public:
    Task();
    std::atomic<bool> canceled;

public slots:
    std::vector< std::pair<QString, long long> > search(QString);
    void index_files(QString);
    void cancel();

//private:
public:
    QList<Index> files;

};

#endif // TASK_H
