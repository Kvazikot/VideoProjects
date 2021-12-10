/**********************************************************************
 *  Project App6
 *
 *  cmaininterface.cpp
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

#include <QString>
#include <QRegExp>
#include "../include/print.h"
#include "../include/cinput.h"
#include "../include/cmaininterface.h"
#include "../include/objectinspector.h"


CMainInterface CMainInterface::instance;

CMainInterface::CMainInterface() :
    QObject(0)
{
}

CMainInterface& CMainInterface::getInstance()
{
    return instance;
}


void CMainInterface::SwitchMode(int n)
{
    mode = n;
    switch(n)
    {
    case MODE_SELECTION:
        isInSelectionMode = 1;
        isInGameMode = 0;
        isInMove = 0;
        isInPickPntMode = 0;
        isInScaleMode = 0;
        isInRotateMode = 0;

        break;
    case MODE_MOVE:
        isInSelectionMode = 0;
        isInGameMode = 0;
        isInMove = 1;
        isInPickPntMode = 0;
        isInScaleMode = 0;
        isInRotateMode = 0;

        break;
    case MODE_GAME:
        isInSelectionMode = 0;
        isInGameMode = 1;
        isInMove = 0;
        isInPickPntMode = 0;
        isInScaleMode = 0;
        isInRotateMode = 0;

        break;
    case MODE_PICK:
        isInSelectionMode = 0;
        isInGameMode = 1;
        isInMove = 0;
        isInPickPntMode = 1;
        isInScaleMode = 0;
        isInRotateMode = 0;
        break;
    case MODE_ROTATE:
        isInSelectionMode = 0;
        isInGameMode = 0;
        isInMove = 0;
        isInPickPntMode = 0;
        isInRotateMode = 1;
        isInScaleMode = 0;
        break;
    case MODE_SCALE:
        isInSelectionMode = 0;
        isInGameMode = 0;
        isInMove = 0;
        isInPickPntMode = 0;
        isInRotateMode = 0;
        isInScaleMode = 1;
        break;




    }
}


void CMainInterface::MousePick()
{
    ObjectInspector* oi = ObjectInspector::getInstance3();
    CInput& iinput = CInput::getInstance2();
    //oi->obj
    if( oi->tablew->currentItem()!=0 )
    {
        int row = oi->tablew->currentItem()->row();
        QString key = oi->tablew->item(row,0)->text();
        QString value = oi->tablew->item(row,1)->text();
        //QRegExp re("p[0-9.]+,p[0-9.]+,p[0-9.]+");
        t_atr* a = oi->obj->GET_ATTR(key.toUtf8().data());
        if(a!=0)
        {
            if(a->type == "Pnt")
            {
                QStringList parts = value.split(",");
                CInput* iinput = CInput::getInstance();
                Pnt p(iinput->vx, parts[1].toFloat(), iinput->vz);
                oi->tablew->item(row,1)->setText(QString(p.toStr().c_str()));
                oi->tablew->update();
                oi->obj->OnAttrChange(*a);
            }
        }
    }
    prn("pick %f %f", iinput.vx, iinput.vz);
/*
    if( !pdlg->m_ObjInspector.isObjDead() )
    {
        Object* o = pdlg->m_ObjInspector.obj;
        int idx = pdlg->m_ObjInspector.selidx;
        if (pdlg->m_ObjInspector.obj->classname == "Curve" )
        {
            Curve* curve = (Curve*)pdlg->m_ObjInspector.obj;
            string s = pdlg->m_ObjInspector.GetText(1,idx);
            string y_s = StrSplitE(s,",",0)[1];
            float scale = curve->scale;
            Pnt p(iinput.vx/scale, StrToDouble(y_s), iinput.vz/scale);
            pdlg->m_ObjInspector.ApplyText(1,idx,(char*)p.toStr().c_str());
            curve->update_timer = gtimer.fsec + 3;
        }
    }
*/
}

