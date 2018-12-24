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
    std::vector<QString> get_files(QString);
    void run(QString);
    void index_files();
    void index_finished();
    void serch(QString);

private:
    QFutureWatcher< std::vector<QString> >  watcher;
    QFutureWatcher<void> index_watcher;
    QFuture< std::vector<QString> > future;
    QFuture<void> index_future;
    QList<Index> files;
    QElapsedTimer *timer;

};

#endif // TASK_H
