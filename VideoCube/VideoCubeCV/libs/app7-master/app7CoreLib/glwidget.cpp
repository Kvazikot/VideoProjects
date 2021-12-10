/**********************************************************************
 *  Project App6
 *
 *  glwidget.cpp
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

//==========================================================
//        Виджет для 3д визуализации OpenGL
//==========================================================
#include "cinput.h"
#include "print.h"
#include "timer.h"
#include "renderobject.h"
#include "curve.h"
#include "flags.h"
#include "vpnt.h"
#include "cobjregistry.h"
#include "selection.h"
#include "cmaininterface.h"
#include "grid.h"
#include "shared_data.h"
#include "camera.h"
//#include "glcode.h"
#include "GLWidget.h"
#include <GL/glu.h>
#include <math.h>
#include <QGLContext>
#include <QImage>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

extern t_flags flags;
static char top_bmp[2048*1024*4];
static char persp_bmp[2048*1024*4];
static bool LoadOnly1 = 0;
static int RES_X;
static int RES_Y;
GLWidget* glw_main=0;
void	drawGrid(int square,int step, int ylevel);



GLWidget::GLWidget(QGLContext* ctx, QWidget *parent)
    : QGLWidget(ctx, parent)
{
    startTimer(1);
    setMouseTracking(true);
    setFocusPolicy( Qt::StrongFocus );
    setWindowTitle(tr("Sample Buffers"));
    MREG = CObjRegistry::getInstance();
    iinput = CInput::getInstance();
    bMinimap = 0;
    LoadOnly1 = 0;
    cam = 0;
    glw_main = this;
}

void GLWidget::SetMinimapMode()
{
    glDisable(GL_TEXTURE_2D);							// Enable Texture Mapping ( NEW )

}

void GLWidget::initializeGL()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-.5, .5, .5, -.5, -1000, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);


    glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping ( NEW )
    glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
    glClearDepth(1.0f);									// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
    glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
    // глобальный лайтинг
    glEnable(GL_LIGHTING);
    GLfloat light[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light);


    // Устанавливаем источники освещения по умолчанию
    GLfloat light0Position[] = { 0.0, 00.0, 10.0, 0.0 };
    GLfloat light1Position[] = { 1.0, -00.0, 10.0, 0.0 };
    GLfloat light1DiffuseAndSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light0Position);
    glLightfv(GL_LIGHT1, GL_POSITION, light1Position);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1DiffuseAndSpecular);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1DiffuseAndSpecular);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_RESCALE_NORMAL);

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    if(!LoadOnly1)
    {
        context()->makeCurrent();
        // загрузка обьектов
        MREG->Load();
        MREG->ListObjs();
        emit ObjectsIsLoaded();
        //mater->print();
        LoadOnly1 = 1;
    }


    return;


}


void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    RES_X = w;
    RES_Y = h;
    prn("RES_X RES_Y %d %d",w,h);
}

void GLWidget::paintGL()
{
    if(!flags.bdisable_timer1)
        display();
}


void GLWidget::CopyImage(QImage& img)
{
    char* pix = (char*)img.bits();
    //persp_bmp
    for(int i=0; i < (int)img.height() * img.width() * 4; i++)
    {
        persp_bmp[i] = *pix;
        pix++;
    }




}

QFont fontqt("Times", 10, QFont::Bold);

/*void renderBitmapString(float x,float y,float z,
                        float R, float G, float B,
                        void *font,	char *string)
{
    glw_main->qglColor(qRgb(R*255,G*255,B*255));
    glw_main->renderText(0,0,0,string,fontqt);
}
*/

double t1,t2,freq,fps;

void starttiming()
{
    QueryPerformanceFrequency((LARGE_INTEGER *)&freq);
    QueryPerformanceCounter((LARGE_INTEGER *)&t1);
}

void endtiming()
{
    QueryPerformanceCounter((LARGE_INTEGER *)&t2);
    fps=(double)(freq)/(double)(t2-t1);
}


