/*
+ - - - + - + - -
+ - + - + copyright by Vladimir Baranov (Kvazikot)  <br>
+ - + - + email: vsbaranov83@gmail.com  <br>
+ - + - + github: http://github.com/Kvazikot/VideoProjects <br>
                          )            (
                         /(   (\___/)  )\
                        ( #)  \ ('')| ( #
                         ||___c\  > '__||
                         ||**** ),_/ **'|
                   .__   |'* ___| |___*'|
                    \_\  |' (    ~   ,)'|
                     ((  |' /(.  '  .)\ |
                      \\_|_/ <_ _____> \______________
                       /   '-, \   / ,-'      ______  \
              b'ger   /      (//   \\)     __/     /   \
                                          './_____/

*/

#include "dialog.h"
#include <shared.h>
#include <QFileDialog>
#include <QPixmap>
#include <QBuffer>
#include <QtCore/QDebug>
#include "print.h"
#include "videoscreen.h"

Dialog::Dialog(QWidget *parent)
  : QDialog(parent)
{
    ui.setupUi(this);
    map_to_prn(this, 0);
    map_to_prn(this, 1);
    connect(ui.loadFromFileButton, SIGNAL(clicked()), SLOT(loadFromFile()));
    connect(ui.loadFromSharedMemoryButton,
            SIGNAL(clicked()),
            SLOT(loadFromMemory()));
    setWindowTitle(tr("SharedMemory Example"));
    startTimer(100);
}

int Dialog::OpenSourcesTest()
{

   return 1;
}

void Dialog::showEvent(QShowEvent *event)
{
    event->accept();
    QImage image("../NicePng_vintage-paper-png_9772625.png");
    //ui.label->setPixmap(QPixmap::fromImage(image));
    sharedmem.Init();
    sharedmem.writeImage("../NicePng_vintage-paper-png_9772625.png", image.width(), image.height());
    VideoScreen* screen = new VideoScreen();
    connect(screen, SIGNAL(sigSetPixmap(int, QPixmap&)), this, SLOT(setPixmap(int, QPixmap&)));

    /// !!!!! ----- - AHTUNG RUNNING TESTS - ----------!!!!!!
    //screen->test1();
    screen->test_resize();
    connect(screen, SIGNAL(sigSetImage(int, QImage&)), &sharedmem, SLOT(writeImage(int, QImage&)));

}

void Dialog::closeEvent(QCloseEvent *)
{
    delete screen;
    //destroyAllWindows();

}

void Dialog::setPixmap(int screen_number, QPixmap& mat)
{
    ui.label->setPixmap(mat);
    repaint();
}

void Dialog::timerEvent(QTimerEvent* event)
{
    sharedmem.Update();
}

void Dialog::loadFromFile()
{
    ui.label->setText(tr("Select an image file"));
    QString fileName = QFileDialog::getOpenFileName(0, QString(), QString(),
                                        tr("Images (*.png *.xpm *.jpg)"));
    QImage image;
    if (!image.load(fileName)) {
        ui.label->setText(tr("Selected file is not an image, please select another."));
        return;
    }
    ui.label->setPixmap(QPixmap::fromImage(image));
    ui.label->setScaledContents(true);
    ui.label->repaint();

    sharedmem.writeImage(0, image);
}

void Dialog::loadFromMemory()
{

    QBuffer buffer;
    QDataStream in(&buffer);
    QImage image;
    ui.label->setPixmap(QPixmap::fromImage(image));
}


void Dialog::print(const QString &input)
{
    ui.plainTextEdit->appendPlainText(input);
}

void Dialog::print_status(const QString &input)
{
    ui.statusBar->setText(input);
}

void Dialog::on_readSrcButton_clicked()
{
    prn("Result of OpenSourcesTest is %d", OpenSourcesTest());
}
