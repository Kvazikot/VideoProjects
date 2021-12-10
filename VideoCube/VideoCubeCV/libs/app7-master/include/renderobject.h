#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include <QGLShaderProgram>
#include <QOpenGLShaderProgram>
#include <QGLFunctions>
//#include <QOpenGLFunctions_2_1>
#include "object.h"
//====================================================================
// Обьект имеющий визуальное представление
//====================================================================
class RenderObject :  public Object
{
//Q_OBJECT
    public:
        RenderObject();
        ~RenderObject();
        //QOpenGLFunctions_2_1* m_f;
        bool  make_on_render;
        bool  is_hidden; // спрятан \ показан
        Box bbox;   // ограничивающий бокс
        void         GetGl();
        virtual void Render();
        virtual void Render(QOpenGLShaderProgram& program);
        virtual void MakeBounding(){} // пересчет ограничигоющего обьем фигуры параллепипеда
        virtual void Make(){}
        //virtual void OnSelect(){};
};

#endif // RENDEROBJECT_H
