#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QDebug>



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

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    uint t_min,t_max;
    double f_min,f_max;
    int npoints;
    void getLimits();
    void setLimits();
    QVector<double> *x_data,*y_data;
};

#endif // MAINWINDOW_H
