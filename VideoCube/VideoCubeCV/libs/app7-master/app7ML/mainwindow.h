#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "test.h"
#include <qwt_plot_curve.h>

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    QwtPlotCurve* curves[16];
    Test* test_module;

    void setupPlot();
    void closeEvent(QCloseEvent *);

    //Tests
private slots:
    void on_showOptmalButton_clicked();

    void on_showQ_clicked();
    void on_lernButton_clicked();
    void on_learnedButton_clicked();
    void on_test_nn1_clicked();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_resetW_clicked();
    void on_pushButton_4_clicked();
};

#endif // MAINWINDOW_H
