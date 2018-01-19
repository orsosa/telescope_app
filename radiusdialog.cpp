#include "radiusdialog.h"
#include "ui_radiusdialog.h"

RadiusDialog::RadiusDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RadiusDialog)
{
    ui->setupUi(this);
}

RadiusDialog::~RadiusDialog()
{
    delete ui;
}
