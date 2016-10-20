#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->window()->setWindowTitle("SAMAHO");
    onlineFlag = false;
    offlineTimeout = 70;
    setPlotStyle();
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("200.1.16.248");
    db.setDatabaseName("telescopes");
    db.setUserName("orsosa");
    //db.setPort(9999);
    //db.setPassword("");
    if (!db.open()) qDebug()<<db.lastError();
    ui->wifi_frame->setStyleSheet(\
        "background-color: transparent;\
         background-image: url(\"wifi_off.png\");\
         background-repeat: none; border: none;");
    npoints=10;
    x_data = new QVector<double>(npoints);
    y_data = new QVector<double>(npoints);
    x_data_m = new QVector<double>(npoints-avg_w);
    y_data_m = new QVector<double>(npoints-avg_w);
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

void MainWindow::getTelescopeData()
{
    QSqlQuery query;
    query.exec(QString("SELECT freq, reg_date FROM usm_telescope_data WHERE reg_date >= DATE_SUB(CURRENT_DATE, INTERVAL 6 MONTH) ORDER BY reg_date DESC LIMIT %1").arg(npoints));
    query.next();
    if (t_max < query.value(1).toDateTime().toTime_t()) t_max =query.value(1).toDateTime().toTime_t();
    uint t_min_a;
    double f_min_a=1e6;
    while (query.next())
    {
        //warning
        qDebug()<<db.lastError();
    }

     query->next();
     if (t_max < query->value(1).toDateTime().toTime_t()) t_max =query->value(1).toDateTime().toTime_t();
     int i=0;
     double freq = query->value(0).toDouble();
     uint date = query->value(1).toDateTime().toTime_t();
     if ((QDateTime::currentDateTime().toTime_t() - date ) < offlineTimeout) onlineFlag=true;
     if (onlineFlag)
        ui->wifi_frame->setStyleSheet("background-image: url(\"wifi_on.png\");");
     else
        ui->wifi_frame->setStyleSheet("background-image: url(\"wifi_off.png\");");
     (*x_data)[i] = date;
     (*y_data)[i++] =freq;
     double f_min_a=1e6;
     double f_avg=0;
     uint t_avg=0;
     while (query->next()) {
        freq = query->value(0).toDouble();
        date = query->value(1).toDateTime().toTime_t();
        (*x_data)[i] = date;
        /*f_avg+=freq;
        t_avg+=date;
        if (i>avg_w)
        {
            (*x_data_m)[i-avg_w] = t_avg;
            (*y_data_m)[i-avg_w] = f_avg;
        }
         */
        (*y_data)[i++] =freq;
        if (freq>f_max)
        f_max=freq;
        if (freq<f_min_a)
        f_min_a=freq;
     }
     if (date>t_min || date<t_min_r) t_min=date;
     if (f_min_a>f_min || f_min_a<f_min_r) f_min=f_min_a;
     t_min_r = t_min-0.1*(t_max-t_min);
     t_max_r = t_max+0.1*(t_max-t_min);
     f_min_r = f_min-0.1*(f_max-f_min);
     f_max_r = f_max+0.1*(f_max-f_min);
}

void MainWindow::setLimits()
{
    if (t_min_r>t_min ||
        t_max_r<t_max ||
        f_min_r>f_min ||
        f_max_r>f_max)
    {
    ui->gFreqTime->xAxis->setRange(t_min_r,t_max_r);
    ui->gFreqTime->yAxis->setRange(f_min_r,f_max_r);
    }
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
    getTelescopeData();
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
    getTelescopeData();
    setLimits();
}

void MainWindow::gate_valueChanged(double arg1)
{
    QSqlQuery query;
    if(!query.exec(QString("INSERT  INTO usm_telescope_parameters (gate)  VALUES (%1)").arg(arg1)))
        qDebug()<<db.lastError();
}

void MainWindow::on_gateButton_released()
{
    if (!query->exec(QString(\
    "SELECT gate FROM usm_telescope_parameters\
    WHERE reg_date >= DATE_SUB(CURRENT_DATE, INTERVAL 2 MONTH)\
    ORDER BY reg_date DESC LIMIT 1")))
    {
    //warning
    qDebug()<<db.lastError();
    }
    query->next();
    stored_gate=query->value(0).toDouble();
    GateDialog *diag = new GateDialog(this,stored_gate);
    connect(diag,SIGNAL(gate_changed(double)),this,SLOT(gate_valueChanged(double)));
    diag->show();
}

void MainWindow::on_npointsButton_released()
{
    NpointsDialog *diag = new NpointsDialog(this,npoints);
    connect(diag,SIGNAL(npoints_changed(int)),this,SLOT(npoints_valueChanged(int)));
    diag->show();
}
