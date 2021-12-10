#ifndef TRANFORM_H
#define TRANFORM_H

#include <vector>

struct TransformOperation
{
    bool    minus;
    float  angle;
    float  x;
    float  y;
    float  z;
    enum {ROTATE, TRANSLATE, SCALE} type;
};



class Transform
{
public:
    std::vector<TransformOperation>  t_stack;
    Transform();
    void AddTranslate(float tx, float ty, float tz);
    void AddTranslateMinus(float tx, float ty, float tz);
    void AddScale(float sx, float sy, float sz);
    void AddRotate(float angle, float axisx, float axisy, float axisz);
    void Apply();
    void Clear();
};

#endif // TRANFORM_H
