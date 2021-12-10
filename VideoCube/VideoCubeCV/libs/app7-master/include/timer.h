#ifndef TIMER_H
#define TIMER_H

#include "object.h"
#include <QElapsedTimer>
//====================================================================
//                  ¬–≈ћя
//====================================================================
class Timer : public Object
{
    public:
    static Timer gtimer;
    Timer();
    void Reset();
    void Timer1();
    void Timer2();
    void Timer3();

    long msec;
    long sec;		//секунды от старта движка
    long frames;	// кадры от старта движка
    double fsec;

    QElapsedTimer _timer;
    qint64 _interval;
    qint64 _reference;

};

#endif
