#ifndef GATEDIALOG_H
#define GATEDIALOG_H

#include <QDialog>

namespace Ui {
class GateDialog;
}

class GateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GateDialog(QWidget *parent = 0, double stored_gate=5);
    ~GateDialog();
signals:
    void gate_changed(double arg);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::GateDialog *ui;
};

#endif // GATEDIALOG_H
