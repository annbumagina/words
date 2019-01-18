#ifndef TASK_H
#define TASK_H

class Task;

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
    std::pair< std::string, std::vector< std::pair<QString, long long> > > search(QString);
    void index_files(QString);
    void cancel();

signals:
    void setRange(int, int);
    void setValue(int);

//private:
public:
    QList<Index> files;

};

#endif // TASK_H
