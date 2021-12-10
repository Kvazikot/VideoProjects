#include <QTextStream>
#include <QFile>
#include <QTextCodec>
#include "../include/print.h"
#include "trajectorydialog.h"
#include "ui_trajectorydialog.h"
#include "../include/var_parse.h"
#include "../include/ploter.h"
#include <qwt/qwt_plot.h>
#include "../include/timer.h"
#include <iostream>
#include <fstream>
#include "../include/traj_gen.h"

extern VarParser varparser;
extern TrajGen traj_gen; // синглтон

TrajectoryDialog* dlg=0;
double time_last;

TrajectoryDialog::TrajectoryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TrajectoryDialog)
{
    ui->setupUi(this);

    map_to_prn(this);
    dlg = this;
    time_last = Timer::gtimer.fsec;

    QFile f("traj.ini");
    if( !f.open( QFile::ReadOnly ) )
    {
        print("Cannot open traj.ini file!");
        return;
    }
    QTextStream ts(&f);
    ts.setCodec("cp1251");

    QString allStrings = ts.readAll();
    varparser.inputS = allStrings.toStdString();
    varparser.ParseInput();
    ui->varEdit->setPlainText(allStrings);
    // классы traj_gen берет данные из таблицы vt
    traj_gen.Init();
    traj_gen.PrintInfo();
    traj_gen.GenerateLaws();

    //создать графики
    //графики для гистограм
    ploters[0] = new Ploter(ui->qwtframe1);
    ploters[0]->plot->setGeometry(ui->qwtframe1->rect());
    ploters[1] = new Ploter(ui->qwtframe2);
    ploters[1]->plot->setGeometry(ui->qwtframe2->rect());
    ploters[2] = new Ploter(ui->qwtframe3);
    ploters[2]->plot->setGeometry(ui->qwtframe3->rect());
    ploters[3] = new Ploter(ui->qwtframe4);
    ploters[3]->plot->setGeometry(ui->qwtframe4->rect());
    for(uint i=0; i<4; i++)
    {
        QRect rc = ploters[i]->plot->geometry();
        rc.setWidth(400);
        ploters[i]->plot->setGeometry(rc);
    }

    //DrawGrafics(ploters);
}

void TrajectoryDialog::print(const QString& str)
{
    ui->console->appendPlainText(str);
    ui->console->moveCursor(QTextCursor::End);
}

void TrajectoryDialog::OnObjsLoad()
{

}

TrajectoryDialog::~TrajectoryDialog()
{
    delete ui;
}

void TrajectoryDialog::on_pushButton_clicked()
{
    varparser.inputS = ui->varEdit->toPlainText().toStdString();
    varparser.ParseInput();
    // классы traj_gen берет данные из таблицы vt
    traj_gen.Reset();
    traj_gen.Init();
    traj_gen.PrintInfo();
    traj_gen.GenerateLaws();
    DrawGrafics(ploters);
}

// нарисовать графики сгенерированных законов для скоростей, ускорений, углов
// f - частота снятия данных
void TrajectoryDialog::DrawGrafics(Ploter* plot[4])
{
    double t;
    double dt;

    for(uint i=0; i<4; i++)
    {
        plot[i]->clearvals(0);
        plot[i]->clearvals(1);
        plot[i]->clearvals(2);
    }

    dt = (double) 1. / traj_gen.f_dat;
    int cnt =0;
    t = 0;

    ofstream out_trajectory("trajectory.txt");

    out_trajectory << "# t Yaw Pitch Roll x y z  \n";

    out_trajectory.precision(10);

    while( t < traj_gen.t_rascheta )
    {
        plot[2]->setXY(0,t,traj_gen.GetYaw(t));
        plot[2]->setXY(1,t,traj_gen.GetPitch(t));
        plot[2]->setXY(2,t,traj_gen.GetRoll(t));

        //out_trajectory << t << "\t" << traj_gen.GetYaw(t) << "\t" << traj_gen.GetPitch(t) << "\t" << traj_gen.GetRoll(t);

        plot[0]->setXY(0,t,traj_gen.Getvx(t));
        plot[0]->setXY(1,t,traj_gen.Getvy(t));
        plot[0]->setXY(2,t,traj_gen.Getvz(t));

        plot[3]->setXY(0,t,traj_gen.Getx(t));
        plot[3]->setXY(1,t,traj_gen.Gety(t));
        plot[3]->setXY(2,t,traj_gen.Getz(t));
        //out_trajectory << "\t" << traj_gen.Getx(t) << "\t" << traj_gen.Gety(t) << "\t" << traj_gen.Getz(t) << "\n";


        plot[1]->setXY(0,t,traj_gen.Getax(t));
        plot[1]->setXY(1,t,traj_gen.Getay(t));
        plot[1]->setXY(2,t,traj_gen.Getaz(t));

       // if(cnt > 200) break;

        t+=dt;
        cnt++;
    }

}


void TrajectoryDialog::closeEvent(QCloseEvent * e)
{
    e=e;
    QFile f("traj.ini");
    if( !f.open( QFile::WriteOnly ) )
    {
        print("Cannot open traj.ini file!");
        return;
    }
    QTextStream ts(&f);
    ts.setCodec("cp1251");
    ts << ui->varEdit->toPlainText();
}

void plot(int plot_n, int curve_n, double X, double Y)
{
    if(plot_n > 4) return;
    dlg->ploters[plot_n]->setXY(curve_n, X, Y);
}

void plot_skip(int plot_n, int curve_n, double update_interval, double X, double Y)
{
    if(plot_n > 4) return;
    double time;
    time = Timer::gtimer.fsec;
    if( (time - time_last) > update_interval || (time - time_last) == 0 )
    {
        dlg->ploters[plot_n]->setXY(curve_n, X, Y);
        time_last = Timer::gtimer.fsec;
    }

}

void TrajectoryDialog::on_clearButton_clicked()
{
    for(uint i=0; i<4; i++)
    {
        for(uint j=0; j<16; j++)
          ploters[i]->clearvals(j);
    }

}

void TrajectoryDialog::on_autoscale2_clicked()
{
     //ploters[1]->set_autoscale(ui->autoscale2->isChecked());
     if( ui->autoscale2->isChecked() )
        ploters[1]->plot->setAxisAutoScale(QwtPlot::yLeft);
     else
        ploters[1]->plot->setAxisScale(QwtPlot::yLeft, ui->yminSB->value(), ui->ymaxSB->value(), 0);
}

void TrajectoryDialog::on_yminSB_valueChanged(double arg1)
{
    ploters[1]->plot->setAxisScale(QwtPlot::yLeft, ui->yminSB->value(), ui->ymaxSB->value(), 0);
}

void TrajectoryDialog::on_ymaxSB_valueChanged(double arg1)
{
    ploters[1]->plot->setAxisScale(QwtPlot::yLeft, ui->yminSB->value(), ui->ymaxSB->value(), 0);
}

void TrajectoryDialog::on_step_valueChanged(double arg1)
{
    ui->yminSB->setSingleStep(arg1);
    ui->ymaxSB->setSingleStep(arg1);
}
