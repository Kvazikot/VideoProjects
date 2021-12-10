#include "../include/window.h"
#include "../include/print.h"
#include "../include/param_screen_bcb.h"
#include "../include/osd.h"
#include "../include/camera.h"
#include "../include/cobjregistry.h"
#include "../include/grid.h"
#include "../include/cinput.h"
#include "../include/selection.h"
#include "../include/cmaininterface.h"
#include "../include/flags.h"
#include "../include/glcode.h"

#include <GL/gl.h>			// Header File For The OpenGL32 Library
#include <GL/glu.h>			// Header File For The GLu32 Library
#include <GL/glut.h>


#include <QCoreApplication>
#include <QKeyEvent>
#include <QOpenGLContext>
#include <QTimer>


static COSD& osd = COSD::getInstance();
static ParamScreen&    pscr = ParamScreen::getInstance();
static ParamScreen& vt = ParamScreen::getVtInstance();
static CMainInterface& main_if = CMainInterface::getInstance();
static Camera* cam;
static QString cam_name="cam_3person";
static CObjRegistry& MREG = CObjRegistry::getInstance2();
static Grid    grid;
static CInput* iinput;
static t_flags& flags = GetGFlags();
static int RES_X;
static int RES_Y;


Window::Window( QScreen* screen )
    : QWindow( screen ),
      m_leftButtonPressed( false )
{
    // Tell Qt we will use OpenGL for this window
    setSurfaceType( OpenGLSurface );

    // Specify the format we wish to use
    QSurfaceFormat format;
    format.setDepthBufferSize( 24 );
    format.setMajorVersion( 4 );
    format.setMinorVersion( 3 );
    format.setSamples( 4 );
    format.setProfile( QSurfaceFormat::CoreProfile );
    //format.setOption( QSurfaceFormat::DebugContext );

    resize( 1366, 768 );
    setFormat( format );
    create();

    // Create an OpenGL context
    m_context = new QOpenGLContext;
    m_context->setFormat( format );
    m_context->create();

    // Setup our scene
    m_context->makeCurrent( this );
    //m_scene->setContext( m_context );
    initializeGL();

    // Make sure we tell OpenGL about new window sizes
    connect( this, SIGNAL( widthChanged( int ) ), this, SLOT( resizeGL() ) );
    connect( this, SIGNAL( heightChanged( int ) ), this, SLOT( resizeGL() ) );
    resizeGL();

    // This timer drives the scene updates
    QTimer* timer = new QTimer( this );
    connect( timer, SIGNAL( timeout() ), this, SLOT( updateScene() ) );
    timer->start( 16 );
}

void Window::initializeGL()
{
    m_context->makeCurrent( this );
    //m_scene->initialise();
    m_time.start();
}

static double t1,t2,freq,fps;

static void starttiming()
{
    //QueryPerformanceFrequency((LARGE_INTEGER *)&freq);
    //QueryPerformanceCounter((LARGE_INTEGER *)&t1);
}

static void endtiming()
{
    //QueryPerformanceCounter((LARGE_INTEGER *)&t2);
    fps=(double)(freq)/(double)(t2-t1);
    pscr.AddVar("fps");
    pscr.setd("fps",fps);
}

static void print_str(char* gstr)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glViewport(0, 0, RES_X, RES_Y);
    gluOrtho2D(0.0, RES_X, 0.0, RES_Y);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    //sprintf(gstr, "gtimer.t=%f сек.", Timer::gtimer.fsec);
    glutPrint2d(1, RES_Y-20, GLUT_BITMAP_9_BY_15, string(gstr),1,0,0,1);
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
}


