#ifndef CINPUT_H
#define CINPUT_H

#include <QObject>
#include "geometry.h"
#include "object.h"

//====================================================================
// ¬вод пользовател€
//====================================================================

struct InputEvent
{
    enum {KEY_UP, KEY_DOWN, MOUSE_L_DOWN, MOUSE_R_DOWN, MOUSE_L_UP, MOUSE_R_UP, MOUSE_WHEEL, MOUSE_MOVE} type;
    long time;
    int m_button;
    int key;
    int mouse_x;
    int mouse_y;
    int zDelta;
};

class CInput : public QObject
{
 //   Q_OBJECT
public:
    CInput();
    vector<InputEvent> events;
    //explicit CInput(QObject *parent = 0);

private:
    Pnt p0;			// коорд мыши во вьюпорте при клике
    unsigned int prev_p;
    double LX,LY,RX,RY,TX,TY;
    bool bNewDelegat;
    vector<string> obj_states;
public:
    int m_button;
    int key;
    long   tick;
    int mouse_x;
    int mouse_y;
    bool bKeyPressed; //нажата клавиша
    bool bRecordEvents;
    bool bPlayEvents;
    Pnt delta;	    // дельта счисл€етс€ в mouse move
    double vx,vy,vz; // коорд мыши во вьюпорте
    double wx,wy,wz;
    bool keystates[256];
    int mod;
    bool snap2grid;	//режим прилипани€ к сетке
    bool bNeedBreak;  // создан новый обьект делегат

    void Snap();
    void mouse_move(int x, int y);
    void start_record();
    void start_play();
    void play_events();
    void click_lmb(int state);
    void click_rmb(int state);
    void click_mmb(int state);
    bool check_lmb();
    void key_pressed(int k);
    void key_upressed(int k);
    void mouse_wheel(int zDelta);
    void add_delegat(Object* obj);
    bool shift_on();
    bool ctrl_on();
    bool alt_on();

    static CInput  instance;
    static CInput* getInstance();
    static CInput& getInstance2();

    vector<Object*> delegates;



signals:

public slots:

};

#endif // CINPUT_H

