#ifndef PLAYER_H
#define PLAYER_H

#include "object.h"
#include <QElapsedTimer>

struct ControlShema
{
    unsigned char ForwardKey;
    unsigned char BackwardKey;
    unsigned char LeftKey;
    unsigned char RightKey;
};

class Player : public Object
{
public:
    ControlShema control;
    double  speed;
    double  cam_dist;
    double step, dt,_t2,_t1,_freq;
    QElapsedTimer timer;
    Pnt pos;
    Pnt rot;
    void SetControlShema(unsigned char f, unsigned char b,
                                 unsigned char r, unsigned char l);
    void SetFPCamera();
    void ProcessInput();
    void Slot1();

    Player();
};

#endif // PLAYER_H
