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

#ifndef VIDEOSCREEN_H
#define VIDEOSCREEN_H

#include <QObject>
#include <QStringList>
#include <QTime>
#include "print.h"
#include <opencv2/core/utility.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio.hpp>
#include "opencv2/opencv.hpp"

#define MAX_SOURCES 20

using namespace cv;

class Source
{
public:
    Source()
    {
        cap = 0;
        id = rand();
        frame = Mat3b::zeros(70, 70);
    }
    ~Source()
    {
       if(cap == 0) return;
       cap->release();
    }
    int Open(QString filename)
    {
        cap = 0;
        this->filename = filename;
        return 1;
    }
    int reopen()
    {
        if(cap==0) cap = new cv::VideoCapture(filename.toStdString());
        return 1;
    }
    cv::Mat getNextFrame()
    {
        reopen();
        if(cap->isOpened()){
           // Capture frame-by-frame
           cap->set(cv::CAP_PROP_POS_MSEC, 144 * 1000 ); //10 sec skipped
           *cap >> frame;
           //prn("frame readed %d %d", frame.cols, frame.rows);
        }
        return frame;
    }

    void setFrame(Mat& fr)
    {
        frame = fr;
    }

    void show()
    {
        // Display the resulting frame
        QString window_name = "src_" + QString::number(id);
        imshow(window_name.toStdString(), frame );
    }

    QString      filename;
    cv::VideoCapture* cap;
    Mat frame;
    int current_frame;
    int id;
};

class ParallelVideoResizer : public ParallelLoopBody
{
    std::vector<Source>* Sorces_list;
    Size output_size;

public:
    ParallelVideoResizer(){}
    ParallelVideoResizer(std::vector<Source>* sorces_list, int output_width, int output_height)
    {
        Sorces_list = sorces_list;
        output_size = Size(output_width, output_height);
    }

    void operator()(const Range& range) const override
    {
        int devisor = Sorces_list->size()/2;
        Size size = Size(output_size.width/devisor, output_size.height/devisor);
        if( (range.start/2) < Sorces_list->size())
        {
            auto src1 = &(*Sorces_list)[range.start/2];
            src1->reopen();
            src1->getNextFrame();
            Mat& src = src1->frame;
            Mat temp;
            resize(src, temp, size);
            src1->setFrame(temp);
        }
    }
};

/*
class MultiVideoTexture : public ParallelLoopBody
{
    std::vector<Source> Sorces_list;
    Mat3b outMat;
public:
    MultiVideoTexture(){}
    MultiVideoTexture(std::vector<Source>& sorces_list)
    {
        Sorces_list = sorces_list;
    }

    void operator()(const Range& range) const override
    {
            out.
            for(int i=0; i < Sorces_list.size(); i++)
            {
                auto inp1 = Sorces_list[i];
                Mat& src = inp1.frame;
                int w = src.cols;
                int h = src.rows;
                int y = 0; int x = 0;
                Vec3b color_red{ 255, 0, 0 };
                Vec3b color_blue{ 0, 0, 255 };
                outMat(Range{ y, y + h }, Range{ x, x + w }) = color_red;
                outMat(Range{ y, y + h }, Range{ x, x + w }) = color_red;
            }

    }
};
*/

class VideoScreen : public QObject
{
    Q_OBJECT
public:
    Source frames_n[MAX_SOURCES];

    VideoScreen();
    void openSources(QStringList filenames);
    void test1();
    void test_resize();
signals:
    void sigSetPixmap(int screen_number, QPixmap& pix);

};

#endif // VIDEOSCREEN_H
