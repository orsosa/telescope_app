#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->window()->setWindowTitle("HuLC");
    onlineFlag = false;
    offlineTimeout = 70;
    setPlotStyle();
    db = QSqlDatabase::addDatabase("QMYSQL");
    //db.setHostName("200.1.16.248");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("telescopes");
    db.setUserName("orsosa");
    db.setPort(10000);
    //db.setPassword("");
    if (!db.open()) qDebug()<<db.lastError();
    query = new  QSqlQuery();
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
    //ui->gFreqTime->graph()->setPen(QPen(Qt::blue, 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
    //ui->gFreqTime->graph()->setLineStyle(QCPGraph::lsLine);
    //ui->gFreqTime->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 7));
    


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
    f_min = 1e6;
    t_max = -UINT_MAX;
    t_min = UINT_MAX;
}

void MainWindow::getTelescopeData()
{
  //    QSqlQuery query;
    query->exec(QString("SELECT freq, reg_date FROM usm_telescope_data WHERE reg_date >= DATE_SUB(CURRENT_DATE, INTERVAL 6 MONTH) and freq < 100 ORDER BY reg_date DESC LIMIT %1").arg(npoints));
    query->next();
    /*    if (t_max < query->value(1).toDateTime().toTime_t()) t_max =query->value(1).toDateTime().toTime_t();
    uint t_min_a;
    double f_min_a=1e6;
    while (query.next())
    {
        //warning
        qDebug()<<db.lastError();
    }

     query->next();
    */
     if (t_max < query->value(1).toDateTime().toTime_t()) t_max =query->value(1).toDateTime().toTime_t();
     int i=0;
     double freq = query->value(0).toDouble();
     uint date = query->value(1).toDateTime().toTime_t();
     if ((QDateTime::currentDateTime().toTime_t() - date ) < offlineTimeout) onlineFlag=true;
     if (onlineFlag)
        ui->wifi_frame->setStyleSheet("background-image: url(\"wifi_on.png\");");
     else
        ui->wifi_frame->setStyleSheet("background-image: url(\"wifi_off.png\");");

     double f_avg=freq;
     double f_sd=freq*freq;
     double f_min_a=1e6;

     (*x_data)[i] = date;
     (*y_data)[i++] =freq;
     if (freq>f_max)
       f_max=freq;
     if (freq<f_min_a)
       f_min_a=freq;

     //uint t_avg=0;
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

        f_avg+=freq;
        f_sd +=freq*freq;
     }

     f_avg=f_avg/i;
     f_sd=( f_sd/(i-1) > i/(i-1)*pow(f_avg,2) )?( sqrt(f_sd/(i-1) - i/(i-1)*pow(f_avg,2)) ):-1.0;
     ui->label_avg->setNum(f_avg);
     ui->label_sd->setNum(f_sd);

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
      t_min_r = t_min-0.1*(t_max-t_min);
      t_max_r = t_max+0.1*(t_max-t_min);
      f_min_r = f_min-0.1*(f_max-f_min);
      f_max_r = f_max+0.1*(f_max-f_min);
      ui->gFreqTime->xAxis->setRange(t_min_r,t_max_r);
      ui->gFreqTime->yAxis->setRange(f_min_r,f_max_r);
    }
}

