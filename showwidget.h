#ifndef SHOWWIDGET_H
#define SHOWWIDGET_H

#include <QWidget>
#include "qcustomplot-source/qcustomplot.h"

class ShowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShowWidget(QWidget *parent = nullptr);
    void testcase_1();
    void testcase_2();

private:
    QCustomPlot *polt_;
    QCustomPlot *plot_1_;
};

#endif // SHOWWIDGET_H
