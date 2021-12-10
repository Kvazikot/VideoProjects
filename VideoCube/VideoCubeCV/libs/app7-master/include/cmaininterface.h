#ifndef CMAININTERFACE_H
#define CMAININTERFACE_H

#include <QObject>
#include "object.h"
#include "cobjregistry.h"

class CMainInterface : public QObject
{
 //   Q_OBJECT
public:
    CMainInterface();
    static CMainInterface instance;
    static CMainInterface& getInstance();

    enum Mode
    {
        MODE_SELECTION,
        MODE_MOVE,
        MODE_GAME,
        MODE_PICK,
        MODE_SCALE,
        MODE_ROTATE
    };

    enum Axis
    {
        X_AXIS,
        Y_AXIS,
        Z_AXIS
    };

    void add_light(int cnt);

    // States:
     bool mayRender;
     bool mayResize;
     //void init();
     void mainloop();


    //TODO: map(Name, Object)
    //map<string,Object&> nametab;
    int		test;
    bool	isInSelectionMode;
    bool	isInGameMode;
    bool	isInMove;
    bool    isInPickPntMode;
    bool    isInRotateMode;
    bool    isInScaleMode;
    bool	firsttime;
    int		mode;
    int     axis;
    int     activeScreen; //1,2,3,4
    string  activeCamera;
    Object*  selobj;

    void SwitchMode(int n);


    CObjRegistry objreg;

    void CreateLuaTables();

    void render();

    //добавить камеру
    void add_main_camera(int x, int y, int z);
    void select_active_camera(int num);
    Object* get_active_camera();
    void add_some_stuff();
    void MousePick();

    //int activeCamera;



signals:

public slots:

};

#endif // CMAININTERFACE_H