void display2(void)
{
    GLint viewport[4];
    static int cnt=-60;

    if( main_if.activeCamera!="")
    {
//        if(cam!=0)
//            cam->isReceiveInput = false;
        cam = (Camera*)MREG.FindObj(main_if.activeCamera);
//        if(cam!=0)
//            cam->isReceiveInput = true;

    }
    else
        cam = (Camera*)MREG.FindObj("cam01");


    endtiming();
    starttiming();

    QString cam_pos_str;
    if(cam!=0)
    {

        //cam_pos_str.sprintf("                                  fps %lf", fps);
        //glutPrint2d(0,0,GLUT_BITMAP_TIMES_ROMAN_24,cam_pos_str.toStdString(),1,1,1,1);
    }


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    //renderText(-0.5, -0.4, -0.5, cam_pos_str);
    print_str((char*)cam_pos_str.toStdString().c_str());
    osd.Render(RES_X,RES_Y);


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


               CSelection& gselection = CSelection::getInstance();
               //отработка селекта
               if(main_if.isInSelectionMode)
               {
                   if(cam->view==1)
                   {
                     main_if.selobj = gselection.SelectByMouse(iinput->mouse_x,iinput->mouse_y);
                   }

               }


           }
           else if(cam->view==0) //perspective
           {

               gluPerspective(cam->fovy, cam->aspect, cam->zNear, cam->zFar);

               gluLookAt(cam->ei.coord.x, cam->ei.coord.y, cam->ei.coord.z,
                         cam->lookAt.x,  cam->lookAt.y,  cam->lookAt.z,
                         0,  1,  0);
           }
       }

    glClearColor(1, 1, 1, 1);

    //if( flags.bShowGrid )
    //    grid.Render();

    MREG.UpdateNew();

    //цветовые обьекты
    vector<Object*>::iterator obj;
    for(obj = MREG.allObj.begin(); obj != MREG.allObj.end(); obj++ )
    {
        //if((*obj)->isEntety)
        if((*obj)->isCanRender)
        if((*obj)->isHidden==0)
        if((*obj)->classname=="VPnt" || (*obj)->classname=="Kirpich")
        {
                    RenderObject* ro =  (RenderObject*)*obj;
                    ro->Render();
        }

    }



    //основные обьекты
    for(obj = MREG.allObj.begin(); obj != MREG.allObj.end(); obj++ )
    {
        //if((*obj)->isEntety)
        if((*obj)->isCanRender)
        if((*obj)->isHidden==0)
        if((*obj)->classname!="VPnt")
        if((*obj)->classname!="Kirpich")
        {
                    RenderObject* ro =  (RenderObject*)*obj;
                    ro->Render();
        }

    }

    //лееры (рисуемые поверх линии, точки, сплайны)
    CObjRegistry& MREG2 = CObjRegistry::getLayerInstance();
    MREG2.UpdateNew();
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




}

void Window::paintGL()
{
    // Make the context current
    m_context->makeCurrent( this );

    // Do the rendering (to the back buffer)
    display2();

    // Swap front/back buffers
    m_context->swapBuffers( this );
}

void Window::resizeGL()
{
    m_context->makeCurrent( this );
    //m_scene->resize( width(), height() );
}

void Window::updateScene()
{
    float time = m_time.elapsed() / 1000.0f;
    //m_scene->update( time );
    paintGL();
}

void Window::keyPressEvent( QKeyEvent* e )
{
    const float speed = 44.7f; // in m/s. Equivalent to 100 miles/hour
    //TerrainTessellationScene* scene = static_cast<TerrainTessellationScene*>( m_scene );
    iinput->key_pressed(e->key());
    switch ( e->key() )
    {
        case Qt::Key_Escape:
            QCoreApplication::instance()->quit();
            break;

        //default:
        //    QWindow::keyPressEvent( e );
    }
}

void Window::keyReleaseEvent( QKeyEvent* e )
{
    iinput->key_upressed(e->key());
    switch ( e->key() )
    {
       // default:
       //     QWindow::keyReleaseEvent( e );
    }
}

void Window::mousePressEvent( QMouseEvent* e )
{
    if ( e->button() == Qt::LeftButton )
    {
        m_leftButtonPressed = true;
        m_pos = m_prevPos = e->pos();
        iinput->click_lmb(1);
    }
    //QWindow::mousePressEvent( e );
}

void Window::mouseReleaseEvent( QMouseEvent* e )
{
    if ( e->button() == Qt::LeftButton )
    {
        m_leftButtonPressed = false;
        iinput->click_lmb(0);
    }
    //QWindow::mouseReleaseEvent( e );
}

void Window::mouseMoveEvent( QMouseEvent* e )
{
    if ( m_leftButtonPressed )
    {
        m_pos = e->pos();
        float dx = 0.2f * ( m_pos.x() - m_prevPos.x() );
        float dy = -0.2f * ( m_pos.y() - m_prevPos.y() );
        m_prevPos = m_pos;
        iinput->mouse_move(m_pos.x(),m_pos.y());
    }

   // QWindow::mouseMoveEvent( e );

}
