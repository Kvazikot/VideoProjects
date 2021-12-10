#include <windows.h>
#include <process.h>
#include <QApplication>
#include "mainwindow.h"
#include "wglwidget.h"
#include "str_utils.h"

int argc;
char* argv[10];

unsigned __stdcall qt_thread(void* arg) {
    arg=arg;
    qApp->addLibraryPath("./plugins");
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}


int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
                    HINSTANCE	hPrevInstance,		// Previous Instance
                    LPSTR		lpCmdLine,			// Command Line Parameters
                    int			nCmdShow)			// Window Show State
{
    hInstance = hPrevInstance;
    nCmdShow = nCmdShow;
    WglWidget* wgl = new WglWidget("planets",640,480,16,0,1);
    //WglWidget* wgl = new WglWidget("app6",1920,1080,16,1);
    _beginthreadex(0,0,qt_thread,lpCmdLine,0,0);
    //qt_thread(0);
    wgl->mainLoop();


}

