#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "task.h"
#include "viewer.h"

#include <QMainWindow>
#include <QProgressBar>
#include <QFileSystemWatcher>

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
    void on_prev_clicked();
    void on_next_clicked();

private:
    Ui::MainWindow *ui;
    Task *t;
    QElapsedTimer *timer;
    std::vector< std::pair<QString, long long> > pos;
    std::string word;
    QFutureWatcher<void> watcher1;
    QFutureWatcher< std::pair< std::string, std::vector< std::pair<QString, long long> > > > watcher2;
    QFuture<void> future1;
    QFuture< std::pair< std::string, std::vector< std::pair<QString, long long> > > > future2;
    QFileSystemWatcher dir_watcher;
};

#endif // MAINWINDOW_H
