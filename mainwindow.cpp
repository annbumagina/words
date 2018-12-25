#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "task.h"

#include <QCommonStyle>
#include <QFileDialog>
#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrentRun>

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

    connect(&watcher1, SIGNAL(finished()), this, SLOT(index_finished()));
    connect(&watcher2, SIGNAL(finished()), this, SLOT(search_finished()));
}

MainWindow::~MainWindow()
{
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
    timer->restart();
    future1 = QtConcurrent::run(t, &Task::index_files, ui->lineEdit->text());
    watcher1.setFuture(future1);
}

void MainWindow::index_finished() {
    qDebug() << QString(__func__) << " from work thread: " << QThread::currentThreadId() <<
                " in" << QString::number(timer->elapsed());
    for (auto &f: t->files) {
        qDebug() << f.file << ' ' << QString::number(f.trigrams.size());
    }
}

void MainWindow::on_search_clicked()
{
    qDebug() << QString(__func__) << " from work thread: " << QThread::currentThreadId();
    timer->restart();
    future2 = QtConcurrent::run(t, &Task::search, ui->word->text());
    watcher2.setFuture(future2);
}

void MainWindow::search_finished() {
    qDebug() << QString(__func__) << " from work thread: " << QThread::currentThreadId() <<
                " in" << QString::number(timer->elapsed());
    for (auto &f: watcher2.result()) {
        ui->listWidget->addItem(f.first + " --- " + QString::number(f.second));
    }
}

void MainWindow::on_pushButton_clicked()
{
    cancel();
}

void MainWindow::cancel() {
    t->cancel();
}
