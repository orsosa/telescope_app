#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("telescopes");
    db.setUserName("diego");
    db.setPort(9999);
    //db.setPassword("");
    if (!db.open()) qDebug()<<db.lastError();
    TQtWidget *rw = ui->root_widget;
    rw->GetCanvas()->cd();
    rw->GetCanvas()->SetFillColorAlpha(kWhite,0);
    rw->GetCanvas()->SetGrid();
    npoints=10;
    /////// getting histogram limits
    getLimits();
    //////

    ft = new TH1F("ft","Frequency vs time",1e5,ft_min,ft_max);
    setHistStyle(ft);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(reDrawHist()));
    timer->start(1000);
}

void MainWindow::getLimits()
{
    QSqlQuery query;
    query.exec(Form("SELECT freq, reg_date FROM usm_telescope_data WHERE reg_date >= DATE_SUB(CURRENT_DATE, INTERVAL 1 MONTH) ORDER BY reg_date DESC LIMIT %d",npoints));
    query.next();
    ft_max = query.value(1).toDateTime().toTime_t();

    while (query.next()) {
            ft_min = query.value(1).toDateTime().toTime_t();
    }

}
void MainWindow::setHistStyle(TH1F *ft)
{
    ft->SetStats(0);
    ft->SetMarkerStyle(kFullDotLarge);
    ft->GetXaxis()->SetTimeDisplay(1);
    ft->GetXaxis()->SetTimeFormat("%H-%M-%S");
}
void MainWindow::reDrawHist()
{
    ui->root_widget->GetCanvas()->cd();
    //ui->root_widget->GetCanvas()->SetFillColor(k);
    QSqlQuery query;
    query.exec(Form("SELECT freq, reg_date FROM usm_telescope_data WHERE reg_date >= DATE_SUB(CURRENT_DATE, INTERVAL 1 MONTH) ORDER BY reg_date DESC LIMIT %d",npoints));
    while (query.next()) {
            float freq = query.value(0).toFloat();
            uint date = query.value(1).toDateTime().toTime_t();
            //qDebug() << freq<< date<<QDateTime::currentDateTime().toTime_t();
            ft->SetBinContent(ft->FindBin(date),freq);
    }
    //fh->FillRandom("gaus");
    ft->Draw("p");
    ui->root_widget->GetCanvas()->Update();

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_samplesIn_valueChanged(int arg1)
{
    delete ft;
    npoints=arg1;
    getLimits();
    ui->root_widget->GetCanvas()->cd();
    ft = new TH1F("ft","Frequency vs time",1e5,ft_min,ft_max);
    setHistStyle(ft);
}
