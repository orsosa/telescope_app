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
    QSqlQuery *query;
    uint t_min,t_max,t_min_r,t_max_r;
    double f_min,f_max,f_min_r,f_max_r;
    double stored_gate;
    bool onlineFlag;
    uint offlineTimeout;
    int npoints,avg_w;
    void initLimits();
    void getLimits();
    void setLimits();
    void setPlotStyle();
    void getTelescopeData();
    QVector<double> *x_data,*y_data;
    QVector<double> *x_data_m,*y_data_m;
};

#endif // MAINWINDOW_H
