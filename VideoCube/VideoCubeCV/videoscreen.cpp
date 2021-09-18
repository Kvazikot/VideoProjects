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
{

    int n = cv::getNumberOfCPUs();
    //prn("getNumberOfCPUs() = %d", n);
    int n1 = cv::getNumThreads();
    //prn("getNumThreads() = %d", n1);
    Mat3b img = Mat3b::zeros(80,80);
    //parallel_for_(Range{ 0, 64 }, RandomFillBody{ img }, 8);

}


Source::Source()
{


}

int Source::Open()
{
    return 0;
}

Source::~Source()
{

}

void test()
{
    LOG() << "IN";

    QThread::sleep( 1 );

    LOG() << "OUT";
}


int main1()
{
    LOG() << "Calling test()...";

    QThreadPool::globalInstance()->setMaxThreadCount(THREADS_NUMBER);

    // open sources

    //QFuture<QImage> frames;
    auto f = QtConcurrent::run( test );

    LOG() << "Running test()...";

    // ...                             ...
    // ... You can do other stuff here ...
    // ...                             ...

    f.waitForFinished(); // Blocking call to wait for function to finish

    LOG() << "Exiting...";

    return 0;
}
