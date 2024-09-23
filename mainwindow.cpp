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
    QDate start_day = QDate::fromString("2024_09_02", "yyyy_MM_dd");

    for (int i = 0; i <40; i++){
        if (i == 1 || i ==10 || i==20 || i== 30) {
            continue;
        }
        DailyWeldData obj;
        obj.date = start_day.addDays(i).toString("yyyy_MM_dd");
        obj.weld_count = i+1;
        inter.addDailyWeldData(obj);
    }

    start_day = QDate::fromString("2024_09_02", "yyyy_MM_dd");
    QDate end_day =  QDate::fromString("2024_10_07", "yyyy_MM_dd");
    QMap<QDate, DailyWeldData> map;
    inter.getWeldDataInDateRange(start_day, end_day, map);
    for (const auto& item: map){
//        qDebug() << item.date;
//        qDebug() << item.welding_time;
//        qDebug() << item.teaching_time;
//        qDebug() << item.flat_weld_length;
//        qDebug() << item.verical_weld_length;
//        qDebug() << item.weld_count;
    }

    inter.addTodayProjectState();
}

MainWindow::~MainWindow()
{
    delete ui;
}

