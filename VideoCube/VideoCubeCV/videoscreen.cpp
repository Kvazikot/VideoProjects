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
#include <QFuture>
#include <QThread>
#include <print.h>
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

void VideoScreen::test_resize()
{
    Source src1, src2, src3, src4;
    src1.Open("e:\\VIDEO\\Idiocracy.2006.WEB-DL.1080p.mkv");
    src2.Open("e:\\VIDEO\\Le.Dernier.Yoyage.2020.HDRip-AVC.mkv");
    src3.Open("e:\\VIDEO\\The.Mutation.2021.WEB-DLRip.avi");
    src4.Open("e:\\VIDEO\\Injustice.2021.BDRip.feofanio.avi");

    src1.getNextFrame();
    src2.getNextFrame();
    src3.getNextFrame();
    src4.getNextFrame();
    std::vector<Source> sources;
    sources.push_back(src1);
    sources.push_back(src2);
    sources.push_back(src3);
    sources.push_back(src4);

//    for(auto b = sources.begin(); b < sources.end(); b++)
//    {
//      b->show();
//      prn("output source %d %d", b->frame.cols, b->frame.rows);
//    }

    ParallelVideoResizer resizer_obj(&sources, 1024, 768);
    parallel_for_(Range{ 0, 16 }, resizer_obj, 9);

    //auto format = QImage::Format_RGB888;
    //Mat& img = *resizer_obj.outMat;
    //QPixmap pix = QPixmap::fromImage(QImage((unsigned char*) img.data, img.cols, img.rows, format));
    //emit sigSetPixmap(0, pix);

    for(auto b = sources.begin(); b < sources.end(); b++)
    {
      b->show();
      prn("output source %d %d", b->frame.cols, b->frame.rows);
    }


}


void test()
{
    LOG() << "IN";

    QThread::sleep( 1 );

    LOG() << "OUT";
}

