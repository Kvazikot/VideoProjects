#ifndef TRAJECTORYDIALOG_H
#define TRAJECTORYDIALOG_H

#include "../include/ploter.h"
#include <QDialog>

namespace Ui {
class TrajectoryDialog;
}

class TrajectoryDialog : public QDialog
{
    Q_OBJECT

public:
    Ploter* ploters[4];
    explicit TrajectoryDialog(QWidget *parent = 0);
    void DrawGrafics(Ploter* plot[4]);
    void closeEvent(QCloseEvent * e);
    ~TrajectoryDialog();

private:
    Ui::TrajectoryDialog *ui;

public slots:
    void print(const QString& str);
private slots:
    void OnObjsLoad();
    void on_pushButton_clicked();
    void on_clearButton_clicked();
    void on_autoscale2_clicked();
    void on_yminSB_valueChanged(double arg1);
    void on_ymaxSB_valueChanged(double arg1);
    void on_step_valueChanged(double arg1);
};

#endif // TRAJECTORYDIALOG_H
