#ifndef CAR_AUTOPILOT_H
#define CAR_AUTOPILOT_H

#include <tuple>
#include "vehicle.h"
#include "maxcurve.h"
#include "amimpath.h"
#include "curve.h"
#include "../Samples/HMProcessing/hmprocessing.h"
#include "mterrain.h"
#include "object.h"

// команды управления автопилотом
enum CommandType
{
    MOVE_TO_POINT, GO_TO_POINT, FOLLOW_CURVE
};

struct t_command
{
    CommandType cmd_id;
    VPnt*       destP;
    double      time;
};

//состояние двигателя
enum ENGINE_STATE
{
    REACH_DEST, ACCELERATION, CONST_V, BREAKING, AVOID_COLISION, ENGINE_STOP, NULL_HANDLER
};

// состояние рулевого управления
enum WHEEL_STATE
{
   TURN_LEFT, TURN_RIGHT, TURN_TO_DEST, TURN_BY_CURVE, TURN_BY_PATH, WHEEL_STOP
};

class TurnMap
{
public:
    TurnMap(){}
    int getDistAt(Pnt p);
    QImage heightmap;
    QImage distancemap;
    float    scale;
    Pnt    world_bounds; //122.956596,38.870701,167.845306
};

//------------------ Режимы MODES ------------------------------------------

class ControlPlanItem
{
public:
    QString id;
    double  t_start;
    double  fulltime;
    ControlPlanItem(){}
    ControlPlanItem(QString idd){ id = idd;}
    virtual void Apply(CVehicle* my_veh){}
};

class AccelerationMode : public ControlPlanItem
{
public:
    AccelerationMode() { id = "AccelerationMode"; }
    double a;
};

class AvoidColisionMode : public ControlPlanItem
{
public:
    AvoidColisionMode() { id = "AvoidColisionMode"; }
    double v;
    double tlast_change_kurs;
    double tlast_colision;
    TurnMap* turn_map;
};


class ConstVelocityMode  : public ControlPlanItem
{
public:
    ConstVelocityMode(){ id = "ConstVelocityMode"; }
    double v;
};


class TurnMode  : public ControlPlanItem
{
public:    
    TurnMode(){ id = "TurnMode"; }
    VPnt*  destPnt;
    TurnMap* turn_map;
    bool     bBrakeAtDest;
    double wk; // весовой коэф угловой скорости
    double max_turn_time;
};

class TurnByCurveMode  : public ControlPlanItem
{
public:
    TurnByCurveMode(MaxCurve* c){ delta = 0; dist0 = 0; id = "TurnByCurveMode"; nearest_p = -1; my_curve = c; }
    int nearest_p;
    double    dist0, delta;
    MaxCurve* my_curve;
    double wk; // весовой коэф угловой скорости
};

class TurnByPathMode  : public ControlPlanItem
{
public:
    TurnByPathMode(AmimPath* path) { my_path = path; id = "TurnByPathMode"; }
    AmimPath* my_path;

};


class BrakeMode  : public ControlPlanItem
{
public:
    BrakeMode() { id = "BrakeMode"; }
    double v0;
};

class BrakeAtDestMode  : public ControlPlanItem
{
public:
    BrakeAtDestMode() { id = "BrakeAtDestMode"; }
    VPnt*  destPnt;
    void Apply(CVehicle* my_veh);
};

class ControlPlan
{
public:
    int                 current_state_index;
    int                 current_wstate_index;
    double              startTime;
    vector<ControlPlanItem*>    control_items;
    vector<ControlPlanItem*>    wheel_control_items;
    vector<ENGINE_STATE>        engine_states;
    vector<WHEEL_STATE>         wheel_states;
    ControlPlan(){ current_state_index = -1; current_wstate_index = -1; }
    bool isEmpty(){ return control_items.size() == 0; }
    void AddEngineState(ControlPlanItem* item, ENGINE_STATE state){ control_items.push_back(item); engine_states.push_back(state); }
    void AddWheelState(ControlPlanItem* item, WHEEL_STATE state ) { wheel_control_items.push_back(item); wheel_states.push_back(state);  }
    void Clear(){ control_items.clear(); wheel_states.clear(); engine_states.clear(); wheel_control_items.clear();  current_state_index = -1; current_wstate_index = -1; }
    void SwitchEngineMode(int i){ current_state_index = i;}
    void Print();
    bool isStateIndexOverflow(){ return current_state_index >= control_items.size();  }
    bool isWheelStateIndexOverflow(){ return current_wstate_index >= wheel_control_items.size();  }
    ENGINE_STATE currentEngineState()
    {
        if(current_state_index!=-1)
            return engine_states[current_state_index];
        else
            return ENGINE_STOP;
    }
    void SetEngineIndex(int i)
    {
        current_state_index = control_items.size() + i - 1;
    }
    void SetWheelIndex(int i)
    {
        current_wstate_index = wheel_control_items.size() + i - 1;
    }
    void Dump();
};

class Callback
{
public:
    Callback(){ freq = 1; t_last=0; }
    Callback(float _freq){ freq = _freq; t_last=0; }
    float freq;
    float t_last;
    virtual void run(){}
};

class CarAutopilot : public Object
{
    Q_OBJECT
public:
    int      destIndex;
    bool     isPaused;
    string   points_selector;
    string   systems_status;
    float    kursFixed;
    string   veh_name;
    string   curve_name;
    TurnMap* turn_map;
    CVehicle* my_veh;
    Curve*    destPoints;   // временно агрегируемый обьект
    MaxCurve* maxcurve;
    AmimPath* path;
    double    t0;
    int       low_dist_counter;
    int       rand_sign;
    bool      bAvoidColision;
    double    T1,T2,Ta;

    vector<t_command>   ap_commands;
    vector<Callback*>   callbacks;
    ControlPlan         control_plan;
    ENGINE_STATE        engine_state;
    WHEEL_STATE         wheel_state;
    Curve*              currentPath;
    VPnt*               currentDestPoint; // в пути  currentPath

    CarAutopilot();
    CarAutopilot(const CarAutopilot& B){}
    void MoveToPoint(VPnt*);
    tuple<double, double> get_kurs_delta(Pnt car_pos, Pnt destPnt, double kurs);
    void MakeEngineTransition();
    void MakeWheelTransition();
    void makeControlPlan(VPnt* destP, double fulltime);
    void makeControlPlan2();
    void ControlFunction(ControlPlan* engine_control_plan);
    void AccelerationHandler(AccelerationMode* params);
    void ConstVelocityHandler(ConstVelocityMode* params);
    void TurnHandler(TurnMode* params);
    void TurnByCurveHandler(TurnByCurveMode* params);
    void TurnByPathHandler(TurnByPathMode* params);
    void ColisionModeHandler(AvoidColisionMode* params);
    void BrakeModeHandler(BrakeMode* params);
    void SendTestCommand();
    void PlanAvoidColision();
    void PlanTurnMode(double t_start, double fulltime, VPnt* destP);
    void PlanConstVelocity(double t_start, double fulltime, double v);
    void PlanBrake(double t_start, double fulltime, double v0);
    void makeTurnMap();
    void Slot1();
    void OnAttrChange(t_atr& a);

    void Declare();


};

#endif // CAR_AUTOPILOT_H
