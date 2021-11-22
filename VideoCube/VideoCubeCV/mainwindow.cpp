#include "mainwindow.h"
#include "keybertwrapper.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    KeyBERTWrapper bert_object;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_markKeywordsButton_clicked()
{

}
