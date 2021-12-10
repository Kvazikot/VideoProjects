/**********************************************************************
 *  Project App6
 *
 *  graviball.cpp
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

#include "../include/stdafx.h"
#include "../include/graviball.h"
#include "../include/terrain.h"
#include "../include/timer.h"
#include "../include/vector3d.h"
#include "../include/cobjregistry.h"
#include "../include/player.h"
#include "../include/glcode.h"
#include "../include/param_screen_bcb.h"

static CObjRegistry& MREG = CObjRegistry::getInstance2();
static ParamScreen& PSCR = ParamScreen::getInstance();



GraviBall::GraviBall()
{
    my_player = new Player();
    isCanRender = true;
    R = 10;
    ball = new Ball(std::string("_ball01"),Pnt(0,0,0),(float)R,(float)255,(float)0,(float)0);
    my_player->SetControlShema(87,83,65,68);
    my_player->cam_dist = 3;
    MREG.AddObjNew(ball);
    MREG.AddObjNew(my_player);
    ter = 0;
    near_idx = -1;
    PSCR.AddVar("player_pos");
}

PPnt* GraviBall::GetTerrainPoint(Pnt& p_near_f)
{
    PPnt* p;

    if( ter==0 ) { return 0; }

    if( near_idx == -1 )
    {
        p = ter->FindNearestP(p_near_f);  // по всему ландшафту
        near_idx = p->i;
    }
    else
    {
        p = ter->FindClosestTo(p_near_f, near_idx, 500, 0); // 100 точек в окрестности near_idx точки
    }
    return p;

}

void GraviBall::Render(QOpenGLShaderProgram& program)
{
    draw_p(program, "Cp",Cp,RGB(255,0,0),0);
    draw_p(program, "Tp",Tp,RGB(0,255,0),0);
    if(ter != 0) DrawWaves();
}

void GraviBall::DrawWaves()
{
    glPushMatrix();
    double alfa, A, Ag, A0, Av, x, R2, R1;
    Pnt p2,p1,pt;
    A0 = R/2;
    R1 = R;
    R2 = 2*R;
/*
    for(alfa=0; alfa < M_PI; alfa+=M_PI/180 )
    {
        Ag = A0 * sin( alfa ) - A0/2;
        Av = A0;
        p2 = Pnt(cos(alfa) * R2 + Cp.x, 0, sin(alfa) * R2 + Cp.z );
        p1 = Pnt(cos(alfa) * R1 + Cp.x, 0, sin(alfa) * R1 + Cp.z);
        glBegin(GL_LINES);
        for( x=0; x < 1; x+=0.01 )
        {
            A = (Av + Ag) * sin( 2 * x * M_PI );
            pt = line_eq(p1, p2, x);
            pt.y = A - R/2;
            glVertex3f(pt.x, pt.y, pt.z);
        }
        glEnd();
    }
*/

    for(unsigned i=0; i < ter->vertL.size(); i++)
    {
        PPnt* p = ter->vertL[i];
        Tp = Pnt(*p->x,*p->y,*p->z);
        double r = rasst(Tp,Cp);
        if( r < R2 && r > R1 )
        {
            x  = (r - R1)  / (R2 - R1);
            CVector3 v1(Tp.x, 0, Tp.z);
            CVector3 v2(Cp.x, 0, Cp.z);
            alfa = acos( v1 | v2 /( v1.findMagnitude() * v2.findMagnitude() ));
            Ag = A0 * sin( alfa ) - A0/2;
            Av = A0;
            A = (Av + Ag) * sin( 2 * x * M_PI ) - R/2;
            if(A > terrain_mesh_copy->vertices[p->i*3+1])
            *p->y = A;
        }
        else
        {
            *p->y = terrain_mesh_copy->vertices[p->i*3+1];
        }

    }


    glPopMatrix();
}

void GraviBall::Slot1()
{
    Cp = my_player->pos;
    // задать рендомное движение препятствия
    ball->ei.coord = Cp;
    PSCR.updP("player_pos",Cp);
    CObjRegistry& MREG = CObjRegistry::getInstance2();
    if(ter == 0)
    {
        ter = (MTerrain*)MREG.FindObj("terrain");
        if(ter!=0)
        {
            terrain_mesh_copy = new GemObject();
            terrain_mesh_copy->Copy(ter->terrain_mesh);
        }
    }
}
