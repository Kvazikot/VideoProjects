#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions_4_3_Core>

class GlWindow : public QWindow
{
public:
    QOpenGLFunctions_4_3_Core * m_funcs;
    QOpenGLContext*             m_context;
    GlWindow();
};

#endif // GLWINDOW_H