void MainWindow::setPlotStyle()
{
    ui->gFreqTime->addGraph();
    ui->gFreqTime->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 9));
    ui->gFreqTime->graph()->setPen(QPen(QColor(120, 120, 120), 2, Qt::DashLine));

    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    ui->gFreqTime->setBackground(plotGradient);
    ui->gFreqTime->xAxis->setTickLabelColor(Qt::white);
    ui->gFreqTime->yAxis->setTickLabelColor(Qt::white);
    ui->gFreqTime->xAxis->setLabelColor(Qt::white);
    ui->gFreqTime->yAxis->setLabelColor(Qt::white);

    ui->gFreqTime->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->gFreqTime->xAxis->setDateTimeFormat("mm-ss");
    ui->gFreqTime->xAxis->setLabel("Time (mm-ss)");
    ui->gFreqTime->yAxis->setLabel("Frequency (Hz)");
}
void MainWindow::reDrawFreq()
{
    getTelescopeData();
    setLimits();
/*    query->exec(QString("SELECT freq, reg_date FROM usm_telescope_data WHERE reg_date >= DATE_SUB(CURRENT_DATE, INTERVAL 2 MONTH) ORDER BY reg_date DESC LIMIT %1").arg(npoints));
    int i=0;
    while (query->next()) {
            double freq = query->value(0).toDouble();
            uint date = query->value(1).toDateTime().toTime_t();
            //qDebug() << freq<< date<<QDateTime::currentDateTime().toTime_t();
            (*x_data)[i] = date;
            (*y_data)[i++] =freq;
    }
    */
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
    if (arg1>50) ui->gFreqTime->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 5));
    if (arg1<20) ui->gFreqTime->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 9));
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
    if (!query->exec(QString(\
    "SELECT azimuth_angle,distance,polar_angle FROM usm_telescope_parameters\
    WHERE reg_date >= DATE_SUB(CURRENT_DATE, INTERVAL 2 MONTH)\
    ORDER BY reg_date DESC LIMIT 1")))
    {
    //warning
    qDebug()<<db.lastError();
    }
    /*
    query->next();
    double azimuth=query->value(0).toDouble();
    query->next();
    double distance = query->value(1).toDouble();
    query->next();
    double polar = query->value(2).toDouble();

    query.prepare("INSERT  INTO usm_telescope_parameters (gate,azimuth_angle,polar_angle,distance)" "VALUES(:gate,:azimuth_angle,:polar_angle,:distance)");
    query.bindValue(":gate",arg1);
    query.bindValue(":azimuth_angle",azimuth);
    query.bindValue(":distance",distance);
    query.bindValue(":polar_angle",polar);
    if(!query.exec()) qDebug()<<db.lastError();
    */
    //QSqlQuery query_send;
    query->first();
    if(!query->exec(QString("INSERT  INTO usm_telescope_parameters (gate,azimuth_angle,polar_angle,distance)  VALUES (%1,%2,%3,%4)").arg(arg1).arg(query->value(0).toDouble()).arg(query->value(2).toDouble()).arg(query->value(1).toDouble())))
        qDebug()<<db.lastError();
}

void MainWindow::azimuth_valueChanged(double arg1)
{
    if (!query->exec(QString(\
        "SELECT gate,distance,polar_angle FROM usm_telescope_parameters\
        WHERE reg_date >= DATE_SUB(CURRENT_DATE, INTERVAL 2 MONTH)\
        ORDER BY reg_date DESC LIMIT 1")))
    {
        //warning
        qDebug()<<db.lastError();
    }
/*
    query->next();
    double gate=query->value(0).toDouble();
    query->next();
    double distance = query->value(1).toDouble();
    query->next();
    double polar = query->value(2).toDouble();

    query.prepare("INSERT  INTO usm_telescope_parameters (gate,azimuth_angle,polar_angle,distance)" "VALUES(:gate,:azimuth_angle,:polar_angle,:distance)");
    query.bindValue("gate",gate);
    query.bindValue(":azimuth_angle",arg1);
    query.bindValue(":polar_angle",polar);
    query.bindValue(":distance",distance);
    if(!query.exec()) qDebug()<<db.lastError();
    */
    query->first();
    //QSqlQuery query_send;
    if(!query->exec(QString("INSERT  INTO usm_telescope_parameters (gate,azimuth_angle,polar_angle,distance)  VALUES (%1,%2,%3,%4)").arg(query->value(0).toDouble()).arg(arg1).arg(query->value(2).toDouble()).arg(query->value(1).toDouble())))
    qDebug()<<db.lastError();

}

