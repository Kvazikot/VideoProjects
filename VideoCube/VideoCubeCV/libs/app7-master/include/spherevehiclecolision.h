#ifndef SPHEREVEHICLECOLISION_H
#define SPHEREVEHICLECOLISION_H

#include "stdafx.h"
#include "graviball.h"
#include "vehicle.h"

typedef struct
{
    bool	active;					// Active (Yes/No)
    float	life;					// Particle Life
    float	fade;					// Fade Speed
    float	r;						// Red Value
    float	g;						// Green Value
    float	b;						// Blue Value
    float	x;						// X Position
    float	y;						// Y Position
    float	z;						// Z Position
    float	xi;						// X Direction
    float	yi;						// Y Direction
    float	zi;						// Z Direction
    float	xg;						// X Gravity
    float	yg;						// Y Gravity
    float	zg;						// Z Gravity
    float   size;                   // Size
}particles;



#define	MAX_PARTICLES	1000		// Number Of Particles To Create

class SphereVehicleColision : public RenderObject
{
 //   Q_OBJECT
public:
    particles particle[MAX_PARTICLES];	// Particle Array (Room For Particle Info)
    GLuint	texture[1];					// Storage For Our Particle Texture
    GLuint	loop;						// Misc Loop Variable
    GLuint	col;						// Current Color Selection
    GLuint	delay;						// Rainbow Effect Delay
    float	slowdown;				// Slow Down Particles
    float	xspeed;						// Base X Speed (To Allow Keyboard Direction Of Tail)
    float	yspeed;						// Base Y Speed (To Allow Keyboard Direction Of Tail)
    float	zoom;                   	// Used To Zoom Out
    float   size;                       // Particle size
    int   variance;                   // Variance of size
    int     volume_body;                // 0-Box 1-Sphere


    CVector3  Vp,Cp;                //Vehicle pos
    GraviBall* graviball;
    CVehicle*  vehicle;


    SphereVehicleColision();
    SphereVehicleColision(const SphereVehicleColision& B){size = B.size;}
    int  LoadGLTextures();
    int  InitGL();
    int  DrawGLScene();
    void FindModels();
    void MakeDissapearEffect();
    void StartEmition();
    void Render();
    void Slot1();
    void Declare()
    {
        classname = "SphereVehicleColision";
        SET_OBJECT(this,(char*)name.c_str());
        SET_ATTR("size",  &size, "float");
        SET_ATTR("variance",  &variance, "int");
        SET_ATTR("xspeed",  &xspeed, "float");
        SET_ATTR("yspeed",  &yspeed, "float");
        SET_ATTR("volume_body", &volume_body, "int");
        //SET_ATTR("freqs",  &freqs, "string");
    }
    void OnAttrChange(t_atr& a);
};

#endif // SPHEREVEHICLECOLISION_H
