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

#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <opencv2/opencv.hpp>
#include <QtConcurrent>
#include <QApplication>
#include <QFuture>
#include <QThread>
#include <print.h>
#include "timer.h"
#include "videoscreen.h"

#define THREADS_NUMBER 8
using namespace cv;

#define LOG()    qDebug() << __func__ << ": "


class RandomFillBody : public ParallelLoopBody
{
    Mat3b img;

public:
    RandomFillBody(Mat3b& img) : img(img)
    { }

    void operator()(const Range& range) const override
    {
        theRNG().state = getTickCount();
        Vec3b color{ theRNG(), theRNG(), theRNG() };

        for (int i = range.start; i < range.end; ++i)
        {
            int x = i % 8, y = i / 8;
            img(Range{ y * 10, (y + 1) * 10 }, Range{ x * 10, (x + 1) * 10 }) = color;
        }
    }
};



VideoScreen::VideoScreen()
    : QObject()
{
    frame_rate = 30;
}

void VideoScreen::test1()
{
    int n = cv::getNumberOfCPUs();
    prn("getNumberOfCPUs() = %d ", n);
    int n1 = cv::getNumThreads();
    prn("getNumThreads() = %d", n1);
    Mat3b img = Mat3b::zeros(80,80);
    parallel_for_(Range{ 0, 64 }, RandomFillBody{ img }, 8);
    auto format = QImage::Format_RGB888;
    QPixmap pix = QPixmap::fromImage(QImage((unsigned char*) img.data, img.cols, img.rows, format));

    emit sigSetPixmap(0, pix);
}

void VideoScreen::onTimerUpdate()
{
    Timer t;

    t.start();
    ParallelVideoResizer resizer_obj(&sources, 1024, 768);
    parallel_for_(Range{ 0, 16 }, resizer_obj, 5);
    t.stop();
    prn("ParallelVideoResizer started time %f msecs!", t.time(Timer::MSEC));
    t.clear();

    // output all sorces
    //for(int i=0; i < sources.size(); i++)
    //    sources[i]->show();
    t.start();
    MultiVideoTexture multi_texture_obj(&sources);
    parallel_for_(Range{ 0, 16 }, multi_texture_obj, 5);
    t.stop();
    prn("MultiVideoTexture started %f msecs!",  t.time(Timer::MSEC));
    t.clear();

    // show source ¹ 1
    Source* src1 = sources[0];
    auto format = QImage::Format_RGB888;
    Mat& img = multi_texture_obj.outMat; //src1->frame;
    QImage image = QImage((unsigned char*) img.data, img.cols, img.rows, format);
    QPixmap pix = QPixmap::fromImage(image);
    emit sigSetPixmap(0, pix);
    emit sigSetImage(0, image);
    //QApplication::processEvents();

}

void VideoScreen::test_resize()
{
    Source* src1 = new Source("e:\\VIDEO\\Idiocracy.2006.WEB-DL.1080p.mkv");
    Source* src2 = new Source("e:\\VIDEO\\Le.Dernier.Yoyage.2020.HDRip-AVC.mkv");
    Source* src3 = new Source("e:\\VIDEO\\The.Mutation.2021.WEB-DLRip.avi");
    Source* src4 = new Source("e:\\VIDEO\\Injustice.2021.BDRip.feofanio.avi");

    sources.push_back(src1);
    sources.push_back(src2);
    sources.push_back(src3);
    sources.push_back(src4);

//    for(auto b = sources.begin(); b < sources.end(); b++)
//    {
//      b->show();
//      prn("output source %d %d", b->frame.cols, b->frame.rows);
//    }
//    return;


    QTimer* timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimerUpdate()));
    timer->start(qRound(1000./frame_rate));
}


void test()
{
    LOG() << "IN";

    QThread::sleep( 1 );

    LOG() << "OUT";
}

