#ifndef GEOMETRICTESTS_H
#define GEOMETRICTESTS_H

#include "../include/renderobject.h"

class GeometricTests : public RenderObject
{
public:
    GeometricTests();
    GeometricTests(const GeometricTests& B ){};
    void Test1();
    void Render();
};

#endif // GEOMETRICTESTS_H
