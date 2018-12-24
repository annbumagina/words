#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "task.h"

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
    void get_files(std::vector<QString>);

    void on_search_clicked();

private:
    Ui::MainWindow *ui;
    Task *t;
};

#endif // MAINWINDOW_H
