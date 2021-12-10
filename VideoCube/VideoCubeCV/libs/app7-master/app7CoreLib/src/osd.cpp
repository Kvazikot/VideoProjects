/**********************************************************************
 *  Project App6
 *
 *  osd.cpp
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

#include "../include/osd.h"
#include "../include/param_screen_bcb.h"
#include "../include/glcode.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

static ParamScreen& pscr = ParamScreen::getInstance();

COSD COSD::instance;

COSD& COSD::getInstance()
{
    return instance;
}

COSD::COSD()
{
	Nstr = 1;
    //isCanRender = true;
}

void COSD::Test()
{
	//SetTopText("one", 150);
	//SetTopText("two", 150);
}


// на первой же строке сразу же
void COSD::SetTopText(string s, int timeout)
{
	conf.fade_state = 0;
	conf.alpha = 0.0;
	if(strings.size()>0)
	{
	  strings.pop_back();
	  //if(conf.fade_state == 0)
	  //{
		conf.fade_state = 1;
		conf.alpha = 1.0;
	  //}
	}
	strings.push_back(s);	
	conf.cnt = timeout;
	conf.timeout = timeout;
	conf.astep = (float)1.0 * 6.1 /timeout;
	
	//mu_printf(strings.back().c_str());
}

 // постановка в очередь
void COSD::SetText(string s, int timeout)
{

}


void COSD::Render(int RES_X, int RES_Y)
{
    glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

    gluOrtho2D(0.0, RES_X, 0.0, RES_Y);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();


	for(int i=0; i<pscr.MaxRows; i++)
	{		
        glutPrint2d(10.5,RES_Y - i*20-20,GLUT_BITMAP_9_BY_15,(char*)pscr.paramStr[i].c_str(),1,0,0,1);
	}
	
	if(strings.size()>0)
        glutPrint2d(10.5,RES_Y - pscr.MaxRows*20-20,GLUT_BITMAP_9_BY_15,(char*)strings[0].c_str(),1,1,1,conf.alpha);
	
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void COSD::Slot1() //timer slot
{
	{
		conf.cnt--;
	   //печать переменных
		pscr.PrintMap();

		if(strings.size()==0)
			return;

		if((conf.timeout - conf.cnt) > (conf.timeout / 6) )
		{
		  conf.fade_state=1;
		}


		if((conf.timeout - conf.cnt) > (conf.timeout - conf.timeout / 6) )
		{
		  conf.fade_state=2;
		}

		//появляется
		if(conf.fade_state==0)
		  conf.alpha+=conf.astep;
		//исчезает
		if(conf.fade_state==2)
		  conf.alpha-=conf.astep;
		//держится
		if(conf.fade_state==1)
		  conf.alpha = 1.0;

		//mu_printf("a=%f", conf.alpha);
			
		if(conf.cnt <=0)
  		 strings.pop_back();
	}

	
	

}
