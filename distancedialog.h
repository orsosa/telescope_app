#ifndef DISTANCEDIALOG_H
#define DISTANCEDIALOG_H

#include <QDialog>

namespace Ui {
class DistanceDialog;
}

class DistanceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DistanceDialog(QWidget *parent = 0, double stored_distance=20);
    ~DistanceDialog();

signals:
    void distance_changed(double);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DistanceDialog *ui;
};

#endif // DISTANCEDIALOG_H
