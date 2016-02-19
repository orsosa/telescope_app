#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setPlotStyle();
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("telescopes");
    db.setUserName("diego");
    db.setPort(9999);
    //db.setPassword("");
    if (!db.open()) qDebug()<<db.lastError();

    npoints=10;
    x_data = new QVector<double>(npoints);
    y_data = new QVector<double>(npoints);
    /////// getting histogram limits
    getLimits();
    ui->gFreqTime->xAxis->setRange(t_min,t_max);
    ui->gFreqTime->yAxis->setRange(f_min,f_max);
    ui->gFreqTime->graph()->setLineStyle(QCPGraph::lsNone);
    ui->gFreqTime->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 6));
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
    timer->start(1000);
}

void MainWindow::getLimits()
{
    QSqlQuery query;
    query.exec(QString("SELECT freq, reg_date FROM usm_telescope_data WHERE reg_date >= DATE_SUB(CURRENT_DATE, INTERVAL 1 MONTH) ORDER BY reg_date DESC LIMIT %1").arg(npoints));
    query.next();
    t_max = query.value(1).toDateTime().toTime_t();
    f_max = -1e6;
    f_min = 1e6;

    while (query.next())
    {
        t_min = query.value(1).toDateTime().toTime_t();
         if (query.value(0).toDouble()>f_max)
             f_max=query.value(0).toDouble();
         if (query.value(0).toDouble()<f_min)
             f_min=query.value(0).toDouble();
    }
    t_min = t_min-0.1*(t_max-t_min);
    t_max = t_max+0.1*(t_max-t_min);
    f_min = f_min-0.1*(f_max-f_min);
    f_max = f_max+0.1*(f_max-f_min);

}

void MainWindow::setLimits()
{
    getLimits();
    ui->gFreqTime->xAxis->setRange(t_min,t_max);
    ui->gFreqTime->yAxis->setRange(f_min,f_max);

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
    query.exec(QString("SELECT freq, reg_date FROM usm_telescope_data WHERE reg_date >= DATE_SUB(CURRENT_DATE, INTERVAL 1 MONTH) ORDER BY reg_date DESC LIMIT %1").arg(npoints));
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

void MainWindow::on_samplesIn_valueChanged(int arg1)
{
    npoints=arg1;
    delete x_data;
    delete y_data;
    x_data = new QVector<double>(npoints);
    y_data = new QVector<double>(npoints);

    getLimits();
    ui->gFreqTime->xAxis->setRange(t_min,t_max);
    ui->gFreqTime->yAxis->setRange(f_min,f_max);
}

void MainWindow::on_gateIn_valueChanged(double arg1)
{
    QSqlQuery query;
    query.exec(QString("INSERT  INTO usm_telescope_parameters (gate)  VALUES (%1)").arg(arg1));
    qDebug()<<db.lastError();
}

void MainWindow::on_gateButton_released()
{
    DefaultDialog *diag = new DefaultDialog(this);
    connect(diag,SIGNAL(gate_changed(double)),this,SLOT(on_gateIn_valueChanged(double)));
    diag->show();
}

void MainWindow::on_pushButton_2_released()
{
    NpointsDiag *diag = new NpointsDiag(this);
    connect(diag,SIGNAL(npoints_changed(int)),this,SLOT(on_samplesIn_valueChanged(int)));
    diag->show();
}
