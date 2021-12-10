#include "mainwindow.h"
#include "mazegen.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_genButton_clicked()
{
    QImage image(600, 600, QImage::Format_RGB32);
    image.fill(Qt::black);
    MazeGen mazegen(image, ui->cellsX->value(), ui->cellsY->value(), ui->wallwidth->value());
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));
}
