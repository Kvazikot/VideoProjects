/**********************************************************************
 *  Project App6
 *
 *  cinput.cpp
 *
 *  Author: Vladimir Baranov
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
**********************************************************************/
#if _WIN32
    #include <windows.h>
    #include <process.h>
#endif

#include "../include/timer.h"
#include "../include/cinput.h"
#include "../include/param_screen_bcb.h"
#include "../include/print.h"
#include "../include/cobjregistry.h"
#include "../include/osd.h"

static CObjRegistry& MREG = CObjRegistry::getInstance2();
static ParamScreen& pscr = ParamScreen::getInstance();
static COSD& osd = COSD::getInstance();

#if _WIN32
    unsigned __stdcall TF_PLAY(void* arg);
#else
    unsigned TF_PLAY(void* arg);
#endif

CInput CInput::instance;

CInput* CInput::getInstance()
{
    return &instance;
}

CInput& CInput::getInstance2()
{
    return instance;
}

CInput::CInput() : QObject(0)
{
    m_button = -1;
    bNeedBreak = 0;
    bRecordEvents = false;
    bPlayEvents = false;
    #if _WIN32
    HANDLE timer = CreateWaitableTimer(0,false,0);
    LARGE_INTEGER li;
    const int unitsPerSecond=1*1000*1000; // 100 nano seconds
    li.QuadPart=-(2*unitsPerSecond);
    SetWaitableTimer(timer,&li,1,0,0,false);
    _beginthreadex(0,0,TF_PLAY,(void*) timer,0,0);
    #endif

}


/*CInput::CInput(QObject *parent) :
    QObject(parent)
{
    m_button = 0;
}
*/

void CInput::start_record()
{
    if( bRecordEvents == false)
    {
        tick = 0;
        events.clear();
        obj_states.clear();
        MREG.SaveObjsStates(obj_states);
    }
    bRecordEvents = !bRecordEvents;
    //pscr.AddVar("Recording");
    //pscr.seti("Recording",bRecordEvents);
    if(bRecordEvents == true)
        osd.SetTopText("Recording",400);
    else
        osd.SetTopText("Recording stop",400);

}


void CInput::start_play()
{
    if(bPlayEvents == false)
    {
        if( bRecordEvents == true)
            bRecordEvents = false;
        MREG.LoadObjsStates(obj_states);
        tick = 0;
        osd.SetTopText("Playing",400);
    }
    else
        osd.SetTopText("Playing stop",400);
    bPlayEvents = !bPlayEvents;
}


void CInput::click_lmb(int state)
{
    if(state)
      CLEAR_BIT(m_button, 1);
    else
      SET_BIT(m_button, 1);

    //mu_printf("lmb state %d m_button=%d", state, m_button);
    for(vector<Object*>::iterator i = delegates.begin(); i!=delegates.end(); i++)
    {
        (*i)->click_lmb(state);
        if( bNeedBreak )
            break;
    }
    bNeedBreak = false;
    if( bRecordEvents )
    {
        InputEvent p;
        if(state)
            p.type = InputEvent::MOUSE_L_DOWN;
        else
            p.type = InputEvent::MOUSE_L_UP;
        p.time = tick;
        events.push_back(p);
    }
}

void CInput::click_mmb(int state)
{
    if(state)
        CLEAR_BIT(m_button, 2);
    else
    SET_BIT(m_button, 2);
    //mu_printf("mmb state %d m_button=%d", state, m_button);
    for(vector<Object*>::iterator i = delegates.begin(); i!=delegates.end(); i++)
    {
        (*i)->click_mmb(state);
        if( bNeedBreak )
            break;
    }
    bNeedBreak = false;

}

bool CInput::check_lmb()
{
    return !CHECK_BIT(m_button, 1);
}

void CInput::click_rmb(int state)
{
    if(state)
        CLEAR_BIT(m_button, 3);
    else
    SET_BIT(m_button, 3);

    //mu_printf("rmb state %d m_button=%d", state, m_button);
    for(vector<Object*>::iterator i = delegates.begin(); i!=delegates.end(); i++)
    {
        (*i)->click_rmb(state);
        if( bNeedBreak )      break;
    }
    bNeedBreak = false;
    if( bRecordEvents )
    {
        InputEvent p;
        if(state)
            p.type = InputEvent::MOUSE_R_DOWN;
        else
            p.type = InputEvent::MOUSE_R_UP;
        p.time = tick;
        events.push_back(p);
    }

}

void CInput::key_upressed(int k)
{
    keystates[k] = false;
    bKeyPressed = false;
    //mod = glutGetModifiers();
    for(vector<Object*>::iterator i = delegates.begin(); i!=delegates.end(); i++)
    {
        (*i)->key_upressed(k);
    }
#ifdef _WIN32
    if( bRecordEvents && k != VK_F5 && k != VK_F6 )
    {
        InputEvent p;
        p.type = InputEvent::KEY_UP;
        p.key = k;
        p.time = tick;
        events.push_back(p);
    }
#endif
}

