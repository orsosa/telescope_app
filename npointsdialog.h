#ifndef NPOINTSDIALOG_H
#define NPOINTSDIALOG_H

#include <QDialog>

namespace Ui {
class NpointsDialog;
}

class NpointsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NpointsDialog(QWidget *parent = 0);
    ~NpointsDialog();

signals:
    void npoints_changed(int);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::NpointsDialog *ui;
};

#endif // NPOINTSDIALOG_H
