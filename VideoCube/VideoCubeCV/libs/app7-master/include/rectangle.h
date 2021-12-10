#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "renderobject.h"
#include <QOpenGLShaderProgram>
#include <QRectF>

namespace app6
{

    class Rectangle : public RenderObject
    {
    public:
        Rectangle();
        Rectangle(QRectF rect, QVector4D color, float ylevel, float line_width);
        void setRect(QRectF rect){ my_rect = rect;  }
        void Render(QOpenGLShaderProgram& program);
        QVector4D my_color;
        QRectF     my_rect;
        float      ylevel;
        float      line_width;
    };
}

#endif // RECTANGLE_H
