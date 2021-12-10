#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include <QGLShaderProgram>
#include <QOpenGLShaderProgram>
#include <QGLFunctions>
//#include <QOpenGLFunctions_2_1>
#include "object.h"
//====================================================================
// ������ ������� ���������� �������������
//====================================================================
class RenderObject :  public Object
{
//Q_OBJECT
    public:
        RenderObject();
        ~RenderObject();
        //QOpenGLFunctions_2_1* m_f;
        bool  make_on_render;
        bool  is_hidden; // ������� \ �������
        Box bbox;   // �������������� ����
        void         GetGl();
        virtual void Render();
        virtual void Render(QOpenGLShaderProgram& program);
        virtual void MakeBounding(){} // �������� ��������������� ����� ������ �������������
        virtual void Make(){}
        //virtual void OnSelect(){};
};

#endif // RENDEROBJECT_H
