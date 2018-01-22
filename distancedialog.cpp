#include "distancedialog.h"
#include "ui_distancedialog.h"

DistanceDialog::DistanceDialog(QWidget *parent,double stored_distance) :
    QDialog(parent),
    ui(new Ui::DistanceDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Radius selection");
    ui->lineEdit->setText(QString("%1").arg(stored_distance));
    ui->lineEdit->setFocus();
}

DistanceDialog::~DistanceDialog()
{
    delete ui;
}

void DistanceDialog::on_buttonBox_accepted()
{
    emit distance_changed(ui->lineEdit->text().toInt());
}
