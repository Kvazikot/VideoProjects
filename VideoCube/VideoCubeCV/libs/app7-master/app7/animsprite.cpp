#include "../include/timer.h"
#include "../include/material.h"
#include "animsprite.h"

// Construct string
// AnimSprite:animsprite01{frame=0 fps=25 frames_in_row=8 total_frames=48 size=5 texturename=textures/explosion/explosion.png }

AnimSprite::AnimSprite()
{

}

AnimSprite::AnimSprite(float size, string texture_name)
    :size(size)
{

}

void AnimSprite::Load(string& s)
{
    LoadFrame(0);
    init();
    //isCanRender = true;
    //load texture
    CMaterial& mater = CMaterial::getInstance();
    texture_id = mater.LoadTexture(texturename, imageSampler());

}

void AnimSprite::LoadFrame(int n)
{
    VertexData v1,v2,v3,v4;
    float dt = 1./frames_in_row;
    float ds = (float)frames_in_row / (float)total_frames;

    int row = n / frames_in_row;
    int column = n % frames_in_row;

    float t = (float)column * dt;
    float s = (float)row * ds;

    v1.position =  QVector3D(size/2,5,-size/2);
    v1.texCoord =  QVector2D(t, s+ds);
    v2.position =  QVector3D(size/2,5,size/2);
    v2.texCoord =  QVector2D(t+dt, s+ds);
    v3.position =  QVector3D(-size/2,5,-size/2);
    v3.texCoord =  QVector2D(t, s);
    v4.position =  QVector3D(-size/2,5,size/2);
    v4.texCoord =  QVector2D(t+dt, s);
    vertices.clear();
    vertices.push_back(v1);    vertices.push_back(v2);
    vertices.push_back(v3);    vertices.push_back(v4);
    indices.clear();
    indices.push_back(2); indices.push_back(1); indices.push_back(0);
    indices.push_back(3); indices.push_back(1); indices.push_back(2);

    t0 = Timer::gtimer.fsec;

}

void AnimSprite::PlayAndHide()
{
    frame = 0;
    isCanRender = true;
}

void AnimSprite::Render(QOpenGLShaderProgram& program)
{
    //change frame
    program.setUniformValue("RenderMode", 2);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    GeometryObject::Render(program);
    if( (Timer::gtimer.fsec - t0) > 1./fps )
    {
        LoadFrame(frame);
        update();
        frame++;
        if( frame > total_frames )
        {
            frame = 0;
            isCanRender = false;
        }
    }
}

void AnimSprite::Slot1()
{

}
