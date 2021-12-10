#ifndef WGLWIDGET_H
#define WGLWIDGET_H

#include <string>
#include <QString>
#include <QWidget>
#include <map>
#include "camera.h"
#include <WinDef.h>


class WglWidget
{
public:
    WglWidget(char* wndName, int width, int height, int bpp, int fullscreen, int topmost);
    int mainLoop();
    void display(void);
    int  DrawGLScene();
    BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag, int topmost);
    static WglWidget* GetWindow(std::string name);
    static QWidget* GetWidget(std::string name);
    QWidget* GetWidget(){ return widget; }
    //static HWND GetWindowHandle(std::string name);
    static WglWidget* GetWindowByHandle(HWND hwnd);
    void KillGLWindow();
    void SetWidget(QWidget* w){ widget = w; }
    HWND        hWnd;
    BOOL        done;
    QWidget*    widget;

protected:
    static std::map<std::string, WglWidget*>  wgl_tab;
    static std::map<HWND, WglWidget*>  handle2window_tab;

    Camera*     cam;            // камера присвоенная этому вьюпорту
    std::string viewport_name;
    QString     cam_name;       // название камеры
    HDC			hDC;		// Private GDI Device Context
    HGLRC		hRC;		// Permanent Rendering Context

};

#endif // WGLWIDGET_H
