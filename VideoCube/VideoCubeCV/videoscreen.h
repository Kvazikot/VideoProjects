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

#include <QStringList>
#include <opencv2/core/utility.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#define MAX_SOURCES 20

using namespace cv;


class Source
{
public:
    Source();
    ~Source();
    int Open();
    cv::Mat getNextFrame();

    QString      filename;
    cv::VideoCapture* cap;
    int current_frame;
};

class VideoScreen
{
public:
    Source frames_n[MAX_SOURCES];

    VideoScreen();
    void openSources(QStringList filenames);

};

#endif // VIDEOSCREEN_H
