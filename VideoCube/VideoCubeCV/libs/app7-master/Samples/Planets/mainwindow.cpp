#include "mainwindow.h"
#include <QTimer>
#include <QScrollArea>
#include <QMessageBox>
#include <QActionGroup>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QColorDialog>
#include <QInputDialog>
#include <QApplication>
#include "glwidget.h"
#include "flags.h"
#include "camera.h"
#include "cobjregistry.h"
#include "timer.h"

static t_flags& flags = GetGFlags();
static CObjRegistry& MREG = CObjRegistry::getInstance2();
void CreateSolarSystem();

void MainWindow::loadApp6()
{
    //SuspendThread(mainThreadHandle);
    Camera* cam = new Camera();
    cam->SetName("cam01");
    cam->isCanRender = false;
    cam->isReceiveInput = true;
    cam->Declare();
    cam->speed = 100;
    cam->zFar = 3000;
    cam->isNotSaving = true;
    //cam->isThirdPerson = true;
    cam->orient.B.x = (float)M_PI;
    MREG.AddObj(cam);


    //установка флагов

    flags.bWire = true;
    flags.bShowHiddenAtrs = true;
    flags.iRenderVersion = 1;
    flags.bColorRendering = true;
    flags.bShowGrid = true;
    startTimer(10);
    //ResumeThread(mainThreadHandle);
}


void MainWindow::timerEvent(QTimerEvent* e)
{
    e=e;
    // Делаем цикл симуляции
    Timer::gtimer.msec+=Timer::gtimer._timer.elapsed();
    Timer::gtimer.fsec = Timer::gtimer.msec / 1000.;
    CObjRegistry* MREG = CObjRegistry::getInstance();
    vector<Object*>::iterator obj;
    for(obj = MREG->allObj.begin(); obj != MREG->allObj.end(); obj++ )
    {
            (*obj)->Slot1();
    }
    Timer::gtimer._timer.start();
}


MainWindow::MainWindow()
{

    createActions();
    createMenus();
    //loadPlugins();
    flags.bdisable_timer1 = true;
    Sleep(50);
    loadApp6();
    CreateSolarSystem();
    flags.bdisable_timer1 = false;
    setWindowTitle(tr("Plug & Paint"));
}

void MainWindow::createActions()
{
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    //connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAsAct = new QAction(tr("&Save As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    //connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    //connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

}

