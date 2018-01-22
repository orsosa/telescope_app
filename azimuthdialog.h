#ifndef AZIMUTHDIALOG_H
#define AZIMUTHDIALOG_H

#include <QDialog>

namespace Ui {
class AzimuthDialog;
}

class AzimuthDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AzimuthDialog(QWidget *parent = 0, double stored_azimuth=0);
    ~AzimuthDialog();

signals:
    void azimuth_changed(double arg);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AzimuthDialog *ui;
};

#endif // AZIMUTHDIALOG_H
