#include "mainwindow.h"

#include <QApplication>
#include "qcustomplot-source/qcustomplot.h"
#include <QApplication>
#include <QMainWindow>
#include <QVector>
#include <QDebug>

#if 0
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWindow window;
    QCustomPlot customPlot;

    window.setCentralWidget(&customPlot);
    window.resize(800, 600);

    // 添加多个图形
    int numGraphs = 6; // 你可以尝试增加这个数量
    for (int i = 0; i < numGraphs; ++i) {
        QCPGraph *graph = customPlot.addGraph();
        graph->setPen(QPen(QColor::fromHsv((i * 360 / numGraphs) % 360, 255, 255), 2));
        graph->setName(QString("Graph %1").arg(i + 1));

        // 生成一些示例数据
        QVector<double> x(100), y(100);
        for (int j = 0; j < 100; ++j) {
            x[j] = j;
            y[j] = qSin(j / 10.0 + i);
        }
        graph->setData(x, y);

        qDebug() << "Graph" << i + 1 << "created with" << graph->data()->size() << "data points.";
    }

    customPlot.rescaleAxes();
    customPlot.replot();

    window.show();
    return a.exec();
}

#else
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
#endif
