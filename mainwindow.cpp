#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include "wdatabase.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug() << Q_FUNC_INFO;
    qDebug() << "___ ss ___" << __func__;

    wd_ = new ShowWidget(this);
    setCentralWidget(wd_);

    auto &inter = WDataBase::instance();

    QDate end_day = QDate::currentDate();
    QDate start_day = end_day.addDays(-5);
    int i = 1;
    for (QDate date = start_day; date <= end_day; date = date.addDays(1)) {
             DailyWeldData obj;
             obj.date = date.toString("yyyy_MM_dd");
             obj.flat_weld_length = i*2;
             obj.verical_weld_length = i*3;
             obj.welding_time = i*20;
             obj.teaching_time = i*10;
             obj.weld_count = i*5;
             inter.addDailyWeldData(obj);
             i++;
      }

    inter.addTodayProjectState();
}

MainWindow::~MainWindow()
{
    delete ui;
}

