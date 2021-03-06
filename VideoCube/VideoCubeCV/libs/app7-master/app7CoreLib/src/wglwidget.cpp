/**********************************************************************
 *  Project App6
 *
 *  wglwidget.cpp
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

#include <gl\glew.h>
#include "camera.h"
#include "cobjregistry.h"
#include "grid.h"
#include "cinput.h"
#include "selection.h"
#include "cmaininterface.h"
#include "flags.h"
#include "glcode.h"
#include <windows.h>		// Header File For Windows
#include <stdio.h>			// Header File For Standard Input/Output
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glut.h>
#include <process.h>
#include "wglwidget.h"
#include <windowsx.h>
#include "print.h"
#include "param_screen_bcb.h"
#include "osd.h"

static COSD& osd = COSD::getInstance();
static ParamScreen&    pscr = ParamScreen::getInstance();
static ParamScreen& vt = ParamScreen::getVtInstance();
static CMainInterface& main_if = CMainInterface::getInstance();
static CObjRegistry& MREG = CObjRegistry::getInstance2();
Grid    grid;
CInput* iinput;
static t_flags& flags = GetGFlags();
HINSTANCE	hInstance;		// Holds The Instance Of The Application
bool	keys[256];			// Array Used For The Keyboard Routine
bool	active=TRUE;		// Window Active Flag Set To TRUE By Default
bool	fullscreen=TRUE;	// Fullscreen Flag Set To Fullscreen Mode By Default
static int RES_X;
static int RES_Y;
//GLuint	texture[1];			// Storage For One Texture ( NEW )
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag, int topmost);
CRITICAL_SECTION CriticalSection;

std::map<std::string, WglWidget*> WglWidget::wgl_tab;
std::map<HWND, WglWidget*> WglWidget::handle2window_tab;

WglWidget::WglWidget(char* wndName, int width, int height, int bpp, int fullscreen, int topmost)
{
    viewport_name = wndName;
    wgl_tab[viewport_name] = this;
    flags.bdisable_timer1 = true;
    iinput = CInput::getInstance();
    cam = 0;
    hDC=NULL;
    hRC=NULL;
    done=FALSE;
    InitializeCriticalSectionAndSpinCount(&CriticalSection, 0x00000400);
    if( fullscreen == -1)
    // Ask The User Which Screen Mode They Prefer
    if (MessageBoxA(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
    {
        fullscreen=FALSE;							// Windowed Mode
    }

    // Create Our OpenGL Window
    if (!CreateGLWindow(wndName,width,height,bpp,fullscreen,topmost))
    {
        return;									// Quit If Window Was Not Created
    }



}

QWidget* WglWidget::GetWidget(std::string name)
{
    if( wgl_tab.find(name)!=wgl_tab.end() )
    {
        return wgl_tab[name]->widget;
    }
    else
    {
        return 0;
    }
}

WglWidget* WglWidget::GetWindow(std::string name)
{
    if( wgl_tab.find(name)!=wgl_tab.end() )
    {
        return wgl_tab[name];
    }
    else
    {
        return 0;
    }
}

WglWidget* WglWidget::GetWindowByHandle(HWND hwnd)
{
    if( handle2window_tab.find(hwnd)!=handle2window_tab.end() )
    {
        return handle2window_tab[hwnd];
    }
    else
    {
        return 0;
    }
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
    if (height==0)										// Prevent A Divide By Zero By
    {
        height=1;										// Making Height Equal One
    }

    glViewport(0,0,width,height);						// Reset The Current Viewport
    RES_X = width;
    RES_Y = height;
    vt.AddVar("RES_X");
    vt.AddVar("RES_Y");
    vt.seti("RES_X",width);
    vt.seti("RES_Y",height);
    glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
    glLoadIdentity();									// Reset The Projection Matrix

    // Calculate The Aspect Ratio Of The Window
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

    glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
    glLoadIdentity();									// Reset The Modelview Matrix
}

int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-.5, .5, .5, -.5, -1000, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    //GLenum err = glewInit();
    glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping ( NEW )
    glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
    glClearDepth(1.0f);									// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
    glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
    // ?????????? ???????
    glEnable(GL_LIGHTING);
    GLfloat light[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light);


    // ????????????? ????????? ????????? ?? ?????????
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

    return TRUE;										// Initialization Went OK
}

static double t1,t2,freq,fps;

static void starttiming()
{
    QueryPerformanceFrequency((LARGE_INTEGER *)&freq);
    QueryPerformanceCounter((LARGE_INTEGER *)&t1);
}

static void endtiming()
{
    QueryPerformanceCounter((LARGE_INTEGER *)&t2);
    fps=(double)(freq)/(double)(t2-t1);
    pscr.AddVar("fps");
    pscr.setd("fps",fps);
}

void print_str(char* gstr)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glViewport(0, 0, RES_X, RES_Y);
    gluOrtho2D(0.0, RES_X, 0.0, RES_Y);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    //sprintf(gstr, "gtimer.t=%f ???.", Timer::gtimer.fsec);
    glutPrint2d(1, RES_Y-20, GLUT_BITMAP_9_BY_15, string(gstr),1,0,0,1);
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
}


void WglWidget::display(void)
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


    //??????? ??????
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
               //????????? ???????
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

    //???? ?????????
    vector<Object*>::iterator obj;
    for(obj = MREG.allObj.begin(); obj != MREG.allObj.end(); obj++ )
    {
        //if((*obj)->isEntety)
        if((*obj)->isCanRender)
        {
                    RenderObject* ro =  (RenderObject*)*obj;
                    ro->Render();
        }

    }


    // ???????? ???????? ???? ????
    for(int i=0; i < (int)MREG.removeQuenue.size(); i++)
    {
        Object* o = MREG.removeQuenue[i];
        MREG.allObj.erase( std::find(MREG.allObj.begin(), MREG.allObj.end(), o) );
        delete o;
    }
    MREG.removeQuenue.clear();

    glFlush();




}

int WglWidget::DrawGLScene()
{
    //EnterCriticalSection(&CriticalSection);
    if( flags.bIsLoading ) return 1;
    if( flags.bdisable_timer1 ) return 1;
    display();
    //LeaveCriticalSection(&CriticalSection);
    return TRUE;										// Keep Going
}

void WglWidget::KillGLWindow()
{
    if (fullscreen)										// Are We In Fullscreen Mode?
    {
        ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
        ShowCursor(TRUE);								// Show Mouse Pointer
    }

    if (hRC)											// Do We Have A Rendering Context?
    {
        if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
        {
            MessageBoxA(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        }

        if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
        {
            MessageBoxA(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        }
        hRC=NULL;										// Set RC To NULL
    }

    if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
    {
        MessageBoxA(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        hDC=NULL;										// Set DC To NULL
    }

    if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
    {
        MessageBoxA(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        hWnd=NULL;										// Set hWnd To NULL
    }

    if (!UnregisterClassA("OpenGL",hInstance))			// Are We Able To Unregister Class
    {
        MessageBoxA(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        hInstance=NULL;									// Set hInstance To NULL
    }
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/

BOOL WglWidget::CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag, int topmost)
{
    GLuint		PixelFormat;			// Holds The Results After Searching For A Match
    WNDCLASS	wc;						// Windows Class Structure
    DWORD		dwExStyle;				// Window Extended Style
    DWORD		dwStyle;				// Window Style
    RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
    WindowRect.left=(long)0;			// Set Left Value To 0
    WindowRect.right=(long)width;		// Set Right Value To Requested Width
    WindowRect.top=(long)0;				// Set Top Value To 0
    WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

    fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

    hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
    wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
    wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
    wc.cbClsExtra		= 0;									// No Extra Window Data
    wc.cbWndExtra		= 0;									// No Extra Window Data
    wc.hInstance		= hInstance;							// Set The Instance
    wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
    wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
    wc.hbrBackground	= NULL;									// No Background Required For GL
    wc.lpszMenuName		= NULL;									// We Don't Want A Menu
    wc.lpszClassName	= L"OpenGL";								// Set The Class Name

    if (!RegisterClass(&wc))									// Attempt To Register The Window Class
    {
        MessageBoxA(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;											// Return FALSE
    }

    if (fullscreen)												// Attempt Fullscreen Mode?
    {
        DEVMODE dmScreenSettings;								// Device Mode
        memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
        dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
        dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
        dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
        dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
        dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

        // Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
        if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
        {
            // If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
            if (MessageBoxA(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
            {
                fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
            }
            else
            {
                // Pop Up A Message Box Letting User Know The Program Is Closing.
                MessageBoxA(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
                return FALSE;									// Return FALSE
            }
        }
    }

    if (fullscreen)												// Are We Still In Fullscreen Mode?
    {
        dwExStyle=WS_EX_APPWINDOW ;								// Window Extended Style
        dwStyle=WS_POPUP;										// Windows Style
        ShowCursor(FALSE);										// Hide Mouse Pointer
    }
    else
    {
        if( topmost )
            dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE | WS_EX_TOPMOST;
        else
            dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
        dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
    }

    AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

    // Create The Window
    if (!(hWnd=CreateWindowExA(	dwExStyle,							// Extended Style For The Window
                                "OpenGL",							// Class Name
                                title,								// Window Title
                                dwStyle |							// Defined Window Style
                                WS_CLIPSIBLINGS |					// Required Window Style
                                WS_CLIPCHILDREN,					// Required Window Style
                                0, 0,								// Window Position
                                WindowRect.right-WindowRect.left,	// Calculate Window Width
                                WindowRect.bottom-WindowRect.top,	// Calculate Window Height
                                NULL,								// No Parent Window
                                NULL,								// No Menu
                                hInstance,							// Instance
                                NULL)))								// Dont Pass Anything To WM_CREATE
    {
        KillGLWindow();								// Reset The Display
        MessageBoxA(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;								// Return FALSE
    }

    handle2window_tab[hWnd] = this;

    static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
    {
        sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
        1,											// Version Number
        PFD_DRAW_TO_WINDOW |						// Format Must Support Window
        PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
        PFD_DOUBLEBUFFER,							// Must Support Double Buffering
        PFD_TYPE_RGBA,								// Request An RGBA Format
        bits,										// Select Our Color Depth
        0, 0, 0, 0, 0, 0,							// Color Bits Ignored
        0,											// No Alpha Buffer
        0,											// Shift Bit Ignored
        0,											// No Accumulation Buffer
        0, 0, 0, 0,									// Accumulation Bits Ignored
        16,											// 16Bit Z-Buffer (Depth Buffer)
        0,											// No Stencil Buffer
        0,											// No Auxiliary Buffer
        PFD_MAIN_PLANE,								// Main Drawing Layer
        0,											// Reserved
        0, 0, 0										// Layer Masks Ignored
    };

    if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
    {
        KillGLWindow();								// Reset The Display
        MessageBoxA(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;								// Return FALSE
    }

    if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
    {
        KillGLWindow();								// Reset The Display
        MessageBoxA(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;								// Return FALSE
    }

    if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
    {
        KillGLWindow();								// Reset The Display
        MessageBoxA(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;								// Return FALSE
    }

    if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
    {
        KillGLWindow();								// Reset The Display
        MessageBoxA(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;								// Return FALSE
    }

    if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
    {
        KillGLWindow();								// Reset The Display
        MessageBoxA(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;								// Return FALSE
    }

    ShowWindow(hWnd,SW_SHOW);						// Show The Window
    SetForegroundWindow(hWnd);						// Slightly Higher Priority
    SetFocus(hWnd);									// Sets Keyboard Focus To The Window
    ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen


    if (!InitGL())									// Initialize Our Newly Created GL Window
    {
        KillGLWindow();								// Reset The Display
        MessageBoxA(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;								// Return FALSE
    }

    return TRUE;									// Success
}

LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
                            UINT	uMsg,			// Message For This Window
                            WPARAM	wParam,			// Additional Message Information
                            LPARAM	lParam)			// Additional Message Information
{
    switch (uMsg)									// Check For Windows Messages
    {
        case WM_ACTIVATE:							// Watch For Window Activate Message
        {
            if (!HIWORD(wParam))					// Check Minimization State
            {
                active=TRUE;						// Program Is Active
            }
            else
            {
                active=FALSE;						// Program Is No Longer Active
            }

            return 0;								// Return To The Message Loop
        }

        case WM_SYSCOMMAND:							// Intercept System Commands
        {
            switch (wParam)							// Check System Calls
            {
                case SC_SCREENSAVE:					// Screensaver Trying To Start?
                case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
                return 0;							// Prevent From Happening
            }
            break;									// Exit
        }

        case WM_CLOSE:								// Did We Receive A Close Message?
        {
            PostQuitMessage(0);						// Send A Quit Message
            return 0;								// Jump Back
        }

        case WM_KEYDOWN:							// Is A Key Being Held Down?
        {
            keys[wParam] = TRUE;					// If So, Mark It As TRUE
            //if(!iinput->bPlayEvents)
            iinput->key_pressed(wParam);
            return 0;								// Jump Back
        }

        case WM_KEYUP:								// Has A Key Been Released?
        {
            keys[wParam] = FALSE;					// If So, Mark It As FALSE
            if(!iinput->bPlayEvents)
                iinput->key_upressed(wParam);
            return 0;								// Jump Back
        }

        case WM_SIZE:								// Resize The OpenGL Window
        {
            ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
            return 0;								// Jump Back
        }
        case WM_MOUSEMOVE:
        {
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);
            if(!iinput->bPlayEvents)
                iinput->mouse_move(xPos,yPos);
            return 0;
        }
        case WM_LBUTTONDOWN:
        {
            flags.bNeedUpdFocus = true;
            if(!iinput->bPlayEvents)
                iinput->click_lmb(1);
            return 0;
        }
        case WM_LBUTTONUP:
        {
            if(!iinput->bPlayEvents)
                iinput->click_lmb(0);
            return 0;
        }
        case WM_RBUTTONDOWN:
        {
            if(!iinput->bPlayEvents)
                iinput->click_rmb(1);
            return 0;
        }
        case WM_RBUTTONUP:
        {
            if(!iinput->bPlayEvents)
                iinput->click_rmb(0);
            return 0;
        }
        case WM_MOUSEWHEEL:
        {
            //int xPos = GET_X_LPARAM(lParam);
            //int yPos = GET_Y_LPARAM(lParam);
            int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
            if(!iinput->bPlayEvents)
                iinput->mouse_wheel(zDelta);
            return 0;
        }

    }

    // Pass All Unhandled Messages To DefWindowProc
    return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WglWidget::mainLoop()
{
    MSG		msg;									// Windows Message Structure

    while(!done)									// Loop That Runs While done=FALSE
    {
        if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
        {
            if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
            {
                done=TRUE;							// If So done=TRUE
            }
            else									// If Not, Deal With Window Messages
            {
                TranslateMessage(&msg);				// Translate The Message
                DispatchMessage(&msg);				// Dispatch The Message
            }
        }
        else										// If There Are No Messages
        {
            // Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
            if ((active && !DrawGLScene()) || keys[VK_ESCAPE])	// Active?  Was There A Quit Received?
            {
                done=TRUE;							// ESC or DrawGLScene Signalled A Quit
            }
            else									// Not Time To Quit, Update Screen
            {
                SwapBuffers(hDC);					// Swap Buffers (Double Buffering)
            }

            if (keys[VK_F1])						// Is F1 Being Pressed?
            {
                keys[VK_F1]=FALSE;					// If So Make Key FALSE
                KillGLWindow();						// Kill Our Current Window
                fullscreen=!fullscreen;				// Toggle Fullscreen / Windowed Mode
                // Recreate Our OpenGL Window
                if (!CreateGLWindow("NeHe's Texture Mapping Tutorial",1280,1024,16,fullscreen,0))
                {
                    return 0;						// Quit If Window Was Not Created
                }
            }
        }
    }


    // Shutdown
    KillGLWindow();
    return 0;
}
