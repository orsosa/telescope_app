#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include "defaultdialog.h"
#include "npointsdiag.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
     void reDrawFreq();

private slots:
     void on_samplesIn_valueChanged(int arg1);

     void on_gateIn_valueChanged(double arg1);

     void on_gateButton_released();

     void on_pushButton_2_released();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    uint t_min,t_max;
    double f_min,f_max;
    int npoints;
    void getLimits();
    void setLimits();
    void setPlotStyle();
    QVector<double> *x_data,*y_data;
};

#endif // MAINWINDOW_H
