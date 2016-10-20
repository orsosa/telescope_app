#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setPlotStyle();
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("200.1.16.248");
    db.setDatabaseName("telescopes");
    db.setUserName("orsosa");
    //db.setPort(9999);
    //db.setPassword("");
    if (!db.open()) qDebug()<<db.lastError();
    npoints=10;
    x_data = new QVector<double>(npoints);
    y_data = new QVector<double>(npoints);
    /////// getting histogram limits
    setLimits();
    ui->gFreqTime->graph()->setPen(QPen(Qt::blue, 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
    ui->gFreqTime->graph()->setLineStyle(QCPGraph::lsLine);
    ui->gFreqTime->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 7));
    //////
    connect(ui->gFreqTime->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->gFreqTime->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->gFreqTime->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->gFreqTime->yAxis2, SLOT(setRange(QCPRange)));
    ui->gFreqTime->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ///////
    //ui->myButton->setStyleSheet("border-image:url(default_button.png);");
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(5);
    ui->gFreqTime->setGraphicsEffect(effect);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(reDrawFreq()));
    timer->start(0);
}
void MainWindow::initLimits()
{
    f_max = -1e6;
    f_min = -1e6;
    t_max = -UINT_MAX;
    t_min = -UINT_MAX;
}

void MainWindow::getLimits()
{
    QSqlQuery query;
    query.exec(QString("SELECT freq, reg_date FROM usm_telescope_data WHERE reg_date >= DATE_SUB(CURRENT_DATE, INTERVAL 6 MONTH) ORDER BY reg_date DESC LIMIT %1").arg(npoints));
    query.next();
    if (t_max < query.value(1).toDateTime().toTime_t()) t_max =query.value(1).toDateTime().toTime_t();
    uint t_min_a;
    double f_min_a=1e6;
    while (query.next())
    {
        t_min_a = query.value(1).toDateTime().toTime_t();
         if (query.value(0).toDouble()>f_max)
             f_max=query.value(0).toDouble();
         if (query.value(0).toDouble()<f_min_a)
             f_min_a=query.value(0).toDouble();
    }
    if (t_min_a>t_min || t_min_a<t_min_r) t_min=t_min_a;
    if (f_min_a>f_min||f_min_a<f_min_r) f_min=f_min_a;
    t_min_r = t_min-0.1*(t_max-t_min);
    t_max_r = t_max+0.1*(t_max-t_min);
    f_min_r = f_min-0.1*(f_max-f_min);
    f_max_r = f_max+0.1*(f_max-f_min);

}

void MainWindow::setLimits()
{
    getLimits();
    /*if (ui->gFreqTime->xAxis->range().lower>t_min ||
        ui->gFreqTime->xAxis->range().upper<t_max ||
        ui->gFreqTime->xAxis->range().lower>f_min ||
        ui->gFreqTime->xAxis->range().upper>f_max)
    {*/

        ui->gFreqTime->xAxis->setRange(t_min_r,t_max_r);
        ui->gFreqTime->yAxis->setRange(f_min_r,f_max_r);
    //}

}

void MainWindow::setPlotStyle()
{
    ui->gFreqTime->addGraph();
    ui->gFreqTime->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->gFreqTime->xAxis->setDateTimeFormat("mm-ss");
    ui->gFreqTime->xAxis->setLabel("Time (mm-ss)");
    ui->gFreqTime->yAxis->setLabel("Frequency (Hz)");
}
void MainWindow::reDrawFreq()
{
    //ui->root_widget->GetCanvas()->SetFillColor(k);
    QSqlQuery query;
    setLimits();
    query.exec(QString("SELECT freq, reg_date FROM usm_telescope_data WHERE reg_date >= DATE_SUB(CURRENT_DATE, INTERVAL 2 MONTH) ORDER BY reg_date DESC LIMIT %1").arg(npoints));
    int i=0;
    while (query.next()) {
            double freq = query.value(0).toDouble();
            uint date = query.value(1).toDateTime().toTime_t();
            //qDebug() << freq<< date<<QDateTime::currentDateTime().toTime_t();
            (*x_data)[i] = date;
            (*y_data)[i++] =freq;
    }
    ui->gFreqTime->graph(0)->setData((*x_data),(*y_data));
    //fh->FillRandom("gaus");

    ui->gFreqTime->replot();


}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::npoints_valueChanged(int arg1)
{
    if (arg1<=npoints){initLimits();}
    if (arg1>50)ui->gFreqTime->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
    if (arg1<20)ui->gFreqTime->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 7));
    npoints=arg1;
    delete x_data;
    delete y_data;
    x_data = new QVector<double>(npoints);
    y_data = new QVector<double>(npoints);

    setLimits();
}

void MainWindow::gate_valueChanged(double arg1)
{
    QSqlQuery query;
    query.exec(QString("INSERT  INTO usm_telescope_parameters (gate)  VALUES (%1)").arg(arg1));
    //qDebug()<<db.lastError();
}

void MainWindow::on_gateButton_released()
{
    GateDialog *diag = new GateDialog(this);
    connect(diag,SIGNAL(gate_changed(double)),this,SLOT(gate_valueChanged(double)));
    diag->show();
}

void MainWindow::on_npointsButton_released()
{
    NpointsDialog *diag = new NpointsDialog(this);
    connect(diag,SIGNAL(npoints_changed(int)),this,SLOT(npoints_valueChanged(int)));
    diag->show();
}
