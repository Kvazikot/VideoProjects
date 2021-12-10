#ifndef DRAWINGS2D_H
#define DRAWINGS2D_H

#include <string>
#include "geometry.h"

using namespace std;

void CreatePoint(Pnt p, string name);
void CreatePointGC(Pnt p, string name);
void CreateLine(Pnt p1, Pnt p2, string name);
void CreateLineGC(Pnt p1, Pnt p2, string name);
void CreateLineP(Pnt p0, Pnt V, string name);
//void CreateLine(CVector3 p1, CVector3 p2, string name);
//void CreateLineC(CVector3 p1, CVector3 p2, string name, int r, int g, int b);
//void CreateLineP(CVector3 p0, CVector3 V, string name);
void DeleteLines();
void DeleteLine(string name);
void DeletePoints();


#endif // DRAWINGS2D_H
