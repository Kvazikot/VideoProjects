/**********************************************************************
 *  Project App6
 *
 *  spherevehiclecolision.cpp
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
#include "../include/spherevehiclecolision.h"
#include "../include/graviball.h"
#include "../include/vehicle.h"

static CInput& iinput = CInput::getInstance2();
static CObjRegistry& MREG = CObjRegistry::getInstance2();
static ParamScreen& PSCR = ParamScreen::getInstance();


static GLfloat Rainbow[12][3]=		// Rainbow Of Colors
{
    {1.0f,0.5f,0.5f},{1.0f,0.75f,0.5f},{1.0f,1.0f,0.5f},{0.75f,1.0f,0.5f},
    {0.5f,1.0f,0.5f},{0.5f,1.0f,0.75f},{0.5f,1.0f,1.0f},{0.5f,0.75f,1.0f},
    {0.5f,0.5f,1.0f},{0.75f,0.5f,1.0f},{1.0f,0.5f,1.0f},{1.0f,0.5f,0.75f}
};

SphereVehicleColision::SphereVehicleColision()
{
    graviball=0;
    vehicle=0;
    slowdown=2.0f;
    size = 0.5;
    variance = 25; // procent
    volume_body = 1;
    col=0;
    isCanRender = true;
}

/*
AUX_RGBImageRec *LoadBMP(char *Filename)
{
        FILE *File=NULL;
        if (!Filename)
        {
                return NULL;
        }
        File=fopen(Filename,"r");
        if (File)
        {
            fclose(File);
            return auxDIBImageLoadA(Filename);
        }
        return NULL;
}
*/

int SphereVehicleColision::LoadGLTextures()
{
    /*
        int Status=false;
        AUX_RGBImageRec *TextureImage[1];
        memset(TextureImage,0,sizeof(void *)*1);

        if (TextureImage[0]=LoadBMP("textures/Particle.bmp"))
        {
            Status=true;
            glGenTextures(1, &texture[0]);

            glBindTexture(GL_TEXTURE_2D, texture[0]);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
        }

        if (TextureImage[0])
        {
            if (TextureImage[0]->data)
            {
                free(TextureImage[0]->data);
            }
            free(TextureImage[0]);
        }
        return Status;
    */
    return false;
}

void SphereVehicleColision::StartEmition()
{
    for (loop=0;loop<MAX_PARTICLES;loop++)				// Initials All The Textures
    {
        particle[loop].active=true;								// Make All The Particles Active
        particle[loop].life=1.0f;								// Give All The Particles Full Life
        particle[loop].size=float(size*(1 - rand()%variance)/100.0f);
        particle[loop].fade=float(rand()%100)/1000.0f+0.001f;	// Random Fade Speed
        particle[loop].r=Rainbow[loop*(12/MAX_PARTICLES)][0];	// Select Red Rainbow Color
        particle[loop].g=Rainbow[loop*(12/MAX_PARTICLES)][1];	// Select Red Rainbow Color
        particle[loop].b=Rainbow[loop*(12/MAX_PARTICLES)][2];	// Select Red Rainbow Color
        particle[loop].xi=float((rand()%50)-26.0f)*10.0f;    	// Random Speed On X Axis
        particle[loop].yi=float((rand()%50)-25.0f)*10.0f;	   // Random Speed On Y Axis
        particle[loop].zi=float((rand()%50)-25.0f)*10.0f;	   // Random Speed On Z Axis
        particle[loop].xg=0.0f;									// Set Horizontal Pull To Zero
        particle[loop].yg=-0.8f;								// Set Vertical Pull Downward
        particle[loop].zg=0.0f;									// Set Pull On Z Axis To Zero
        particle[loop].x = 0;
        particle[loop].y = 0;
        particle[loop].z = 0;
    }

    for (loop=0;loop<MAX_PARTICLES;loop++)
    {
        float dist = float(rand()%250);
        float alfa = float(rand()%360);
        float phi = float(rand()%360);
        particle[loop].xi  = dist*sin(GR * alfa)*sin(GR * phi);
        particle[loop].yi  = dist*cos(GR * alfa);
        particle[loop].zi =  dist*sin(GR * alfa)*cos(GR * phi);
    }
}

void SphereVehicleColision::OnAttrChange(t_atr& a)
{
    a.isHidden();
    StartEmition();
}

int SphereVehicleColision::InitGL()
{
    if (!LoadGLTextures())
    {
        return false;
    }

    glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
    //glClearColor(0.0f,0.0f,0.0f,0.0f);					// Black Background
    //glClearDepth(1.0f);									// Depth Buffer Setup
    //glDisable(GL_DEPTH_TEST);							// Disable Depth Testing
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Type Of Blending To Perform
    //glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);	// Really Nice Perspective Calculations
    //glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);				// Really Nice Point Smoothing
    glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
    glBindTexture(GL_TEXTURE_2D,texture[0]);			// Select Our Texture
    StartEmition();
    return true;
}

