#include "../include/renderobject.h"


RenderObject::RenderObject()
    :Object()
{
    make_on_render = 0;
    is_hidden = 0;
}

void RenderObject::GetGl()
{

}

RenderObject::~RenderObject()
{

}

void RenderObject::Render()
{

}

void RenderObject::Render(QOpenGLShaderProgram& program)
{

}
