/**********************************************************************
 *  Project App6
 *
 *  kirpich.cpp
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
#include "../include/kirpich.h"
#include "../include/glcode.h"

//IMPLEMENT_SERIAL( Kirpich, CObject, 1 )


//====================================================================
// <OBJECT> ОБЫЧНЫЙ КИ� ПИЧ  </OBJECT>
//====================================================================

Kirpich::Kirpich()

{
	name = "box01";
	classname = "Kirpich";
	isCanRender = true;
	isEntety = true;
	RGB2GL2(RGB(255,0,0),color);
	// запрос на апдейт
	C0 = Pnt(0,0,0);
	a = 5; b = 5; c = 5;
	angle = Pnt(0,0,0);
	scale = Pnt(1,1,1);
	bounds.x = a*scale.x;
	bounds.y = b*scale.y;
	bounds.z = c*scale.z;
	Connect();
}

Kirpich::Kirpich(string _name, float a, float b, float c)

{
    Kirpich();
    name = _name;
    this->a = a;
    this->b = b;
    this->c = c;
    bounds.x = a*scale.x;
    bounds.y = b*scale.y;
    bounds.z = c*scale.z;
}


void Kirpich::Serialize(  )
{
/*   Object::Serialize( archive );

   if (archive.IsStoring())
    {
                archive.Write(&ei.coord, sizeof(ei.coord));
                archive.Write(&a, sizeof(a));
                archive.Write(&b, sizeof(b));
                archive.Write(&c, sizeof(c));
                archive.Write(&color, sizeof(color));
                archive.Write(&angle, sizeof(angle));
                archive.Write(&scale, sizeof(Pnt));
                archive.Write(&bounds, sizeof(Pnt));

    }
    else
    {

                archive.Read(&ei.coord, sizeof(ei.coord));
                archive.Read(&a, sizeof(a));
                archive.Read(&b, sizeof(b));
                archive.Read(&c, sizeof(c));
                archive.Read(&color, sizeof(color));
                archive.Read(&angle, sizeof(angle));
                archive.Read(&scale, sizeof(Pnt));
                archive.Read(&bounds, sizeof(Pnt));


    }
*/
}

void Kirpich::Scale(float,float,float)
{

}

void Kirpich::Rotate(float,float,float)
{
	
}


