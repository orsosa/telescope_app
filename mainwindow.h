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
#include <QPen>
#include "gatedialog.h"
#include "npointsdialog.h"



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
     void npoints_valueChanged(int arg1);

     void gate_valueChanged(double arg1);

     void on_gateButton_released();

     void on_npointsButton_released();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    uint t_min,t_max,t_min_r,t_max_r;
    double f_min,f_max,f_min_r,f_max_r;
    int npoints;
    void initLimits();
    void getLimits();
    void setLimits();
    void setPlotStyle();
    QVector<double> *x_data,*y_data;
};

#endif // MAINWINDOW_H
