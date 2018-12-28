#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "task.h"
#include "viewer.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_select_directory_clicked();
    void on_index_directory_clicked();
    void on_search_clicked();
    void index_finished();
    void search_finished();
    void on_pushButton_clicked();
    void cancel();
    void view_file(int, int);

private:
    Ui::MainWindow *ui;
    Task *t;
    QElapsedTimer *timer;
    QFutureWatcher<void> watcher1;
    QFutureWatcher< std::vector< std::pair<QString, std::vector<long long> > > > watcher2;
    QFuture<void> future1;
    QFuture< std::vector< std::pair<QString, std::vector<long long> > > > future2;
};

#endif // MAINWINDOW_H
