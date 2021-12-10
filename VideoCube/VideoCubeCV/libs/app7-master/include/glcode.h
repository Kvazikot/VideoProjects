#ifndef glcodeH
#define glcodeH

#include <QtGui>
#include <QtOpenGL>
#include <QOpenGLShaderProgram>
#include <string>
#include "geometry.h"

#define EulOrdXYZ    1
#define EulOrdXYX    2
#define EulOrdXZY    3
#define EulOrdXZX    4
#define EulOrdYZX    5
#define EulOrdYZY    6
#define EulOrdYXZ    7
#define EulOrdYXY    8
#define EulOrdZXY    9
#define EulOrdZXZ    10
#define EulOrdZYX    11
#define EulOrdZYZ    12

using namespace  std;

#ifndef _WIN32
uint RGB(int r, int g, int b);
#endif
void RGB2GL2(uint CRGB, GLfloat* col);
void RGB2GL(int R, int G, int B, GLfloat* col);
void renderBitmapString(float x,float y,float z,
						float R, float G, float B,
						void *font,	char *string);

void glutPrint2d(float x, float y, void* font, string text, float r, float g, float b, float a);
void glutPrint(float x, float y, float z, void* font, std::string text, float r, float g, float b, float a);

void calc_select_line(int mouse_x, int mouse_y, Pnt& p1, Pnt& p2);

void draw_navp(string name, Pnt& p, unsigned int rgb, bool bBounds, float R);
void draw_p_gl(QOpenGLShaderProgram& program, string name, Pnt p, GLfloat cl[4], bool bBounds, float R);
void draw_p(QOpenGLShaderProgram& program, string name, Pnt p, uint rgb, bool bBounds);
void draw_pts(QOpenGLShaderProgram& program, vector<Pnt>& pts, string namepref, uint rgb, bool bBounds);
void unset_col();
void set_col(uint rgb);
void drawStrelka( float x, float y, float len, float w, float percent, float alfa, uint rgb);
void draw_v( tvect v, float percent, uint rgb);
void create_pyr(Pnt* p, float R, float H);
inline void draw_line(Pnt& p1, Pnt& p2);
void draw_kurs(char* label, Pnt p, Pnt Ea, char Eord, uint rgb, int len);
template<typename T>
void connect_all_points(T pts, int from, int N )
{
    static map<pair<int,int>,char> mp;
    map<pair<int,int>,char>::iterator it;
    Pnt p1,p2;
    int i,j;

    for(i=from; i<N; i++)
    for(j=from; j<N; j++)
    {
        if(i==j)
            continue;
        if(mp.find(pair<int,int>(i,j) )==mp.end())
        {
            if(mp.find(pair<int,int>(j,i) )==mp.end())
            {
                mp[pair<int,int>(i,j)] = 1;
            }
        }
    }


    for( it = mp.begin(); it!= mp.end(); it++ )
    {
        glBegin(GL_LINES);
        p1 = pts[it->first.first];
        p2 = pts[it->first.second];
        glVertex3f(p1.x, p1.y, p1.z);
        glVertex3f(p2.x, p2.y, p2.z);

        glEnd();
    }


}

#endif
