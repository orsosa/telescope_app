#include "npointsdialog.h"
#include "ui_npointsdialog.h"

NpointsDialog::NpointsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NpointsDialog)
{
    ui->setupUi(this);
    ui->lineEdit->setFocus();
}

NpointsDialog::~NpointsDialog()
{
    delete ui;
}

void NpointsDialog::on_buttonBox_accepted()
{
    emit npoints_changed(ui->lineEdit->text().toInt());
}
