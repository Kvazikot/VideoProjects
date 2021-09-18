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
#include <QBuffer>
#include <QtCore/QDebug>
#include "print.h"
#include "videoscreen.h"

Shared sharedmem;

Dialog::Dialog(QWidget *parent)
  : QDialog(parent)
{
    ui.setupUi(this);
    map_to_prn(this);
    connect(ui.loadFromFileButton, SIGNAL(clicked()), SLOT(loadFromFile()));
    connect(ui.loadFromSharedMemoryButton,
            SIGNAL(clicked()),
            SLOT(loadFromMemory()));
    setWindowTitle(tr("SharedMemory Example"));
    startTimer(100);
}

void Dialog::showEvent(QShowEvent *event)
{
    QImage image("../NicePng_vintage-paper-png_9772625.png");
    ui.label->setPixmap(QPixmap::fromImage(image));
    sharedmem.Init();
    sharedmem.writeImage("../NicePng_vintage-paper-png_9772625.png", image.width(), image.height());
    VideoScreen* screen = new VideoScreen();
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

    sharedmem.writeImage(image);
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
