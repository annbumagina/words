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
    ui->progressBar->hide();

    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->horizontalHeader()->hide();
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setShowGrid(false);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->widget_5->hide();

    connect(&watcher1, SIGNAL(finished()), this, SLOT(index_finished()));
    connect(&watcher2, SIGNAL(finished()), this, SLOT(search_finished()));
    connect(ui->tableWidget, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(view_file(int, int)));
    connect(t, SIGNAL(setRange(int, int)), ui->progressBar, SLOT(setRange(int, int)));
    connect(t, SIGNAL(setValue(int)), ui->progressBar, SLOT(setValue(int)));
}

MainWindow::~MainWindow()
{
    cancel();
    watcher1.waitForFinished();
    watcher2.waitForFinished();
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
    ui->progressBar->show();
    ui->progressBar->setValue(0);
    ui->label->hide();
    timer->restart();
    future1 = QtConcurrent::run(t, &Task::index_files, ui->lineEdit->text());
    watcher1.setFuture(future1);
}

void MainWindow::index_finished() {
    /*qDebug() << QString(__func__) << " from work thread: " << QThread::currentThreadId() <<
                " in" << QString::number(timer->elapsed());*/
    ui->index_directory->setEnabled(true);
    ui->search->setEnabled(true);

    ui->progressBar->hide();
    ui->label->show();
    ui->label->setText("Indexed in " + QString::number(timer->elapsed()));
}

void MainWindow::on_search_clicked()
{
    qDebug() << QString(__func__) << " from work thread: " << QThread::currentThreadId();
    ui->index_directory->setEnabled(false);
    ui->search->setEnabled(false);
    ui->progressBar->show();
    ui->progressBar->setValue(0);
    ui->label->hide();
    timer->restart();
    future2 = QtConcurrent::run(t, &Task::search, ui->word->text());
    watcher2.setFuture(future2);
}

void MainWindow::search_finished() {
    /*qDebug() << QString(__func__) << " from work thread: " << QThread::currentThreadId() <<
                " in" << QString::number(timer->elapsed());*/
    ui->index_directory->setEnabled(true);
    ui->search->setEnabled(true);
    word = watcher2.result().first;
    pos = watcher2.result().second;
    ui->tableWidget->setRowCount(pos.size());
    int i = 0;
    for (auto &f: pos) {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(f.second)));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(f.first));
        i++;
    }

    ui->progressBar->hide();
    ui->label->show();
    ui->label->setText("Found in " + QString::number(timer->elapsed()));
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
    ui->viewer->word = word;
    ui->viewer->file = pos[r].first;
    timer->restart();
    ui->viewer->display();
    ui->widget_5->show();
    ui->position->setText(QString::number(ui->viewer->pos + 1) + " / "
                          + QString::number(ui->viewer->m.size()));
    qDebug() << QString::number(timer->elapsed());
}

void MainWindow::on_prev_clicked()
{
    ui->viewer->prev();
    ui->position->setText(QString::number(ui->viewer->pos + 1) + " / "
                                             + QString::number(ui->viewer->m.size()));
}

void MainWindow::on_next_clicked()
{
    ui->viewer->next();
    ui->position->setText(QString::number(ui->viewer->pos + 1) + " / "
                          + QString::number(ui->viewer->m.size()));
}
