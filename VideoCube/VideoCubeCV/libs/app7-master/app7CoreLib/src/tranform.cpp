/**********************************************************************
 *  Project App6
 *
 *  transform.cpp
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

#include "../include/tranform.h"
#include "../include/glcode.h"

Transform::Transform()
{

}

void Transform::AddTranslate(float tx, float ty, float tz)
{
    TransformOperation op;
    op.type = TransformOperation::TRANSLATE;
    op.minus = false;
    op.x = tx;
    op.y = ty;
    op.z = tz;
    t_stack.push_back(op);
}

void Transform::AddTranslateMinus(float tx, float ty, float tz)
{
    TransformOperation op;
    op.minus = true;
    op.type = TransformOperation::TRANSLATE;
    op.x = tx;
    op.y = ty;
    op.z = tz;
    t_stack.push_back(op);
}


void Transform::AddScale(float sx, float sy, float sz)
{
    TransformOperation op;
    op.type = TransformOperation::SCALE;
    op.x = sx;
    op.y = sy;
    op.z = sz;
    t_stack.push_back(op);
}

void Transform::AddRotate(float angle, float axisx, float axisy, float axisz)
{
    TransformOperation op;
    op.type = TransformOperation::ROTATE;
    op.x = axisx;
    op.y = axisy;
    op.z = axisz;
    op.angle = angle;
    t_stack.push_back(op);
}

void Transform::Clear()
{
    t_stack.clear();
}

void Transform::Apply()
{
    std::vector<TransformOperation>::iterator it;
    for(it = t_stack.begin(); it!=t_stack.end(); it++)
    {
        switch((*it).type)
        {
            case TransformOperation::ROTATE:
              glRotatef(((*it).angle),((*it).x),((*it).y),((*it).z));
            break;
            case TransformOperation::SCALE:
              glScalef(((*it).x),((*it).y),((*it).z));
            break;
            case TransformOperation::TRANSLATE:
               if((*it).minus)
                   glTranslatef(-((*it).x),-((*it).y),-((*it).z));
               else
                 glTranslatef(((*it).x),((*it).y),((*it).z));
            break;
        }
    }
}

