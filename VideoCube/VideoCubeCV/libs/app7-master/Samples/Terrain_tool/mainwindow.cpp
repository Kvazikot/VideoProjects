#include "mainwindow.h"
#include "paintarea.h"

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
#include "terraindlg.h"
#include "material.h"
#include "glwidget.h"
#include "flags.h"
#include "camera.h"
#include "cobjregistry.h"
#include "timer.h"

static CMaterial& mater = CMaterial::getInstance();
static CObjRegistry& MREG = CObjRegistry::getInstance2();
static t_flags& flags = GetGFlags();

void MainWindow::loadApp6()
{
    flags.bdisable_timer1 = true;
    Sleep(50);
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
    flags.bShading = false;
    flags.bdisable_timer1 = false;
    startTimer(10);


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


MainWindow::MainWindow() :
    paintArea(new PaintArea),
    scrollArea(new QScrollArea)
{
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(paintArea);

    TerrainDlg* terrdlg = new TerrainDlg(this);

    setCentralWidget(scrollArea);
/*    QGridLayout* gridl = new QGridLayout(this);
    gridl->addWidget(scrollArea);
    gridl->addWidget(terrdlg);
    setLayout(gridl);
*/
    terrdlg->show();

    createActions();
    createMenus();
    //loadPlugins();
    loadApp6();

    setWindowTitle(tr("Plug & Paint"));

    if (!brushActionGroup->actions().isEmpty())
        brushActionGroup->actions().first()->trigger();

    //QTimer::singleShot(500, this, SLOT(aboutPlugins()));
}

void MainWindow::open()
{
    const QString fileName = QFileDialog::getOpenFileName(this,
                                                          tr("Open File"),
                                                          QDir::currentPath());
    if (!fileName.isEmpty()) {
        if (!paintArea->openImage(fileName)) {
            QMessageBox::information(this, tr("Plug & Paint"),
                                     tr("Cannot load %1.").arg(fileName));
            return;
        }
        paintArea->adjustSize();
    }
}

bool MainWindow::saveAs()
{
    const QString initialPath = QDir::currentPath() + "/untitled.png";

    const QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                          initialPath);
    if (fileName.isEmpty()) {
        return false;
    } else {
        return paintArea->saveImage(fileName, "png");
    }
}

void MainWindow::brushColor()
{
    const QColor newColor = QColorDialog::getColor(paintArea->brushColor());
    if (newColor.isValid())
        paintArea->setBrushColor(newColor);
}

void MainWindow::brushWidth()
{
    bool ok;
    const int newWidth = QInputDialog::getInt(this, tr("Plug & Paint"),
                                              tr("Select brush width:"),
                                              paintArea->brushWidth(),
                                              1, 50, 1, &ok);
    if (ok)
        paintArea->setBrushWidth(newWidth);
}

//! [0]
void MainWindow::changeBrush()
{
}
//! [0]

//! [1]
void MainWindow::insertShape()
{
}
//! [1]

//! [2]
void MainWindow::applyFilter()
{
}
//! [2]

void MainWindow::about()
{
   QMessageBox::about(this, tr("About Plug & Paint"),
            tr("The <b>Plug & Paint</b> example demonstrates how to write Qt "
               "applications that can be extended through plugins."));
}

//! [3]
void MainWindow::aboutPlugins()
{
    //PluginDialog dialog(pluginsDir.path(), pluginFileNames, this);
   // dialog.exec();
}
//! [3]

void MainWindow::createActions()
{
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAsAct = new QAction(tr("&Save As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    brushColorAct = new QAction(tr("&Brush Color..."), this);
    connect(brushColorAct, SIGNAL(triggered()), this, SLOT(brushColor()));

    brushWidthAct = new QAction(tr("&Brush Width..."), this);
    connect(brushWidthAct, SIGNAL(triggered()), this, SLOT(brushWidth()));

    brushActionGroup = new QActionGroup(this);

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    aboutPluginsAct = new QAction(tr("About &Plugins"), this);
    connect(aboutPluginsAct, SIGNAL(triggered()), this, SLOT(aboutPlugins()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    brushMenu = menuBar()->addMenu(tr("&Brush"));
    brushMenu->addAction(brushColorAct);
    brushMenu->addAction(brushWidthAct);
    brushMenu->addSeparator();

    shapesMenu = menuBar()->addMenu(tr("&Shapes"));

    filterMenu = menuBar()->addMenu(tr("&Filter"));

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
    helpMenu->addAction(aboutPluginsAct);
}

