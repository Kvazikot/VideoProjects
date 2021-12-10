/**********************************************************************
 *  Project App6
 *
 *  ball.cpp
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

//REALISATION CPP
#include "../include/ball.h"
#include "../include/move.h"
#include "../include/glcode.h"
#include "../include/timer.h"

//====================================================================
// <OBJECT> Ўј– \ физическа€ модель м€ча  </OBJECT>
//====================================================================

Ball::Ball()
{
	name = "ball01";
	classname = "Ball";
	isCanRender = true;
	isEntety = true;
	RGB2GL2(RGB(255,0,0),color);
	// запрос на апдейт
	C0 = Pnt(0,0,0);
    gentime = Timer::gtimer.msec;
	R = 5;
	Nseg = 20;
	Npts = 20;
	Nparal = 20;
	phi1 = 0;
	algo = "type1 triangles geosphere";
	algon = 0;
	rot = Pnt(0,0,0);
	scale = Pnt(1,1,1);


	bounds.x = R*scale.x;
	bounds.y = R*scale.y;
	bounds.z = R*scale.z;
	mesh = new GemObject();		
	Generate();
	bNeedGenerate = false;
	mesh->isCanRender = true;	
}


Ball::Ball(string _name, Pnt _C0, float _R,
           float r, float g, float b)
    :R(_R),Nseg(20),Nparal(20),Npts(20),RenderObject()
{

    name = _name;

    classname = "Ball";
    isCanRender = true;
    isEntety = true;
    RGB2GL2(RGB(r,g,b),color);
    C0 = _C0;
    // запрос на апдейт
    gentime = Timer::gtimer.msec;
    phi1 = 0;
    algo = "type1 triangles geosphere";
    algon = 0;
    rot = Pnt(0,0,0);
    scale = Pnt(1,1,1);
    bounds.x = R*scale.x;
    bounds.y = R*scale.y;
    bounds.z = R*scale.z;
    mesh = new GemObject();
    Generate();
    bNeedGenerate = false;
    mesh->isCanRender = true;
}

void Ball::Generate()
{
	//mu_printf("Ball::Generate()");	
	float x,y,z,a=0,b=0;

	if(algon==0)
	{
		mesh->SetNumVert(Npts*Nparal+10);
		
		//параллели
		for(int j=0; j<Nparal; j++)
		{
			b+=(180./(float)Nparal) * GR;
			a=0;
			for(int i=j*Npts; i<j*Npts+Npts; i++)
			{
				a+=(360./(float)Npts) * GR;
				x = R * sin(b)*cos(a);
				z = R * sin(b)*sin(a);
				y = R * cos(b);
				mesh->vertices[3*i]=x;
				mesh->vertices[3*i+1]=y;
				mesh->vertices[3*i+2]=z;
			}
		}
	}

	
}

void Ball::Scale(float,float,float)
{

}

void Ball::Rotate(float,float,float)
{
	
}


void Ball::Render()
{
    Pnt p1[9];
	//glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	//glLoadIdentity();
	
	if(!isCanRender)
		return;
    //glTranslatef ( 0, 0, 0 );
    glTranslatef ( ei.coord.x, ei.coord.y, ei.coord.z );
	
	//glRotatef(angle.x, angle.y, angle.z,1);

		
	static float cl=0;		
	if(isSelected!=2)
	{
		glEnable(GL_COLOR_MATERIAL);	
		glColor4f(0.5,0.5,0.5,1);
		//glPointSize(3);
		
		if(rot.x!=0)
			glRotatef(rot.x, 1, 0, 0);
		if(rot.y!=0)
			glRotatef(rot.y, 0, 1, 0);
		if(rot.z!=0)
			glRotatef(rot.z, 0, 0, 1);
		
		glScalef(scale.x, scale.y, scale.z);



	
        //if(Timer::gtimer.msec%10 == 0)
		//	 cl=cl+0.1;
		//if(cl>1.0) cl=0;

	}
	else
	{
		
		glPolygonMode( GL_FRONT_AND_BACK, GL_QUADS );
		glMaterialfv ( GL_FRONT_AND_BACK, GL_DIFFUSE, color );
	
		
		if(rot.x!=0)
			glRotatef(rot.x, 1, 0, 0);
		if(rot.y!=0)
			glRotatef(rot.y, 0, 1, 0);
		if(rot.z!=0)
			glRotatef(rot.z, 0, 0, 1);
		
		glScalef(scale.x, scale.y, scale.z);

	}

	if(algon==0)
	{
		//if(flags.bWire)
		{
			RGB2GL2(RGB(255,0,0),color);
			glMaterialfv ( GL_FRONT_AND_BACK, GL_DIFFUSE, color );
			for(int j=0; j<Nparal; j++)
			{
				glBegin(GL_LINE_LOOP);
				for(int i=j*Npts; i<j*Npts+Npts; i++)
					glVertex3f(mesh->vertices[3*i],mesh->vertices[3*i+1],mesh->vertices[3*i+2]);
				glEnd();
			}
			
			RGB2GL2(RGB(0,0,255),color);		
			glMaterialfv ( GL_FRONT_AND_BACK, GL_DIFFUSE, color );
			for(int i=0; i<Npts; i++)
			{
				glBegin(GL_LINE_STRIP);		
				for(int j=0; j<Nparal-1; j++)
				{
					glVertex3f(mesh->vertices[3*(j*Npts+i)],mesh->vertices[3*(j*Npts+i)+1],mesh->vertices[3*(j*Npts+i)+2]);
				}
				glEnd();
			}
		}
		

	}
	if(isSelected!=2)
	{
		//glPointSize(1);
		glDisable(GL_COLOR_MATERIAL);
	}
	glPopMatrix();
		



}


//object inspector event
void Ball::OnAttrChange(t_atr& a)
{
	if( (a.name == "Nseg") || (a.name == "R")  || (a.name == "Npts") || (a.name == "Nparal"))
	{
        gentime = Timer::gtimer.msec;
		bNeedGenerate = true;
		isCanRender = false;
		Npts = Nseg;
		Nparal = Nseg;
	}

	if (a.name == "algo" )
	{
		algon = a.data;
		bNeedGenerate = true;
	}



	

}

//obj update slot
void Ball::Slot1()
{
	//если обновились параметры
    if( bNeedGenerate && ( (Timer::gtimer.msec - gentime)> 1000 ) )
	{
		mesh->isCanRender = false;
		Generate();
		mesh->isCanRender = true;
		isCanRender = true;
		bNeedGenerate = false;
		
	}

	
	Generate();
	if(m_Updated)
	{
		bounds.x = R*scale.x;
		bounds.y = R*scale.y;
		bounds.z = R*scale.z;
		bNeedGenerate = true;
		//Generate();
		m_Updated = false;
		//bNeedGenerate = false;
		//mu_printf("%s accept new params %s", name.c_str(), bounds.toStr().c_str());	
	}

	w = v.x / R;
	f = w / (2 * M_PI);

	phi1 = phi1 + 360 * 0.05 * f;  //50ms * частота 
	rot = Pnt(0, 0, phi1);


	if ( phi1 > 360 ) 
		phi1 = phi1 - 360;

	if ( phi1 < 0 ) 
		phi1 = phi1 + 360;

    /*
	mov->CalcTrenie(this);
	mov->CalcGravity(this);
	mov->SchislenieCoord(this);
    */


}
