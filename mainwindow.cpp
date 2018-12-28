#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "task.h"

#include <QCommonStyle>
#include <QFileDialog>
#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrentRun>
#include <thread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    t(new Task),
    timer(new QElapsedTimer)
{
    ui->setupUi(this);

    QCommonStyle style;
    ui->select_directory->setIcon(style.standardIcon(QCommonStyle::SP_DialogOpenButton));
    ui->lineEdit->setReadOnly(true);

    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->horizontalHeader()->hide();
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setShowGrid(false);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(&watcher1, SIGNAL(finished()), this, SLOT(index_finished()));
    connect(&watcher2, SIGNAL(finished()), this, SLOT(search_finished()));
    connect(ui->tableWidget, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(view_file(int, int)));
}

MainWindow::~MainWindow()
{
    delete timer;
    delete t;
    delete ui;
}

void MainWindow::on_select_directory_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select Directory for Scanning",
                                                    QString(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->lineEdit->setText(dir);
}

void MainWindow::on_index_directory_clicked()
{
    qDebug() << QString(__func__) << " from work thread: " << QThread::currentThreadId();
    ui->index_directory->setEnabled(false);
    ui->search->setEnabled(false);
    timer->restart();
    future1 = QtConcurrent::run(t, &Task::index_files, ui->lineEdit->text());
    watcher1.setFuture(future1);
}

void MainWindow::index_finished() {
    qDebug() << QString(__func__) << " from work thread: " << QThread::currentThreadId() <<
                " in" << QString::number(timer->elapsed());
    ui->index_directory->setEnabled(true);
    ui->search->setEnabled(true);
    /*for (auto &f: t->files) {
        qDebug() << f.file << ' ' << QString::number(f.trigrams.size());
    }*/
}

void MainWindow::on_search_clicked()
{
    qDebug() << QString(__func__) << " from work thread: " << QThread::currentThreadId();
    ui->index_directory->setEnabled(false);
    ui->search->setEnabled(false);
    timer->restart();
    future2 = QtConcurrent::run(t, &Task::search, ui->word->text());
    watcher2.setFuture(future2);
}

void MainWindow::search_finished() {
    qDebug() << QString(__func__) << " from work thread: " << QThread::currentThreadId() <<
                " in" << QString::number(timer->elapsed());
    ui->index_directory->setEnabled(true);
    ui->search->setEnabled(true);
    ui->viewer->m = watcher2.result();
    ui->tableWidget->setRowCount(ui->viewer->m.size());
    int i = 0;
    for (auto &f: ui->viewer->m) {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(f.second.size())));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(f.first));
        i++;
    }
}

void MainWindow::on_pushButton_clicked()
{
    qDebug() << QString(__func__) << " from work thread: " << QThread::currentThreadId();
    cancel();
}

void MainWindow::cancel() {
    qDebug() << QString(__func__) << " from work thread: " << QThread::currentThreadId();
    if (!ui->index_directory->isEnabled())
        t->cancel();
}

void MainWindow::view_file(int r, int) {
    qDebug() << QString(__func__) << " " << QString::number(r);
    ui->viewer->display(r);
}
