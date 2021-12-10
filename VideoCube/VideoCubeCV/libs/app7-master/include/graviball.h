#ifndef GRAVIBALL_H
#define GRAVIBALL_H

#include "player.h"
#include "group.h"
#include "mterrain.h"
#include "ball.h"

class GraviBall : public Group
{
//    Q_OBJECT
public:
    Ball*    ball;
    Player*  my_player;
    MTerrain*   ter;
    GemObject* terrain_mesh_copy;
    GraviBall();
    GraviBall(const GraviBall& B){R = B.R; }
    void SetFPCamera();
    PPnt* GetTerrainPoint(Pnt& p_near_f);
    void Render(QOpenGLShaderProgram& program);
    void DrawWaves();
    int  near_idx; //������ ��������� � ����� �����
    Pnt  Cp;     //����� �����
    Pnt  Tp;     //�������� ������ �� terrain
    double R;    //������ �����
    double ts,te,t_rascheta;

    virtual void Slot1();
};

#endif // GRAVIBALL_H
