#include <QElapsedTimer>
#include "../include/timer.h"


Timer Timer::gtimer;

Timer::Timer()
{
    _timer.start();
}

