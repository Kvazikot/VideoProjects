#ifndef BIPED_H
#define BIPED_H

#include "../include/object.h"
#include "../include/vector3d.h"
#include "../include/gemobject.h"
#include "../include/weapon.h"
#include <map>
#include <QMatrix4x4>

struct BipState
{
    float q[4];
    float pos[3];
};

struct BipedControl
{
    unsigned char ForwardKey;
    unsigned char BackwardKey;
    unsigned char LeftKey;
    unsigned char RightKey;
    bool isControlKey(int k)
    {
        if(k == ForwardKey ||
           k == BackwardKey ||
           k == LeftKey ||
           k == RightKey )
            return true;
        else
            return false;
    }
    void SetControlShema(unsigned char f, unsigned char b,
                                 unsigned char r, unsigned char l)
    {
        ForwardKey = f;
        BackwardKey = b;
        LeftKey = l;
        RightKey = r;
    }


};

class Biped : public Object
{
 //   Q_OBJECT
public:
    Weapon*       weapon;
    BipedControl control;
    int N_frames;
    int n;
    int cur_frame;
    string keys_file;
    string ctrl_objs;
    Pnt pos;
    Pnt rot;
    Pnt C0;
    Pnt delta;
    Pnt delta2;
    float speed, step, scale;
    CVector3 lookAt,lookAt2;

    map<GemObject*,vector<BipState> > states_data;
    map<GemObject*,vector<BipState> > states_aim;
    map<GemObject*,vector<BipState> > states_walk;
    GemObject* head;

    float  kurs;
    double t_sim;
    double _t2, _t1, dt, _freq;
    QElapsedTimer timer;
    bool   isCanPlay;
    bool   isLoop;
    bool   isWalking;
    bool   isWalkingBackwards;
    bool   isStrafeLeft;
    bool   isStrafeRight;
    QMatrix4x4 basis_mat;

    Biped();
    Biped(const Biped& B){ n = B.n; }
    void CreateDefaultTransformStack(GemObject* go);
    void SetControlShema(unsigned char f, unsigned char b,unsigned char r, unsigned char l);
    void Play(int from_frame, int N_frames_to_play, bool loop);
    void FramesLoad(string fn, int N_fr);
    void SimpleAnimate(int cur_frame);
    void DoShot();
    void Load(string& s);
    void CreateCopy();
    void Slot1();
    void ProcessInput();
    void SetFPCamera();
    void key_pressed(int k);
    void key_upressed(int k);
    void click_lmb(int state);

    void Declare()
    {
        if(name.size()==0)
            name = "biped01";
        classname = "Biped";
        //SET_ATTR("isReceiveInput", &isReceiveInput, "bool");        
        SET_ATTR("cur_frame", &cur_frame, "int");
        SET_ATTR("N_frames", &N_frames, "int");
        SET_ATTR("n", &n, "int");
        SET_ATTR("pos", &pos, "Pnt");
        SET_ATTR("C0", &C0, "Pnt");
        SET_ATTR("rot", &rot, "Pnt");
        SET_ATTR("keys_file",&keys_file,"string");
        SET_ATTR("ctrl_objs",&ctrl_objs,"string");
        SET_ATTR("speed",&speed,"float");
        SET_ATTR("delta2",&delta2,"Pnt");
        SET_ATTR("delta",&delta,"Pnt");

    }
};

#endif // BIPED_H
