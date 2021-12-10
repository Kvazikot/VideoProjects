#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QDebug>
#include "../include/geometry.h"

char gstr3[1000000];


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QImage maze("maze.png");
    ui->setupUi(this);
    ui->label->setPixmap(QPixmap::fromImage(maze));
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* ev)
{
   ui->label->keyPressEvent(ev);
}
