#ifndef SELECTION_H
#define SELECTION_H

#include "object.h"

class CSelection
{
public:


    vector<Object*> objs;
    vector<string> lobjnames;
    string		   objnames;
    CSelection();

    void ResetSelection();
    Object* SelectByVector(Pnt p1, Pnt p2);
    Object* SelectByMouse(float x, float y);
    Object* SelectByMouseTop(double vx, double vy, double vz);
    void SelectByGUID(int guid);
    void SelectByName(char* name);
    void UnSelectByName(char* name);
    void AddObj(Object* obj);
    void RemObj(Object* obj);
    int  Count(){ return objs.size();}
    static CSelection instance;
    static CSelection& getInstance();


    //virtual void mouse_move(int x, int y);
    //virtual void click_lmb(int state);
    //virtual void click_rmb(int state);
    //virtual void click_mmb(int state);
    void key_pressed(int k);


};

#endif
