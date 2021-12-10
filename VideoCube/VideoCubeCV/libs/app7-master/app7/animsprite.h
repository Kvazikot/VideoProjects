#ifndef ANIMSPRITE_H
#define ANIMSPRITE_H

#include "qopengl.h"
#include "../include/geometryobject.h"

class AnimSprite :  public GeometryObject
{
public:
    AnimSprite();
    AnimSprite(float size, string texture_name);
    AnimSprite(const AnimSprite& B){}
    float size;
    double t0;
    int  frame;
    int  fps;
    int  frames_in_row;
    int  total_frames;
    GLuint texture_id;
    string texturename;

    void Slot1();
    void PlayAndHide();
    void Declare()
    {
        SetClassName("AnimSprite");
        SetName("explosion_sprite01");
        SET_ATTR("frame",&frame,"int",false);
        SET_ATTR("total_frames",&total_frames,"int",false);
        SET_ATTR("frames_in_row",&frames_in_row,"int",false);
        SET_ATTR("fps",&fps,"int",false);
        SET_ATTR("size",&size,"float",false);
        SET_ATTR("texturename",&texturename,"string",false);

        SET_ATTR("isCanRender",&isCanRender,"bool",false);
    }
    virtual void Load(string& s);
    void NewFrame();
    void LoadFrame(int n);
    void Render(QOpenGLShaderProgram& program);
};

#endif // ANIMSPRITE_H
