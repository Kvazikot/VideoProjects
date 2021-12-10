#include "mainwindow.h"
#include "test.h"
#include <iostream>
#include <map>
#include <random>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    setupPlot();
    test_module = new Test(curves, &nnwidget->network);
    //test_discrete_distribution();

    connect(actionSave_values, SIGNAL(triggered()), gamewidget, SLOT(saveValues()));
    connect(actionGenerate_values, SIGNAL(triggered()), gamewidget, SLOT(genValues()));
    connect(actionShowIDs,SIGNAL(triggered()), gamewidget, SLOT(showIDS()));
    connect(gamewidget,SIGNAL(print(QString)),console,SLOT(appendPlainText(QString)));
    connect(nnwidget,SIGNAL(print(QString)),console,SLOT(appendPlainText(QString)));
    connect(&nnwidget->network,SIGNAL(print(QString)),console,SLOT(appendPlainText(QString)));
    connect(&nnwidget->network,SIGNAL(clearConsole()),console,SLOT(clear()));
    connect(nnwidget,SIGNAL(clearConsole()),console,SLOT(clear()));
    connect(actionSave_NNet_weights, SIGNAL(triggered()),nnwidget,SLOT(saveNN()));
    connect(actionLoad_NNet_weights, SIGNAL(triggered()),nnwidget,SLOT(loadNN()));
    connect(actionClear_NNet, SIGNAL(triggered()),&nnwidget->network,SLOT(Clear()));
    connect(actionGenerate_NNet, SIGNAL(triggered()),nnwidget,SLOT(SlotGenerate()));
    //connect(nnwidget,SIGNAL(destroyed()),&nnwidget->network,SLOT(stopLearn()));
}

void MainWindow::closeEvent(QCloseEvent *)
{
    QMetaObject::invokeMethod(&nnwidget->network, "stopLearn", Qt::QueuedConnection);
    //delete test_module;
}


void MainWindow::setupPlot()
{
    QwtPlot* plot = qwtPlot;
    plot->setAxisAutoScale(QwtPlot::xBottom, true);
    plot->setAxisAutoScale(QwtPlot::yLeft, true);
    // названия графика и осей
    //plot->setTitle();
    plot->setFont(QFont("Arial", 8, QFont::Bold));
    plot->setAxisTitle(QwtPlot::xBottom, "");
    plot->setAxisFont(QwtPlot::xBottom, QFont("Arial", 8, QFont::Bold));
    plot->setAxisFont(QwtPlot::yLeft, QFont("Arial", 8, QFont::Bold));
    plot->setAutoReplot(true);
    for(int i=0; i<16; i++)
    {
        curves[i]= new QwtPlotCurve(QString("Curve %1").arg(i));
        curves[i]->setStyle(QwtPlotCurve::Steps);
        //curves[i]->setPen(QPen(pal[i]));
        curves[i]->attach(plot);
    }
}



void MainWindow::on_showOptmalButton_clicked()
{
    gamewidget->ShowOptimalTrajectory(10);
}

void MainWindow::on_showQ_clicked()
{
    QString out = gamewidget->PrintQOfSelected();
    console->setPlainText(out);
}

void MainWindow::on_lernButton_clicked()
{
    gamewidget->qlearn.Learning(12000, gamewidget->maxValue, gamewidget->cellsX * gamewidget->cellsY);
}

void MainWindow::on_learnedButton_clicked()
{
    gamewidget->ShowAgentTrajectory(10);
}

void MainWindow::on_test_nn1_clicked()
{
    test_module->test_learn_func(&nnwidget->network, maxEpochSB->value());
}

void MainWindow::on_pushButton_clicked()
{
    test_module->test_approximate_fun(&nnwidget->network);
}

void MainWindow::on_pushButton_2_clicked()
{
    nnwidget->network.stopLearn();
}

void MainWindow::on_pushButton_3_clicked()
{
    test_module->drawErrors(nnwidget->network.stat);
}

void MainWindow::on_resetW_clicked()
{
    nnwidget->network.ResetWeights();
}

void MainWindow::on_pushButton_4_clicked()
{
    test_module->drawStatistic(nnwidget->network.stat);
}
