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
#include <QMutex>
#include <QTime>
#include <random>
#include <algorithm>
#include "print.h"
#include <opencv2/core/utility.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio.hpp>
#include "opencv2/opencv.hpp"

#define MAX_SOURCES 20
#define CACHE_SIZE 30

class Source
{
public:
    Source(QString filename)
    {
        cap = 0;
        this->filename = filename;
        id = rand();
        frame = cv::Mat3b::zeros(1, 1);
        current_frame = 0;
        for(int i=0; i < CACHE_SIZE + 1; i++)
            frames_cache.push_back(cv::Mat3b::zeros(1, 1));

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
    bool isBlank()
    {
        return  (frame.cols == 1) && (frame.rows == 1);
    }
    int reopen()
    {
        if(cap==0)
        {
            cap = new cv::VideoCapture(filename.toStdString());
            cap->set(cv::CAP_PROP_POS_MSEC, 144 * 1000 ); //10 sec skipped
        }
        return 1;
    }

    // read n consecutive frames
    int precache()
    {
        int n_readed = 0;

        for(int i=0; i < CACHE_SIZE; i++)        
        {
            mutex.lock();
            if(cap->isOpened()){
                *cap >> frames_cache[i];
                n_readed++;
            }
            mutex.unlock();
        }

        return n_readed;
    }
    cv::Mat3b getNextFrame()
    {
        mutex.lock();
        if(cap->isOpened()){
            *cap >> frame;
            //n_readed++;
        }
        mutex.unlock();

        return frame;

        prn("(current_frame %% CACHE_SIZE) = %d", (current_frame % CACHE_SIZE));
        if((current_frame % CACHE_SIZE) == 0)
           precache();

        if(current_frame > CACHE_SIZE)
            frame = frames_cache[current_frame % CACHE_SIZE];
        else
            frame = frames_cache[current_frame];

        current_frame++;
        return frame;
    }

    void setFrame(cv::Mat& fr)
    {
        frame = fr;
    }

    void show()
    {
        // Display the resulting frame
        if( isBlank() ) return;
        QString window_name = "src_" + QString::number(id);
        imshow(window_name.toStdString(), frame );
    }

    QString      filename;
    QMutex       mutex;
    cv::VideoCapture* cap;
    std::vector<cv::Mat3b>  frames_cache;
    cv::Mat3b frame;
    int current_frame;
    int id;
};

class SourcesList : public std::vector<Source*>
{
public:
    SourcesList(){}
    SourcesList(std::vector<Source*> list):
      std::vector<Source*>(list)
    {

    }
    void shuffle()
    {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(begin(), end(), g);
    }

};

class ParallelVideoResizer : public cv::ParallelLoopBody
{
    SourcesList sorces_list;
    cv::Size output_size;

public:
    ParallelVideoResizer(){}
    ParallelVideoResizer(std::vector<Source*>* src_list, int output_width, int output_height)
    {
        sorces_list = *src_list;
        sorces_list.shuffle();
        output_size = cv::Size(output_width, output_height);
    }


    void operator()(const cv::Range& range) const override
    {
        int devisor = 4;
        cv::Size size = cv::Size(output_size.width/devisor, output_size.height/devisor);
        prn("range %d %d ", range.start, range.end);
        if( (range.start/2) < sorces_list.size())
        {
            Source* src1 = sorces_list[range.start/2];
            src1->reopen();
            cv::Mat3b src = src1->getNextFrame();
            cv::Mat3b temp;
            resize(src, temp, size);
            src1->setFrame(temp);
        }
    }
};


class MultiVideoTexture : public cv::ParallelLoopBody
{
    SourcesList sorces_list;
public:
    cv::Mat3b outMat;
    MultiVideoTexture(){}
    MultiVideoTexture(std::vector<Source*>* src_list)
    {
        sorces_list = *src_list;
        outMat = cv::Mat3b::zeros(1024, 768);
    }

    void operator()(const cv::Range& range) const override
    {
        cv::Vec3b color_red{ 255, 0, 0 };
        //Vec3b color_blue{ 0, 0, 255 };

        int index = range.start/2;
        if(index >= sorces_list.size())
            return;
        Source* inp1 = sorces_list[index];
        cv::Mat3b& src = inp1->frame;

        cv::Mat3b temp;
        int devisor = 2;
        cv::Size size = cv::Size(outMat.cols/devisor, outMat.rows/devisor);
        resize(src, temp, size);

        int w = size.width;
        int h = size.height;
        int y = 0; int x = 0;
        //if( index == 0 )
        if( index == 0 )
        {
            cv::Mat subImg = outMat(cv::Range{ 0, h }, cv::Range{ 0, w });
            temp.copyTo(subImg);
        }
        if( index == 1 )
        {
            cv::Mat subImg = outMat(cv::Range{ y, y + h }, cv::Range{ x + w, x + 2 * w });
            temp.copyTo(subImg);
        }
        if( index == 2 )
        {
            cv::Mat subImg = outMat(cv::Range{ y + h, y + 2 * h }, cv::Range{ x, x + w });
            temp.copyTo(subImg);
        }
        if( index == 3 )
        {
            cv::Mat subImg = outMat(cv::Range{ y + h, y + 2 * h }, cv::Range{ x + w, x + 2 * w });
            temp.copyTo(subImg);
        }

    }
};


class VideoScreen : public QObject
{
    Q_OBJECT
public:
    std::vector<Source*> sources;
    double frame_rate;
    VideoScreen();
    void openSources(QStringList filenames);
    void test1();
    void test_resize();
signals:
    void sigSetPixmap(int screen_number, QPixmap& pix);
    void sigSetImage(int screen_number, QImage& image);

public slots:
    void onTimerUpdate();

};

#endif // VIDEOSCREEN_H
