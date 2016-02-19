#include "gatedialog.h"
#include "ui_gatedialog.h"

GateDialog::GateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GateDialog)
{
    ui->setupUi(this);
    ui->lineEdit->setFocus();
}

GateDialog::~GateDialog()
{
    delete ui;
}

void GateDialog::on_buttonBox_accepted()
{
    emit gate_changed(ui->lineEdit->text().toDouble());
}