void MainWindow::distance_valueChanged(double arg1)
{
    if (!query->exec(QString(\
          "SELECT gate,azimuth_angle,polar_angle FROM usm_telescope_parameters\
           WHERE reg_date >= DATE_SUB(CURRENT_DATE, INTERVAL 2 MONTH)\
           ORDER BY reg_date DESC LIMIT 1")))
    {
    //warning
        qDebug()<<db.lastError();
    }

/*
    query->next();
    double gate=query->value(0).toDouble();
    query->next();
    double azimuth = query->value(1).toDouble();
    query->next();
    double polar = query->value(2).toDouble();

    query.prepare("INSERT  INTO usm_telescope_parameters (gate,azimuth_angle,polar_angle,distance)" "VALUES(:gate,:azimuth_angle,:polar_angle,:distance)");
    query.bindValue(":gate",gate);
    query.bindValue(":azimuth_angle",azimuth);
    query.bindValue(":polar_angle",polar);
    query.bindValue(":distance",arg1);
    if(!query.exec()) qDebug()<<db.lastError();
    */
    query->first();
    //QSqlQuery query_send;
    if(!query->exec(QString("INSERT  INTO usm_telescope_parameters (gate,azimuth_angle,polar_angle,distance)  VALUES (%1,%2,%3,%4)").arg(query->value(0).toDouble()).arg(query->value(1).toDouble()).arg(query->value(2).toDouble()).arg(arg1)))
        qDebug()<<db.lastError();

}
void MainWindow::polar_valueChanged(double arg1)
{
    if (!query->exec(QString(\
                    "SELECT gate,azimuth_angle,distance FROM usm_telescope_parameters\
                     WHERE reg_date >= DATE_SUB(CURRENT_DATE, INTERVAL 2 MONTH)\
                     ORDER BY reg_date DESC LIMIT 1")))
    {
        //warning
        qDebug()<<db.lastError();
    }

/*
    query->next();
    double gate=query->value(0).toDouble();
    query->next();
    double azimuth = query->value(1).toDouble();
    query->next();
    double distance = query->value(2).toDouble();

    query.prepare("INSERT  INTO usm_telescope_parameters (gate,azimuth_angle,polar_angle,distance)" "VALUES(:gate,:azimuth_angle,:polar_angle,:distance)");
    query.bindValue(":gate",gate);
    query.bindValue(":azimuth_angle",azimuth);
    query.bindValue(":polar_angle",arg1);
    query.bindValue(":distance",distance);
    if(!query.exec()) qDebug()<<db.lastError();
    */
    query->first();
    //QSqlQuery query_send;
    if(!query->exec(QString("INSERT  INTO usm_telescope_parameters (gate,azimuth_angle,polar_angle,distance)  VALUES (%1,%2,%3,%4)").arg(query->value(0).toDouble()).arg(query->value(1).toDouble()).arg(arg1).arg(query->value(2).toDouble())))
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

void MainWindow::on_azimuthButton_released()
{
    if (!query->exec(QString("SELECT azimuth_angle FROM usm_telescope_parameters\
     WHERE reg_date >= DATE_SUB(CURRENT_DATE, INTERVAL 2 MONTH)\
     ORDER BY reg_date DESC LIMIT 1")))
    {
    //warning
    qDebug()<<db.lastError();
    }
    query->next();
    stored_azimuth = query->value(0).toDouble();
    AzimuthDialog *diag = new AzimuthDialog(this,stored_azimuth);
    connect(diag,SIGNAL(azimuth_changed(double)),this,SLOT(azimuth_valueChanged(double)));
    diag->show();
}

void MainWindow::on_distanceButton_released()
{
    if (!query->exec(QString("SELECT distance FROM usm_telescope_parameters\
        WHERE reg_date >= DATE_SUB(CURRENT_DATE, INTERVAL 2 MONTH)\
        ORDER BY reg_date DESC LIMIT 1")))
    {
    //warning
     qDebug()<<db.lastError();
    }
    query->next();
    stored_distance=query->value(0).toDouble();

    DistanceDialog *diag = new DistanceDialog(this,stored_distance);
    connect(diag,SIGNAL(distance_changed(double)),this,SLOT(distance_valueChanged(double)));
    diag->show();
}

void MainWindow::on_polarButton_released()
{
    if (!query->exec(QString("SELECT polar_angle FROM usm_telescope_parameters\
                              WHERE reg_date >= DATE_SUB(CURRENT_DATE, INTERVAL 2 MONTH)\
                              ORDER BY reg_date DESC LIMIT 1")))
    {
        //warning
        qDebug()<<db.lastError();
    }
    query->next();
    stored_polar=query->value(0).toDouble();
    PolarDialog *diag = new PolarDialog(this,stored_polar);
    connect(diag,SIGNAL(polar_changed(double)),this,SLOT(polar_valueChanged(double)));
    diag->show();
}