void CInput::key_pressed(int k)
{
    prn("%x key",k);
    key = k;
    keystates[k] = true;
    //mod = glutGetModifiers();
    bKeyPressed = true;
    for(vector<Object*>::iterator i = delegates.begin(); i!=delegates.end(); i++)
    {
        (*i)->key_pressed(k);
    }
#ifdef _WIN32
    if( bRecordEvents && k != VK_F5 && k != VK_F6 )
    {
        InputEvent p;
        p.type = InputEvent::KEY_DOWN;
        p.key = k;
        p.time = tick;
        events.push_back(p);
    }
    if(k==VK_F5) start_play();
    if(k==VK_F6) start_record();
#endif
}

void CInput::mouse_move(int x, int y)
{
     mouse_x = x;
     mouse_y = y;
     delta.x = p0.x - vx;
     delta.y = p0.y - vy;
     delta.z = p0.z - vz;

       //if( snap2grid )
     Snap();

       //mu_printf("%d %d",x,y);
       for(vector<Object*>::iterator i = delegates.begin(); i!=delegates.end(); i++)
       {
            (*i)->mouse_move(x, y);
       }

    mouse_x = x;
    mouse_y = y;
    for(vector<Object*>::iterator i = delegates.begin(); i!=delegates.end(); i++)
    {
        (*i)->mouse_move(x, y);
    }
    if( bRecordEvents )
    {
        InputEvent p;
        p.type = InputEvent::MOUSE_MOVE;
        p.mouse_x = x;
        p.mouse_y = y;
        p.time = tick;
        events.push_back(p);
    }
}


void CInput::Snap()
{
/*
    return;
    static GLint    viewport[4];
    static GLdouble projection[16];
    static GLdouble modelview[16];
    static GLdouble vx,vy,vz;
    static GLdouble wx,wy,wz;
    glGetIntegerv( GL_VIEWPORT, viewport );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );

    float x, x1, x2, xm;
    float y, y1, y2, ym;

    //	ближайшая т. сетки для единичного шага
    if( ( x1 - ceil(x1) ) < ( x1 - floor(x1)) )
        x =	x1;
    else
        x = x2;

    //
    if( ( y1 - ceil(y1) ) < ( y1 - floor(y1)) )
        y =	y1;
    else
        y = y2;

    // получим коорд во вьюпорте
    if( gluProject(x,0,y,modelview, projection, viewport, &wx, &wy, &wz) == GL_FALSE )
    {
        prn("gluProject failed");
    }
    else
    {
        prn("snap коорд во вьюпорте %f %f", wx, wy);
    }
*/


}

void CInput::mouse_wheel(int Zdelta)
{
    for(vector<Object*>::iterator i = delegates.begin(); i!=delegates.end(); i++)
    {
        (*i)->mouse_wheel(Zdelta);
    }
    if( bRecordEvents )
    {
        InputEvent p;
        p.type = InputEvent::MOUSE_WHEEL;
        p.zDelta = Zdelta;
        p.time = tick;
        events.push_back(p);
    }
}

//таймер для проигрывания
#ifdef _WIN32
unsigned __stdcall TF_PLAY(void* arg) {
  HANDLE timer=(HANDLE) arg;
  while (1) {
    WaitForSingleObject(timer,INFINITE);
    CInput* iinput = CInput::getInstance();
    iinput->play_events();
    iinput->tick++;
  }
}
#else
  unsigned TF_PLAY(void* arg)
  {

  }
#endif

void CInput::play_events()
{
    if( !bPlayEvents ) return;
    CInput* iinput = CInput::getInstance();

    double t = tick;

    for(int i=0; i<(int)events.size(); i++)
    {
        InputEvent event = events[i];
        if(abs((long) (event.time - t) ) < 1 )
        {
            switch( event.type )
            {
                case InputEvent::KEY_DOWN:
                  iinput->key_pressed(event.key);
                break;
                case InputEvent::KEY_UP:
                  iinput->key_upressed(event.key);
                break;
                case InputEvent::MOUSE_MOVE:
                  iinput->mouse_move(event.mouse_x, event.mouse_y);
                break;
                case InputEvent::MOUSE_L_DOWN:
                  iinput->click_lmb(1);
                break;
                case InputEvent::MOUSE_L_UP:
                  iinput->click_lmb(0);
                break;
                case InputEvent::MOUSE_R_DOWN:
                  iinput->click_rmb(1);
                break;
                case InputEvent::MOUSE_R_UP:
                  iinput->click_rmb(0);
                break;
                case InputEvent::MOUSE_WHEEL:
                    iinput->mouse_wheel(event.zDelta);
                break;


            }
        }

    }

    double tPlayStop;

    if(events.size() > 1)
    {
       InputEvent mk = events[events.size()-1];
       tPlayStop = mk.time;
    }

    if(t > tPlayStop)
    {
        bPlayEvents = false;
    }


}


void CInput::add_delegat(Object* obj)
{
    delegates.push_back(obj);
}

bool CInput::shift_on()
{
    return true;
}