void Kirpich::Render()
{
    Pnt p1[9];
	//glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	//glLoadIdentity();
    glDisable(GL_TEXTURE);

    //glTranslatef ( 0, 0, 0 );
    glTranslatef ( ei.coord.x, ei.coord.y, ei.coord.z );
	
	//glRotatef(angle.x, angle.y, angle.z,1);
	//glLoadIdentity (); 
    //glScalef(scale.x,scale.y,scale.z);
	p1[1] = Pnt(-(a / 2.), (b / 2.), (-c / 2.));
	p1[2] = Pnt(-(a / 2.), -(b / 2.), (-c / 2.));
	p1[3] = Pnt((a / 2.), -(b / 2.), (-c / 2.));
	p1[4] = Pnt((a / 2.), (b / 2.), (-c / 2.));
	p1[5] = Pnt((-a / 2.), (b / 2.), (c / 2.));
	p1[6] = Pnt(-(a / 2.), -(b / 2.), (c / 2.));
	p1[7] = Pnt((a / 2.), -(b / 2.), (c / 2.));
	p1[8] = Pnt((a / 2.), (b / 2.), (c / 2.));
		
	static float cl=0;		
	if(isSelected==2)
	{
		glEnable(GL_COLOR_MATERIAL);	
		glColor4f(1,0,0,cl);
		glPointSize(3);
		
		if(angle.x!=0)
			glRotatef(angle.x, 1, 0, 0);
		if(angle.y!=0)
			glRotatef(angle.y, 0, 1, 0);
		if(angle.z!=0)
			glRotatef(angle.z, 0, 0, 1);
		
		glScalef(scale.x, scale.y, scale.z);

		glBegin(GL_LINE_LOOP);
		glVertex3f(p1[2].x,p1[2].y,p1[2].z); 
		glVertex3f(p1[1].x,p1[1].y,p1[1].z);
		glVertex3f(p1[5].x,p1[5].y,p1[5].z);
		glVertex3f(p1[6].x,p1[6].y,p1[6].z);
		glEnd();
		
		glBegin(GL_LINE_LOOP);
		glVertex3f(p1[2].x,p1[2].y,p1[2].z); 
		glVertex3f(p1[6].x,p1[6].y,p1[6].z);
		glVertex3f(p1[7].x,p1[7].y,p1[7].z);
		glVertex3f(p1[3].x,p1[3].y,p1[3].z);
		glEnd();

		glBegin(GL_LINE_LOOP);
		glVertex3f(p1[7].x,p1[7].y,p1[7].z); 
		glVertex3f(p1[8].x,p1[8].y,p1[8].z);
		glVertex3f(p1[5].x,p1[5].y,p1[5].z);
		glVertex3f(p1[6].x,p1[6].y,p1[6].z);
		glEnd();
		
		glBegin(GL_LINE_LOOP);
		glVertex3f(p1[4].x,p1[4].y,p1[4].z); 
		glVertex3f(p1[1].x,p1[1].y,p1[1].z);
		glVertex3f(p1[2].x,p1[2].y,p1[2].z);
		glVertex3f(p1[3].x,p1[3].y,p1[3].z);
		glEnd();
			
		glBegin(GL_LINE_LOOP);
		glVertex3f(p1[1].x,p1[1].y,p1[1].z); 
		glVertex3f(p1[5].x,p1[5].y,p1[5].z);
		glVertex3f(p1[8].x,p1[8].y,p1[8].z);
		glVertex3f(p1[4].x,p1[4].y,p1[4].z);
		glEnd();
		
		glBegin(GL_LINE_LOOP);
		glVertex3f(p1[8].x,p1[8].y,p1[8].z); 
		glVertex3f(p1[7].x,p1[7].y,p1[7].z);
		glVertex3f(p1[3].x,p1[3].y,p1[3].z);
		glVertex3f(p1[4].x,p1[4].y,p1[4].z);
		glEnd();

		glPointSize(1);
		glDisable(GL_COLOR_MATERIAL);
		//if(gtimer.msec%10 == 0)
			 cl=cl+0.1;
		if(cl>1.0) cl=0;

	}
	//else
	{
		
		glPolygonMode( GL_FRONT_AND_BACK, GL_QUADS );
		glMaterialfv ( GL_FRONT_AND_BACK, GL_DIFFUSE, color );
	
		
		if(angle.x!=0)
			glRotatef(angle.x, 1, 0, 0);
		if(angle.y!=0)
			glRotatef(angle.y, 0, 1, 0);
		if(angle.z!=0)
            glRotatef(angle.z, 0, 0, 1);
		
		glScalef(scale.x, scale.y, scale.z);
		
		RGB2GL2(RGB(255,0,0),color);
		glMaterialfv ( GL_FRONT_AND_BACK, GL_DIFFUSE, color );
        glBegin(GL_QUADS);
		glVertex3f(p1[2].x,p1[2].y,p1[2].z); 
		glVertex3f(p1[1].x,p1[1].y,p1[1].z);
		glVertex3f(p1[5].x,p1[5].y,p1[5].z);
		glVertex3f(p1[6].x,p1[6].y,p1[6].z);
		glEnd();
		
		RGB2GL2(RGB(0,255,0),color);
		glMaterialfv ( GL_FRONT_AND_BACK, GL_DIFFUSE, color );
        glBegin(GL_QUADS);
		glVertex3f(p1[2].x,p1[2].y,p1[2].z); 
        glVertex3f(p1[6].x,p1[6].y,p1[6].z);
		glVertex3f(p1[7].x,p1[7].y,p1[7].z);
		glVertex3f(p1[3].x,p1[3].y,p1[3].z);
		glEnd();
		
		RGB2GL2(RGB(0,0,255),color);
		glMaterialfv ( GL_FRONT_AND_BACK, GL_DIFFUSE, color );
        glBegin(GL_QUADS);
		glVertex3f(p1[7].x,p1[7].y,p1[7].z); 
		glVertex3f(p1[8].x,p1[8].y,p1[8].z);
		glVertex3f(p1[5].x,p1[5].y,p1[5].z);
		glVertex3f(p1[6].x,p1[6].y,p1[6].z);
		glEnd();
		
		RGB2GL2(RGB(255,255,0),color);
		glMaterialfv ( GL_FRONT_AND_BACK, GL_DIFFUSE, color );
        glBegin(GL_QUADS);
		glVertex3f(p1[4].x,p1[4].y,p1[4].z); 
		glVertex3f(p1[1].x,p1[1].y,p1[1].z);
		glVertex3f(p1[2].x,p1[2].y,p1[2].z);
		glVertex3f(p1[3].x,p1[3].y,p1[3].z);
		glEnd();
		
		RGB2GL2(RGB(0,0,0),color);
		glMaterialfv ( GL_FRONT_AND_BACK, GL_DIFFUSE, color );
		
        glBegin(GL_QUADS);
		glVertex3f(p1[1].x,p1[1].y,p1[1].z); 
		glVertex3f(p1[5].x,p1[5].y,p1[5].z);
		glVertex3f(p1[8].x,p1[8].y,p1[8].z);
		glVertex3f(p1[4].x,p1[4].y,p1[4].z);
		glEnd();
		
		RGB2GL2(RGB(255,255,255),color);
		glMaterialfv ( GL_FRONT_AND_BACK, GL_DIFFUSE, color );
        glBegin(GL_QUADS);
		glVertex3f(p1[8].x,p1[8].y,p1[8].z); 
		glVertex3f(p1[7].x,p1[7].y,p1[7].z);
		glVertex3f(p1[3].x,p1[3].y,p1[3].z);
		glVertex3f(p1[4].x,p1[4].y,p1[4].z);
		glEnd();
	}

		glPopMatrix();
		



}




//obj update slot
void Kirpich::Slot1()
{
	
	if(m_Updated)
	{
		bounds.x = a*scale.x;
		bounds.y = b*scale.y;
		bounds.z = c*scale.z;
		//mu_printf("%s accept new params %s", name.c_str(), bounds.toStr().c_str());
		m_Updated = false;
	}

    //mov->CalcTrenie(this);
    //mov->CalcGravity(this);
    //mov->SchislenieCoord(this);

}