void GLWidget::display(void)
{
    GLint viewport[4];
    static int cnt=-60;


    if(cam_name!="")
    {
        if(cam!=0)
            cam->isReceiveInput = false;
        cam = (Camera*)MREG->FindObj(cam_name.toStdString());
        cam->isReceiveInput = true;

    }
    else
        cam = (Camera*)MREG->FindObj("cam01");


    endtiming();
    starttiming();

    qglColor(Qt::white);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-.5, .5, .5, -.5, -1000, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    QString cam_pos_str;
    if(cam!=0)
        cam_pos_str.sprintf("cam_pos: %f %f %f \t fps %lf",cam->ei.coord.x,cam->ei.coord.y,cam->ei.coord.z, fps);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(0.25f, -0.10f, 0.0f);
    glPopMatrix();
    qglColor(Qt::white);
    renderText(-0.5, -0.4, -0.5, cam_pos_str);

    //return;

    //glMatrixMode(GL_MODELVIEW);

    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glGetIntegerv (GL_VIEWPORT, viewport);


    //позиция обзора
       if(cam!=0)
       {
           glMatrixMode(GL_PROJECTION);
           glLoadIdentity();
           if(cam->view==1) //top
           {

               glOrtho(-cam->ei.coord.y*cam->aspect/2,cam->ei.coord.y*cam->aspect/2,-cam->ei.coord.y/2,cam->ei.coord.y/2,cam->zNear,cam->zFar);
               gluLookAt(cam->ei.coord.x, cam->ei.coord.y, cam->ei.coord.z,
                         cam->lookAt.x,  cam->lookAt.y,  cam->lookAt.z,
                         0,  1,  0);

               static GLint    viewport[4];
               static GLdouble projection[16];
               static GLdouble modelview[16];
               static GLdouble vx,vy,vz;
               static GLdouble wx,wy,wz;
               glGetIntegerv(GL_VIEWPORT,viewport);
               glGetDoublev(GL_PROJECTION_MATRIX,projection);
               glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
               gluUnProject(iinput->mouse_x, RES_Y - iinput->mouse_y, 0, modelview, projection, viewport, &iinput->vx,&iinput->vy,&iinput->vz);

               CMainInterface& main_if = CMainInterface::getInstance();
               CSelection& gselection = CSelection::getInstance();
               //отработка селекта
               if(main_if.isInSelectionMode)
               {
                   if(cam->view==1)
                   {
                    //mu_printf("%d %d", iinput.mouse_x,iinput.mouse_y);
                     main_if.selobj = gselection.SelectByMouse(iinput->mouse_x,iinput->mouse_y);
                   }

               }


           }
           else if(cam->view==0) //perspective
           {

               gluPerspective(cam->fovy, cam->aspect, cam->zNear, cam->zFar);
               //glMatrixMode(GL_MODELVIEW);
               //glLoadIdentity();
               gluLookAt(cam->ei.coord.x, cam->ei.coord.y, cam->ei.coord.z,
                         cam->lookAt.x,  cam->lookAt.y,  cam->lookAt.z,
                         0,  1,  0);
           }
       }

/*
       glPixelStorei(GL_PACK_ALIGNMENT, 1);
       glPixelStorei(GL_PACK_ROW_LENGTH, RES_X);

     if(cam->view==0)
               {
                   //glReadBuffer ( GL_FRONT );
                   //int maxY=491, maxX=614;
                   int maxY=RES_Y, maxX=RES_X;
                   glReadPixels(
                   0,
                   0,
                   maxX,
                   maxY,
                   GL_BGR_EXT,
                   GL_UNSIGNED_BYTE,//GL_UNSIGNED_SHORT
                   (void*)persp_bmp
                   );
                   //prn("%d %d",RES_X, RES_Y);
               }
               if(cam->view==1)
               {
                   //glReadBuffer ( GL_FRONT );
                   //int maxY=491, maxX=614;
                   int maxY=RES_Y, maxX=RES_X;
                   glReadPixels(
                   0,
                   0,
                   maxX,
                   maxY,
                   GL_BGR_EXT,
                   GL_UNSIGNED_BYTE,//GL_UNSIGNED_SHORT
                   (void*)top_bmp
                   );
               }

*/

    glClearColor(0, 0, 0, 1);
    //if( flags.bShowGrid )
    //    grid.Render();

    MREG->UpdateNew();

    //основные обьекты
    vector<Object*>::iterator obj;
    for(obj = MREG->allObj.begin(); obj != MREG->allObj.end(); obj++ )
    {
        //if((*obj)->isEntety)
        if((*obj)->isCanRender)
        if((*obj)->isHidden==0)
        {
                    RenderObject* ro =  (RenderObject*)*obj;
                    ro->Render();
        }

    }

    //лееры (рисуемые поверх линии, точки, сплайны)
    CObjRegistry& MREG2 = CObjRegistry::getLayerInstance();
    for(obj = MREG2.allObj.begin(); obj != MREG2.allObj.end(); obj++ )
    {
        //if((*obj)->isEntety)
        if((*obj)->isCanRender)
        if((*obj)->isHidden==0)
        {
                    RenderObject* ro =  (RenderObject*)*obj;
                    ro->Render();
        }

    }
    // удаление обьектов если надо
    for(int i=0; i < (int)MREG2.removeQuenue.size(); i++)
    {
        Object* o = MREG2.removeQuenue[i];
        MREG2.allObj.erase( std::find(MREG2.allObj.begin(), MREG2.allObj.end(), o) );
        delete o;
    }
    MREG2.removeQuenue.clear();

    glFlush();
    frame = grabFrameBuffer();
    CopyImage(frame);
    Data* data = (Data*) MREG->FindObj("data");
    if(data!=0)
    {
        data->bits = (int)frame.bits();

    }



}


void GLWidget::timerEvent(QTimerEvent *)
{
    //update();
    updateGL();


}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
    focusWidget();

    if ( event->button() == Qt::LeftButton)
    {
      iinput->click_lmb(1);
    }

    if ( event->button() == Qt::RightButton)
    {
        iinput->click_rmb(1);
    }
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    lastPos = event->pos();
    if ( event->button() == Qt::LeftButton)
    {
      iinput->click_lmb(0);
    }
    if ( event->button() == Qt::RightButton)
    {
      iinput->click_rmb(0);
    }

}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    //prn("key %d", event->nativeVirtualKey());
    iinput->key_pressed(event->nativeVirtualKey());
    //event->
    if(event->key()<256)
    {

    }
}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
    iinput->key_upressed(event->nativeVirtualKey());
    if(event->key()<256)
    {

    }
}


void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    iinput->mouse_move(event->x(), event->y());
    lastPos = event->pos();

}

void GLWidget::wheelEvent(QWheelEvent *e)
{
    //cam01.mouse_wheel(e->delta());
    iinput->mouse_wheel(e->delta());

}

void GLWidget::SetCamName(QString name)
{
    cam_name = name;
/*    cam->isReceiveInput = false;
    Camera* cam = (Camera*)MREG->FindObj(name.toStdString());
    cam->isReceiveInput = true;
    t_atr* a = cam->GET_ATTR("view");
    cam->OnAttrChange(*a);
*/
}

void GetBmpPtrs(char** top, char** pers)
{
    *top = &top_bmp[0];
    *pers = &persp_bmp[0];
}

void GetBmpProps(int& stribe, int& maxX, int& maxY)
{
    maxX=RES_X; stribe = maxX*3; maxY=RES_Y;
}

