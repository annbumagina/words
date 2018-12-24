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
    t(new Task)
{
    ui->setupUi(this);

    QCommonStyle style;
    ui->select_directory->setIcon(style.standardIcon(QCommonStyle::SP_DialogOpenButton));
    ui->lineEdit->setReadOnly(true);
    connect(t, SIGNAL(send(std::vector<QString>)), this, SLOT(get_files(std::vector<QString>)));
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
    qDebug() << QString(__func__);
    t->run(ui->lineEdit->text());
}

void MainWindow::get_files(std::vector<QString> vs) {
    qDebug() << QString(__func__);
    /*for (QString &s: vs) {
        ui->listWidget->addItem(s);
    }*/
}

void MainWindow::on_search_clicked()
{
    qDebug() << QString(__func__);
    t->serch((ui->search->text()));
}
