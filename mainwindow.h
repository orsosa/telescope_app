#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QDebug>
#include "TCanvas.h"
#include "TH1F.h"


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
     void reDrawHist();

private slots:
     void on_samplesIn_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    TH1F *ft;
    QSqlDatabase db;
    uint ft_min,ft_max;
    int npoints;
    void getLimits();
    void setHistStyle(TH1F *ft);
};

#endif // MAINWINDOW_H
