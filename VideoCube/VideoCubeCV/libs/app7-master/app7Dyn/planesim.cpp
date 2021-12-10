/**********************************************************************
 *  Project App6
 *
 *  planesim.cpp
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
#include "../include/print.h"
#include "../include/geometry.h"
#include "../include/interp.h"
#include "../include/planesim.h"
#include "../include/var_parse.h"
#include "../include/random_staff.h"
#define PLANESIM_DEBUG_INFO_LEVEL 5
void gprintf(const char* fmt,...);
#define DBG(level, fmt, args) do{ if ((level) <= PLANESIM_DEBUG_INFO_LEVEL) gprintf(fmt,##args); } while(0)


void InitPlaneSimulation( PlaneSim_Input& sim_in,  PlaneSim_Output& sim_out )
{
	prn("======InitPlaneSimulation=======");
	sim_in.S = Pnt(0,0,0); 
	sim_in.a = Pnt(0,0,0);
    sim_in.v = Pnt(0,0.3f,0);
//	sim_in.angles = Pnt(0,0,0);
    sim_in.trac = -0.1f;
    sim_in.mass = 0.8f;
    sim_in.G = sim_in.mass * 9.8f;
    sim_in.rule.PitchW = 0.4f;
    sim_in.rule.RollW = 0.6f;
	sim_in.rule.TrottleW = 100;
	
	sim_out.AssignFromInput( sim_in );
	prn("mass = %f", sim_in.mass);
	prn("G = %f", sim_in.G);
	prn("======End of InitPlaneSimulation=======");
}

void PlaneSim_Output::AssignFromInput(PlaneSim_Input& in)
{
	v = in.v; a = in.a; 
	angles = in.angles;
	S = in.S;
}

void PlaneSim_Input::AssignFromOutput(PlaneSim_Output& out)
{
	v = out.v; a = out.a; 
	angles = out.angles;
	S = out.S;		
}


Pnt CalcTeilor(double dt, Pnt r0, Pnt v, Pnt a)
{
	Pnt val;
	val.x = r0.x + v.x*dt + a.x*dt*dt / 2.;
	val.y = r0.y + v.y*dt + a.y*dt*dt / 2.;
	val.z = r0.z + v.z*dt + a.z*dt*dt / 2.;
	return val;

}


void PlaneSimulation( double dt, PlaneSim_Input& sim_in, PlaneSim_Output& sim_out )
{
	float step;

	sim_in.AssignFromOutput( sim_out );
    
	Pnt a = sim_in.a;	Pnt s = sim_in.S;	Pnt v = sim_in.v;
	Angles agl = sim_out.angles;

	
	agl.pitch = agl.pitch  + sim_in.rule.rulePitch;
	//prn("agl.pitch %f sim_in.rule.rulePitch %f", agl.pitch, sim_in.rule.rulePitch);
	agl.roll = agl.roll  + sim_in.rule.ruleRoll;
	//agl.yaw = agl.yaw ;

	if(agl.roll > 360) agl.roll = agl.roll - 360;
	if(agl.roll < -360) agl.roll = agl.roll + 360;
	if(agl.pitch > 360) agl.pitch = agl.pitch - 360;
	if(agl.pitch < -360) agl.pitch = agl.pitch + 360;
	
	step = sim_in.rule.ruleTrottleUp * dt * 3 - sim_in.rule.ruleTrottleDown * dt * 3;
    Pnt w;
	w.x = step * sim_in.rule.rulePitch;
	w.z = step * sim_in.rule.ruleRoll;


	v.z = v.z + w.z * sin( GR * agl.pitch) * sin( GR * agl.roll);
	v.y = v.y - step * cos( GR * agl.pitch)  ;
	v.x = v.x - w.x * sin( GR * agl.pitch) * cos(GR * agl.roll);
	a.x = (sim_in.v.x - v.x) / dt;
	a.y = (sim_in.v.y - v.y) / dt;
	a.z = (sim_in.v.z - v.z) / dt;
		 

	s = CalcTeilor(dt, s, v, a); 

	Atmosphere atm;
	ApplyAtmosphere(atm, agl, s);

	sim_out.S = s;
	sim_out.angles = agl;
	sim_out.v = v;
	sim_out.a = a;
	
}



Atmosphere::Atmosphere()
{
	probOfLvl[2.3] = 0.4;
}

void ApplyAtmosphere(Atmosphere& atm, Angles& agl, Pnt& S)
{
	atm.applyRandom = true;
    S=S;
	if( atm.applyRandom )
	{
        agl.roll+= randf(0.1f)*rand_sign(0.5);
        agl.roll += triangular(0.10, 0.60, 0.20)*rand_sign(0.5);
		agl.pitch += triangular(0.10, 0.60, 0.20)*rand_sign(0.5);
		//prn("rand_sign %d", rand_sign(0.3));
		//возврат
		if( agl.roll > 0) 
		{
			//agl.roll-=agl.roll*0.01; 
		}
		else 
		{
			//agl.roll+=agl.roll*0.01;
		}
			
	}

	if( atm.applyTurbulence )
	{

	}


	if( atm.applyVeter )
	{

	}
}
