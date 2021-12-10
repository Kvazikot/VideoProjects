#include "../include/rectangle.h"

namespace app6
{

Rectangle::Rectangle()
{
}

Rectangle::Rectangle(QRectF rect, QVector4D color, float ylevel, float line_width)
    : my_rect(rect), my_color(color), ylevel(ylevel), line_width(line_width)
{
}

void Rectangle::Render(QOpenGLShaderProgram& program)
{
    if(!isCanRender)
        return;

    program.setUniformValue("RenderMode", 0 );
    program.setUniformValue("COLOR", my_color);
    QMatrix4x4 model_matrix;
    model_matrix.setToIdentity();
    model_matrix.translate(my_rect.center().x(), ylevel, my_rect.center().y());

    program.setUniformValue("model_matrix", model_matrix);

    glLineWidth(line_width);
    glBegin(GL_LINE_LOOP);
    glVertex3f(0, ylevel, 0);
    glVertex3f(my_rect.width(), ylevel, 0);
    glVertex3f(my_rect.width(), ylevel, my_rect.width());
    glVertex3f(0, ylevel, my_rect.width());
    glEnd();


}

}
