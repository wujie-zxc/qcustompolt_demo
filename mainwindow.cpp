#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug() << Q_FUNC_INFO;
    qDebug() << "___ ss ___" << __func__;

    wd_ = new ShowWidget(this);
    setCentralWidget(wd_);
}

MainWindow::~MainWindow()
{
    delete ui;
}

