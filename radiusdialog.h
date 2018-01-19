#ifndef RADIUSDIALOG_H
#define RADIUSDIALOG_H

#include <QDialog>

namespace Ui {
class RadiusDialog;
}

class RadiusDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RadiusDialog(QWidget *parent = 0);
    ~RadiusDialog();

private:
    Ui::RadiusDialog *ui;
};

#endif // RADIUSDIALOG_H