int SphereVehicleColision::DrawGLScene()
{
   glPushMatrix();
   glTranslatef(Cp.x,Cp.y,Cp.z);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glLoadIdentity();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texture[0]);			// Select Our Texture
    glEnable(GL_BLEND);									// Enable Blending
    glDisable(GL_DEPTH_TEST);
    //glClearColor(0.0f,0.0f,0.0f,0.0f);					// Black Background
    glClearDepth(1.0f);									// Depth Buffer Setup

    for (loop=0;loop<MAX_PARTICLES;loop++)
    {
        if (particle[loop].active)
        {
            float x=particle[loop].x;
            float y=particle[loop].y;
            float z=particle[loop].z;

            // Draw The Particle Using Our RGB Values, Fade The Particle Based On It's Life
            glColor4f(particle[loop].r,particle[loop].g,particle[loop].b,particle[loop].life);

            glBegin(GL_TRIANGLE_STRIP);						// Build Quad From A Triangle Strip
                glTexCoord2d(1,1); glVertex3f(x+size,y+size,z); // Top Right
                glTexCoord2d(0,1); glVertex3f(x-size,y+size,z); // Top Left
                glTexCoord2d(1,0); glVertex3f(x+size,y-size,z); // Bottom Right
                glTexCoord2d(0,0); glVertex3f(x-size,y-size,z); // Bottom Left
            glEnd();										// Done Building Triangle Strip

            glBegin(GL_TRIANGLE_STRIP);						// Build Quad From A Triangle Strip
                glTexCoord2d(1,1); glVertex3f(x,y+size,z+size); // Top Right
                glTexCoord2d(0,1); glVertex3f(x,y+size,z-size); // Top Left
                glTexCoord2d(1,0); glVertex3f(x,y-size,z+size); // Bottom Right
                glTexCoord2d(0,0); glVertex3f(x,y-size,z-size); // Bottom Left
            glEnd();										// Done Building Triangle Strip


            particle[loop].x+=particle[loop].xi/(slowdown*1000);// Move On The X Axis By X Speed
            particle[loop].y+=particle[loop].yi/(slowdown*1000);// Move On The Y Axis By Y Speed
            particle[loop].z+=particle[loop].zi/(slowdown*1000);// Move On The Z Axis By Z Speed

            particle[loop].xi+=particle[loop].xg;			// Take Pull On X Axis Into Account
            particle[loop].yi+=particle[loop].yg;			// Take Pull On Y Axis Into Account
            particle[loop].zi+=particle[loop].zg;			// Take Pull On Z Axis Into Account
            particle[loop].life-=particle[loop].fade;		// Reduce Particles Life By 'Fade'

            if (particle[loop].life<0.0f)					// If Particle Is Burned Out
            {

                //col=loop%12;
                particle[loop].life=1.0f;					// Give It New Life
                particle[loop].fade=float(rand()%100)/1000.0f+0.003f;	// Random Fade Value
                particle[loop].x=0.0f;						// Center On X Axis
                particle[loop].y=0.0f;						// Center On Y Axis
                particle[loop].z=0.0f;						// Center On Z Axis
                particle[loop].xi=xspeed+float((rand()%60)-32.0f);	// X Axis Speed And Direction
                particle[loop].yi=yspeed+float((rand()%60)-30.0f);	// Y Axis Speed And Direction
                particle[loop].zi=float((rand()%60)-30.0f);	// Z Axis Speed And Direction
                particle[loop].r=Rainbow[col][0];			// Select Red From Color Table
                particle[loop].g=Rainbow[col][1];			// Select Green From Color Table
                particle[loop].b=Rainbow[col][2];			// Select Blue From Color Table


            }

            /*
            // If Number Pad 8 And Y Gravity Is Less Than 1.5 Increase Pull Upwards
            if (iinput.keystates[VK_NUMPAD8] && (particle[loop].yg<1.5f))
                particle[loop].yg+=0.01f;

            // If Number Pad 2 And Y Gravity Is Greater Than -1.5 Increase Pull Downwards
            if (iinput.keystates[VK_NUMPAD2] && (particle[loop].yg>-1.5f)) particle[loop].yg-=0.01f;

            // If Number Pad 6 And X Gravity Is Less Than 1.5 Increase Pull Right
            if (iinput.keystates[VK_NUMPAD6] && (particle[loop].xg<1.5f)) particle[loop].xg+=0.01f;

            // If Number Pad 4 And X Gravity Is Greater Than -1.5 Increase Pull Left
            if (iinput.keystates[VK_NUMPAD4] && (particle[loop].xg>-1.5f)) particle[loop].xg-=0.01f;

            if (iinput.keystates[49])										// Tab Key Causes A Burst
            {
                StartEmition();
                col=rand()%11;
            }
            */
        }
    }
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);									// Enable Blending
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();

    if (delay>25 )	// Space Or Rainbow Mode
    {
        delay=0;						// Reset The Rainbow Color Cycling Delay
        col++;							// Change The Particle Color
        if (col>11)	col=0;				// If Color Is To High Reset It
    }
    delay++;							// Increase Rainbow Mode Color Cycling Delay Counter
    return true;											// Everything Went OK
}



void SphereVehicleColision::FindModels()
{
    graviball = (GraviBall*)MREG.FindObj("gravi01");
    vehicle = (CVehicle*)MREG.FindObj("veh01");
}

void SphereVehicleColision::MakeDissapearEffect()
{
    DrawGLScene();

}

void SphereVehicleColision::Slot1()
{
    if(graviball==0 || vehicle==0)
    {
        FindModels();
    }

}

void SphereVehicleColision::Render()
{
    static bool ot=true;
    if( ot )
    {
      InitGL();
      ot=false;
    }

    if(graviball==0 || vehicle==0) return;

    Cp = graviball->Cp;
    Vp = vehicle->ei.coord;
    CVector3 v = Cp - Vp;
    if( v.findMagnitude() < graviball->R )
    {
        /*
        particle[loop].x=0.0f;								// Center On X Axis
        particle[loop].y=0.0f;								// Center On Y Axis
        particle[loop].z=0.0f;								// Center On Z Axis
        particle[loop].xi=float((rand()%50)-26.0f)*10.0f;	// Random Speed On X Axis
        particle[loop].yi=float((rand()%50)-25.0f)*10.0f;	// Random Speed On Y Axis
        particle[loop].zi=float((rand()%50)-25.0f)*10.0f;	// Random Speed On Z Axis
        */
        vehicle->isCanRender = false;
    }else
        vehicle->isCanRender = true;

    MakeDissapearEffect();

}
