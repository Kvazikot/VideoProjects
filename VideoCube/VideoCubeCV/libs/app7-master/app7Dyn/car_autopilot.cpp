#include <tuple>
#include "../include/car_autopilot.h"
#include "../include/cobjregistry.h"
#include "../include/selection.h"
#include "../include/vector3d.h"
#include "../include/plot.h"
#include "../include/vpnt.h"
#include "../include/amimpath.h"
#include "../include/print.h"

static CObjRegistry& MREG = CObjRegistry::getInstance2();
string nullstring;
VPnt* vpnt1;

CarAutopilot::CarAutopilot()
{
    engine_state = ENGINE_STOP;
    my_veh = 0;
    destPoints = 0;
    maxcurve = 0;
    path = 0;
    destIndex = 3;
    kursFixed = 0;
    t0 = 0;
    turn_map = 0;
    rand_sign = 1;
    T1=0.5;
    T2=5;
    Ta=2;
    isPaused = false;
    bAvoidColision = false;
    vpnt1 = new VPnt(1,0,0);

    //SendTestCommand();
    MREG.AddObjNew(vpnt1);

    //Declare();
}

void CarAutopilot::Declare()
{
    classname = "CarAutopilot";
    if(name =="")
      SetName("autopilot");
    SET_OBJECT(this,(char*)name.c_str());
    SET_ATTR("SendTestCommand",&nullstring,"func", 0);
    SET_ATTR("OnStart",&nullstring,"func", 0);
    SET_ATTR("Pause",&nullstring,"func", 0);
    SET_ATTR("Reset",&nullstring,"func", 0);
    SET_ATTR("Dump",&nullstring,"func", 0);
    SET_ATTR("makeTurnMap",&nullstring,"func", 0);
    //SET_ATTR("kursFixed",&kursFixed,"float", 0);
    SET_ATTR("T1",&T1,"float", 0);
    SET_ATTR("T2",&T2,"float", 0);
    SET_ATTR("Ta",&Ta,"float", 0);
    SET_ATTR("veh_name",&veh_name,"string", 0);
    SET_ATTR("curve_name",&curve_name,"string", 0);
    SET_ATTR("PointSelector", (void*)&points_selector, "selector"); atrs.back().NotSaving();
    SET_ATTR("systems_status",&systems_status,"string",0); atrs.back().NotSaving();
    SET_STATE("wheel_state", "TURN_LEFT TURN_RIGHT TURN_TO_DEST TURN_BY_CURVE TURN_BY_PATH WHEEL_STOP", &wheel_state); atrs.back().NotSaving();
    SET_STATE("engine_state", "REACH_DEST ACCELERATION CONST_V BREAKING AVOID_COLISION ENGINE_STOP NULL_HANDLER", &engine_state); atrs.back().NotSaving();


}

void CarAutopilot::MakeEngineTransition()
{
      int i = control_plan.current_state_index;

     // if( control_plan.control_states.size() < i ) return;

      if(i < control_plan.engine_states.size()-1 )
      {
          if(control_plan.engine_states[i] == ACCELERATION && control_plan.engine_states[ i+1 ] == CONST_V )
          {
              AccelerationMode* accel_mode = (AccelerationMode*)control_plan.control_items[i];
              ConstVelocityMode* constv_mode = (ConstVelocityMode*)control_plan.control_items[i+1];
              constv_mode->v = my_veh->speed;
          }

          if(control_plan.engine_states[ i] == CONST_V && control_plan.engine_states[ i+1 ] == BREAKING )
          {
              BrakeMode* break_mode = (BrakeMode*)control_plan.control_items[ i+1 ];
              break_mode->v0 = my_veh->speed;
          }
      }

      if(control_plan.engine_states[ i] == BREAKING )
      {
          engine_state = ENGINE_STOP;
      }


     control_plan.current_state_index++;
     //if(engine_state == ENGINE_STOP)
     //    control_plan.current_state_index = -1;
}

void CarAutopilot::MakeWheelTransition()
{

    control_plan.current_wstate_index++;

   // if(wheel_state == WHEEL_STOP)
   //     control_plan.current_wstate_index = -1;
}

void CarAutopilot::AccelerationHandler(AccelerationMode* params)
{
    double t = Timer::gtimer.fsec - params->t_start;
    double v1 = params->a * t;
    my_veh->speed = v1;

    //prn("AccelerationHandler speed = %f t = %f", my_veh->speed, t);
}

void CarAutopilot::ConstVelocityHandler(ConstVelocityMode* params)
{
    double t = Timer::gtimer.fsec - params->t_start;
    my_veh->speed = params->v;

    //prn("ConstVelocityHandler v1 = %f t=%f", my_veh->speed, t);
}

float clamp_kurs(float a)
{
    if(a > 360)
    {
        return a - 360;
    }
    if(a < 0)
    {
        return (a + 360);
    }

    return a;
}

float clamp_kurs2(float a)
{
    if(a > 360)
    {
        return 360;
    }
    if(a < -360)
    {
        return -360;
    }

    return a;
}


tuple<double, double> CarAutopilot::get_kurs_delta(Pnt car_pos, Pnt destPnt, double kurs)
{
    CVector3 A(car_pos.x, 0, car_pos.z);
    CVector3 onDest;
    onDest = CVector3((destPnt)) - A;
    onDest.normalize();
    float kurs2dest  = clamp_kurs(atan2(onDest.z ,onDest.x) * 180/M_PI+180);
    float delta = kurs - kurs2dest;
    //prn("veh_kurs = %f kurs2dest = %f delta = %f ",kurs1, kurs2dest,  delta );
    return make_tuple(delta, kurs2dest);
}

void CarAutopilot::PlanTurnMode(double t_start, double fulltime, VPnt* destP)
{
    // поворот на точку назначения
    TurnMode* turn_mode = new TurnMode();
    turn_mode->turn_map = turn_map;
    turn_mode->t_start = t_start;
    turn_mode->wk = 2;
    turn_mode->max_turn_time = 5;
    turn_mode->fulltime = fulltime;
    turn_mode->destPnt = destP;
    turn_mode->destPnt->y = 0;
    turn_mode->destPnt->isSelected = 2;
    control_plan.AddWheelState(turn_mode, TURN_TO_DEST);
}

void CarAutopilot::PlanConstVelocity(double t_start, double fulltime, double v)
{
    // постоянная скорость
    ConstVelocityMode* constv_mode = new ConstVelocityMode();
    constv_mode->t_start = t_start;
    constv_mode->v = v;
    constv_mode->fulltime = fulltime;
    control_plan.AddEngineState(constv_mode, CONST_V);
}

Pnt get_point(Pnt in, double speed, double kurs)
{
    Pnt out;
    out.z  = in.z - speed*sin(GR * kurs);
    out.x =  in.x - speed*cos(GR * kurs);
    return out;
}

vector< tuple<double, double, double, Pnt> > get_future_points(double dist, double speed, Pnt pos, double kurs, TurnMap* turn_map, int Np)
{
    vector< tuple<double, double, double, Pnt> > out;
    double k = kurs;
    for(int i=0; i < Np; i++)
    {
        Pnt p = get_point(pos, speed, k);
        double d = turn_map->getDistAt( p );
        out.push_back( make_tuple(k, fabs(d - dist), d, p) );
        k+=2;
    }
    k = kurs;
    for(int i=0; i < Np; i++)
    {
        Pnt p = get_point(pos, speed, k);
        double d = turn_map->getDistAt( p );
        out.push_back( make_tuple(k, fabs(d - dist), d, p) );
        k-=2;
    }

    return out;
}


void CarAutopilot::ColisionModeHandler(AvoidColisionMode* params)
{
    double t = Timer::gtimer.fsec - params->t_start;

    if ( t > T1 )
    {
        my_veh->speed = params->v;

        double dist = params->turn_map->getDistAt(my_veh->ei.coord);
        //----------- kurs   gradient  kurs_delta   dist -------------
        vector< tuple<double, double, double, Pnt> > out;
        out = get_future_points(dist, my_veh->speed, my_veh->ei.coord, my_veh->kurs.x, params->turn_map, 30);
        sort(out.begin(),out.end(),
             [](const tuple<double, double, double, Pnt>& a,
             const tuple<double, double, double, Pnt>& b) -> bool
        {
            return std::get<1>(a) < std::get<1>(b);
        });

        my_veh->kurs.x = get<0>(out[0]);  + rand()%10;
        my_veh->kurs.x = clamp_kurs(my_veh->kurs.x);

    }
    else
        my_veh->speed = -params->v;

    t = Timer::gtimer.fsec - params->tlast_colision;
    if( t > T2 )
    {
        PlanTurnMode(Timer::gtimer.fsec, 1200, destPoints->pts[destIndex]);
        PlanConstVelocity(Timer::gtimer.fsec, 1200, 5);
        control_plan.SetEngineIndex(-1);
        control_plan.SetWheelIndex(-1);
        prn("time since last colision %f", t);
    }

    if( my_veh->hasColision() )
      params->tlast_colision =  Timer::gtimer.fsec;

}

void CarAutopilot::PlanBrake(double t_start, double fulltime, double v0)
{
    BrakeMode* break_mode = new BrakeMode();
    break_mode->t_start =  t_start;
    break_mode->fulltime =  fulltime;
    break_mode->v0 = v0;
    control_plan.AddEngineState(break_mode, BREAKING);
    control_plan.AddEngineState(break_mode, ENGINE_STOP);
}

void CarAutopilot::TurnByPathHandler(TurnByPathMode* params)
{
    my_veh->ei.coord = params->my_path->GetPosition();
    my_veh->kurs.x = clamp_kurs(params->my_path->GetOrientation());
}

void CarAutopilot::TurnByCurveHandler(TurnByCurveMode* params)
{

    std::tuple<int ,float, Pnt> out;
    //Pnt p_forward = get_point(my_veh->ei.coord, my_veh->speed * 2, my_veh->kurs.x);
    out = params->my_curve->FindNearestTo(params->nearest_p, my_veh->ei.coord, 500);
    double d = rasst(my_veh->ei.coord,  get<2>(out));
    //tuple<double, double> out2 = get_kurs_delta(my_veh->ei.coord, get<2>(out), my_veh->kurs.x);
    //float delta = get<0>(out2);
    params->nearest_p = get<0>(out);
    //prn("TurnByCurveHandler nearest_p=%d", params->nearest_p);
    *(Pnt*)vpnt1 = params->my_curve->getP(params->nearest_p);
    my_veh->kurs.x = clamp_kurs(get<1>(out));
    //my_veh->ei.coord = get<2>(out);
    //if( d > 2 )
    {
        Pnt p_forward1 = get_point(my_veh->ei.coord, my_veh->speed, my_veh->kurs.x-1);
        Pnt p_forward2 = get_point(my_veh->ei.coord, my_veh->speed, my_veh->kurs.x+1);
        out = params->my_curve->FindNearestTo(params->nearest_p, p_forward1, 500);
        Pnt p_forward1_curve = get<2>(out);
        out = params->my_curve->FindNearestTo(params->nearest_p, p_forward2, 500);
        Pnt p_forward2_curve = get<2>(out);
        double d1 = rasst(p_forward1,  p_forward1_curve);
        double d2 = rasst(p_forward2,  p_forward2_curve);
        if( d1 < d2 )
            params->delta=-1;
        else
            params->delta=1;

    }
    //my_veh->kurs.x+=params->delta;
    params->dist0 = d;
}

void CarAutopilot::TurnHandler(TurnMode* params)
{

    // дистанция
    double dist = params->turn_map->getDistAt(my_veh->ei.coord);
    prn_skip("dist=%f", dist);
    if( dist > 1000)
        prn("too big!!");

    // рассмотрим множество будущих точек
    // с курсом   kurs +- 1 5 10 градусов
    // возьмем градиент функции дистанций для этих точек
    // найти минимальный градиент
    //----------- kurs   gradient  kurs_delta   dist -------------
    vector< tuple<double, double, double, Pnt> > out, out2;
    out = get_future_points(dist, my_veh->speed, my_veh->ei.coord, my_veh->kurs.x, params->turn_map, 10);
    //prn_skip("--- tuple < kurs   gradient  kurs_delta >----");
    for(int i=0; i < out.size(); i++)
    {
        get<2>(out[i]) = fabs( get<0>( get_kurs_delta(my_veh->ei.coord, *(Pnt*)params->destPnt, get<0>(out[i])) ) );
        //prn("%f %f %f",get<0>(out[i]),get<1>(out[i]),get<2>(out[i]));
    }

    sort(out.begin(),out.end(),
         [](const tuple<double, double, double, Pnt>& a,
         const tuple<double, double, double, Pnt>& b) -> bool
    {
        return std::get<2>(a) < std::get<2>(b);
    });

    my_veh->kurs.x = get<0>(out[0]);
    //plot(0,0,t0,get<1>(out[0]));
    //plot(1,0,t0,get<2>(out[0]));
    my_veh->kurs.x = clamp_kurs(my_veh->kurs.x);

    if( params->bBrakeAtDest )
    {
        // вышли на точку
        double rasst_to_dest = rasst(my_veh->ei.coord, *(Pnt*) params->destPnt);
        double a = my_veh->speed*my_veh->speed/(2*rasst_to_dest);
        if( rasst_to_dest < 5 && engine_state != BREAKING && engine_state != ENGINE_STOP)
        {
            PlanBrake(Timer::gtimer.fsec, my_veh->speed / a, my_veh->speed);
            control_plan.SetEngineIndex(-1);
            //prn("break_mode->fulltime=%f",break_mode->fulltime);
        }
    }

    t0 = Timer::gtimer.fsec;

}


void CarAutopilot::BrakeModeHandler(BrakeMode* params)
{
    double t = Timer::gtimer.fsec - params->t_start;
    my_veh->speed = (params->v0/params->fulltime) * (params->fulltime - t) ;
    if(my_veh->speed < 0) my_veh->speed = 0;

    //prn("BrakeModeHandler my_veh->speed = %f params->v0=%f params->fulltime=%f t=%f", my_veh->speed, params->v0, params->fulltime, t);
}


void BrakeAtDestMode::Apply(CVehicle* my_veh)
{
    double t = Timer::gtimer.fsec - t_start;
    double k,g,a,s,l;
    k = 0.3;           // коэфф трения
    g = 9.8;
    a = k*g;
    s = rasst(my_veh->ei.coord, *(Pnt*)destPnt);

    l = my_veh->speed * my_veh->speed / (2* k * g);

    if((s - l) < 1 )
    {
       my_veh->speed = a * (fulltime - t) ;
       //prn("BrakeAtDestHandler s=%f l=%f speed=%f", s, l, my_veh->speed);
    }
    else
        fulltime = my_veh->speed / a;

}


int TurnMap::getDistAt(Pnt p)
{
    int x = distancemap.width()/2 - (p.x ) * scale;
    int y = distancemap.height()/2 - (p.z) * scale;
    if(x<0) return 1;
    if(y<0) return 1;
    if(x>distancemap.width()) return 1;
    if(y>distancemap.height()) return 1;
    prn_skip("getDistAt(%d %d)=%d",x,y,qBlue(distancemap.pixel(x, y)));
    return qBlue(distancemap.pixel(x, y));
}

void CarAutopilot::makeTurnMap()
{
    turn_map = new TurnMap();

    //апдейт карты дистанции для движущихся обьектов
    MTerrain* terrain = (MTerrain*) MREG.FindObj("maze01");
    if(terrain!=0)
    {
        turn_map->world_bounds = Pnt(terrain->mprops.sizex,terrain->mprops.sizey,terrain->mprops.sizez);

        QImage height_map;
        height_map.load("maze.png");
        QPainter p(&height_map);
        QPen pen;
        pen.setWidth(30);
        pen.setColor(Qt::black);
        p.setPen(pen);
        p.drawRect(height_map.rect());
        //height_map.save("maze.png");
        HMProcessing hmp;
        hmp.processHM(height_map, 100);
        height_map.save("distance.png");
        turn_map->distancemap = height_map;
        turn_map->scale = (float)turn_map->distancemap.width()/(float)terrain->mprops.sizex;
        hmp.amplify(height_map);
        terrain->bump_texture = height_map;

    }
}


void CarAutopilot::makeControlPlan(VPnt* destP, double fulltime)
{
    fulltime = 1200;

    control_plan.Clear();

    control_plan.startTime = Timer::gtimer.fsec;

    // разгон занимает 1/6 от всего времени
    AccelerationMode* accel_mode = new AccelerationMode();
    accel_mode->a = 2;
    accel_mode->fulltime = Ta;
    accel_mode->t_start = Timer::gtimer.fsec;
    control_plan.AddEngineState(accel_mode, ACCELERATION);

    // постоянная скорость
    ConstVelocityMode* constv_mode = new ConstVelocityMode();
    constv_mode->t_start = accel_mode->t_start + accel_mode->fulltime;
    constv_mode->fulltime = fulltime;
    control_plan.AddEngineState(constv_mode, CONST_V);

    //
    makeTurnMap();


    // поворот на точку назначения
    TurnMode* turn_mode = new TurnMode();
    turn_mode->turn_map = turn_map;
    turn_mode->t_start = constv_mode->t_start;
    turn_mode->wk = 4;
    turn_mode->bBrakeAtDest = true;
    turn_mode->max_turn_time = 5;
    turn_mode->fulltime = fulltime;
    turn_mode->destPnt = destP;
    turn_mode->destPnt->y = 0;
    turn_mode->destPnt->isSelected = 2;
    control_plan.AddWheelState(turn_mode, TURN_TO_DEST);

    // торможение занимает 1/6 всего времени
    /*BrakeMode* break_mode = new BrakeMode();
    break_mode->t_start = accel_mode->t_start + accel_mode->fulltime + constv_mode->fulltime;
    break_mode->fulltime = fulltime/6.;
    control_plan.AddEngineState(break_mode, BREAKING);
    */


    /*BrakeAtDestMode* break_at_dest = new BrakeAtDestMode();
    break_at_dest->t_start =  Timer::gtimer.fsec;
    break_at_dest->destPnt = activeTraj->pts[destIndex];
    break_at_dest->destPnt->y = 0;
    break_at_dest->fulltime = 3;
    control_plan.AddAutomaticItem(break_at_dest);
    */

    //control_plan.AddEngineState(new ControlPlanItem(), ENGINE_STOP);
    //control_plan.AddWheelState(new ControlPlanItem("Wheel_stop"), WHEEL_STOP);
    control_plan.current_state_index = 0;
    control_plan.current_wstate_index = 0;

}

void CarAutopilot::makeControlPlan2()
{
    double fulltime = 1200;

    control_plan.Clear();

    control_plan.startTime = Timer::gtimer.fsec;

    // разгон занимает 1/6 от всего времени
    AccelerationMode* accel_mode = new AccelerationMode();
    accel_mode->a = 2;
    accel_mode->fulltime = Ta;
    accel_mode->t_start = Timer::gtimer.fsec;
    control_plan.AddEngineState(accel_mode, ACCELERATION);

    // постоянная скорость
    ConstVelocityMode* constv_mode = new ConstVelocityMode();
    constv_mode->t_start = accel_mode->t_start + accel_mode->fulltime;
    constv_mode->fulltime = fulltime;
    control_plan.AddEngineState(constv_mode, NULL_HANDLER);

    // найти ближайшую точку на кривой
    //std::tuple<int ,float, Pnt> out;
    //out = maxcurve->FindNearestTo(-1, my_veh->ei.coord, 500);

    //поворачивать по курве
    /*TurnByCurveMode* turn_mode2 = new TurnByCurveMode( maxcurve );
    turn_mode2->t_start = constv_mode->t_start;
    turn_mode2->fulltime = fulltime;
    control_plan.AddWheelState(turn_mode2, TURN_BY_CURVE);
    */

    TurnByPathMode* turn_mode = new TurnByPathMode( path );
    path->SetFulltime(rand()%50);
    //path->Reset();
    turn_mode->t_start = constv_mode->t_start;
    turn_mode->fulltime = fulltime;
    control_plan.AddWheelState(turn_mode, TURN_BY_PATH);

    control_plan.current_state_index = 0;
    control_plan.current_wstate_index = 0;

}

bool times_equal(double t1, double t2, double epsilon )
{
    if( fabs(t1 - t2) < epsilon)
        return true;
    else
        return false;
}

void CarAutopilot::ControlFunction(ControlPlan* control_plan)
{
    int i = control_plan->current_state_index;
    if( i!=-1 && !control_plan->isStateIndexOverflow() )
    {
        // обработка режимов плана управления
        ControlPlanItem* engine_item = control_plan->control_items[i];
        engine_state = control_plan->engine_states[i];

        if(! (engine_item->t_start > Timer::gtimer.fsec) )
        {
            if( engine_state == ACCELERATION )
                AccelerationHandler((AccelerationMode*)engine_item);

            if( engine_state == CONST_V )
                ConstVelocityHandler((ConstVelocityMode*)engine_item);

            if( engine_state == BREAKING )
                BrakeModeHandler((BrakeMode*)engine_item);

            if( engine_state == AVOID_COLISION )
                ColisionModeHandler((AvoidColisionMode*)engine_item);
        }



        if(Timer::gtimer.fsec >= (engine_item->t_start + engine_item->fulltime))
        {
           MakeEngineTransition();
           prn("%s ended at %f", engine_item->id.toStdString().c_str(), Timer::gtimer.fsec - control_plan->startTime);
        }

    }

    int j = control_plan->current_wstate_index;
    if( j!=-1 && !control_plan->isWheelStateIndexOverflow() )
    {
        ControlPlanItem* wheel_item = control_plan->wheel_control_items[j];
        wheel_state = control_plan->wheel_states[j];
        if( !(wheel_item->t_start > Timer::gtimer.fsec)  )
        {
          if( wheel_state == TURN_TO_DEST )
                TurnHandler((TurnMode*)wheel_item);
          if( wheel_state == TURN_BY_CURVE )
              TurnByCurveHandler((TurnByCurveMode*)wheel_item);
          if( wheel_state == TURN_BY_PATH )
              TurnByPathHandler((TurnByPathMode*)wheel_item);
        }
        else
            t0 = Timer::gtimer.fsec;

        if(Timer::gtimer.fsec >= (wheel_item->t_start + wheel_item->fulltime))
        {
          MakeWheelTransition();
          //prn("%s ended at %f", wheel_item->id.toStdString().c_str(), Timer::gtimer.fsec - control_plan->startTime);
        }
    }







}

void CarAutopilot::SendTestCommand()
{
    t_command cmd;
    cmd.cmd_id = GO_TO_POINT;
    cmd.destP = destPoints->pts[destIndex];
    cmd.time = 40;
    ap_commands.push_back(cmd);
    //my_veh->target_wp = *(Pnt*)(cmd.destP);
    //my_veh->ei.coord = *(Pnt*)activeTraj->pts[0];
}

void CarAutopilot::OnAttrChange(t_atr& a)
{
    if(!isLoading &&  a.name == "Dump" )
    {
        control_plan.Dump();
    }
    if(!isLoading &&  a.name == "PointSelector" )
    {
        destIndex = a.data;
        prn("%d",a.data);
        if(destPoints)
        {
            CSelection s = CSelection::getInstance();
            s.ResetSelection();
            s.AddObj(destPoints->pts[destIndex]);
        }

    }
    if( !isLoading && a.name == "SendTestCommand") SendTestCommand();
    if( !isLoading && a.name == "Reset")
    {
        control_plan.Clear();
        engine_state = ENGINE_STOP;
        wheel_state = WHEEL_STOP;
        control_plan.current_state_index = control_plan.control_items.size();
        control_plan.current_wstate_index = control_plan.wheel_control_items.size();
    }
    if( !isLoading && a.name == "OnStart")
    {
        if(destPoints)
        {
            my_veh->ei.coord = *(Pnt*)destPoints->pts[destIndex];
            my_veh->near_idx = -1;
        }
        if(maxcurve)
        {
            my_veh->ei.coord = maxcurve->pts[destIndex];
            my_veh->near_idx = -1;
            std::tuple<int, float, Pnt> out;
            out = maxcurve->FindNearestTo(-1, maxcurve->pts[destIndex], 500);
            my_veh->kurs.x = get<1>(out);
        }
    }
    if( !isLoading && a.name == "Pause") isPaused=!isPaused;
    if( !isLoading && a.name == "kursFixed") my_veh->kurs.x = kursFixed;
    if( !isLoading && a.name == "makeTurnMap") makeTurnMap();
    if( !isLoading) QApplication::processEvents();

    //if( !isLoading && a.name == "RootsTest") BrakeAtDestHandler(new BrakeAtDestMode());
}

void CarAutopilot::PlanAvoidColision()
{
    //немедленно затормозить
    BrakeMode* break_mode = new BrakeMode();
    break_mode->t_start = Timer::gtimer.fsec;
    break_mode->fulltime =  1;
    break_mode->v0 = my_veh->speed;
    control_plan.AddEngineState(break_mode, BREAKING);

    //control_plan.AddEngineState(break_mode, BREAKING);

    //не трогать руль
    ControlPlanItem* wheel_lock = new ControlPlanItem();
    wheel_lock->id = "wheel_lock";
    wheel_lock->t_start = Timer::gtimer.fsec;
    wheel_lock->fulltime = 1;
    control_plan.AddWheelState(wheel_lock, WHEEL_STOP);

    //режим обьезда колизии
    AvoidColisionMode* colision_mode = new AvoidColisionMode();
    colision_mode->turn_map = turn_map;
    colision_mode->v = 7;
    colision_mode->fulltime = 10;
    colision_mode->t_start = Timer::gtimer.fsec + 1;
    colision_mode->tlast_colision = colision_mode->t_start;
    colision_mode->tlast_change_kurs = colision_mode->t_start;
    control_plan.AddEngineState(colision_mode, AVOID_COLISION);


    control_plan.SetEngineIndex(-2);
    control_plan.SetWheelIndex(-1);


}

void ControlPlan::Dump()
{
    prn("=========ControlPlan::Dump() ENGINE ===================");
    for(int i=0; i < control_items.size(); i++)
    {
        prn("[%d] %s t_start=%f fulltime=%f", i, control_items[i]->id.toStdString().c_str(),
                                        control_items[i]->t_start,
                                        control_items[i]->fulltime);

    }
    prn("=========ControlPlan::Dump() WHEEL ===================");
    for(int i=0; i < wheel_control_items.size(); i++)
    {
        prn("[%d] %s t_start=%f fulltime=%f", i, wheel_control_items[i]->id.toStdString().c_str(),
                                        (float)wheel_control_items[i]->t_start,
                                        (float)wheel_control_items[i]->fulltime);

    }
    prn("current_state_index=%d", current_state_index);
    prn("current_wstate_index=%d", current_wstate_index);

}

void CarAutopilot::Slot1()
{
    if( my_veh == 0 )
    {
        my_veh = (CVehicle*)MREG.FindObj(veh_name);
        systems_status+=" veh";
    }
    else
        my_veh->bIsReceiveInput = false;
    if( path == 0 )
    {
        //maxcurve = (MaxCurve*)MREG.FindObj(curve_name);
        //path = new AmimPath(maxcurve, 20);
        Curve* curve = (Curve*)MREG.FindObj(curve_name);
        //path = new AmimPath(curve, 20);
        //MREG.AddObjNew(path);

//        if(maxcurve)
//        {
//            for(int i=0; i < maxcurve->pts.size(); i++)
//                points_selector+= IntToStr(i) + " ";
//        }




        systems_status+=" curve";
    }
    if( destPoints == 0 )
    {
        destPoints = (Curve*)MREG.FindObj("curve01");
        if( destPoints )
        {
            systems_status+=" path";
            for(int i=0; i < destPoints->pts.size(); i++)
                points_selector+= IntToStr(i) + " ";
        }

    }

    if( engine_state == ENGINE_STOP || engine_state == REACH_DEST )
    {
        if( ap_commands.size() > 0)
        {
            t_command cmd = ap_commands.back();
            ap_commands.pop_back();
            if( cmd.cmd_id == GO_TO_POINT )
            {
                makeControlPlan(cmd.destP, cmd.time);
            }
            if( cmd.cmd_id == FOLLOW_CURVE )
            {
                makeControlPlan2();
            }


        }
    }

    if( !isPaused )
      ControlFunction(&control_plan);

    // стены пройти невозможно
    if( turn_map )
    {

        Pnt p2 = my_veh->takePointAtDist(my_veh->bounds.z/2);
        if( turn_map->getDistAt(p2) == 0)
        {
           //return;
           vector< tuple<double, double, double, Pnt> > out, out2;
           out = get_future_points(0, my_veh->bounds.z, my_veh->ei.coord, my_veh->kurs.x, turn_map, 20);
           out2 = get_future_points(0, -my_veh->bounds.z, my_veh->ei.coord, my_veh->kurs.x, turn_map, 20);

           copy(out2.begin(), out2.end(), back_inserter(out));
           sort(out.begin(),out.end(),
                [](const tuple<double, double, double, Pnt>& a,
                const tuple<double, double, double, Pnt>& b) -> bool
           {
               return std::get<2>(a) > std::get<2>(b);
           });



           my_veh->lookpts.clear();
           //передняя полусфера
           //my_veh->lookpts.push_back( make_tuple(std::get<3>(out[0]), std::get<2>(out[0])) );
           //for(int i=0; i < out.size(); i++)
           //    my_veh->lookpts.push_back( make_tuple(std::get<3>(out[i]), std::get<2>(out[i])) );
           //задняя полусфера
           //for(int i=0; i < out2.size(); i++)
           //    my_veh->lookpts.push_back( make_tuple(std::get<3>(out2[i]), std::get<2>(out2[i])) );

           //my_veh->select_pointz[0] = std::get<3>(out[0]);
           //my_veh->select_pointz[1] = std::get<3>(out2[0]);

           //if(max_iterations==0) break;
           my_veh->near_idx = -1;
           my_veh->ei.coord = std::get<3>(out[0]);
           my_veh->kurs.x = std::get<0>(out[0]);

           if( control_plan.currentEngineState() != AVOID_COLISION && control_plan.currentEngineState() != ENGINE_STOP)
           {
               PlanAvoidColision();
           }

        }
    }



    //если колизия включаем режим ухода от коллизии
    if( my_veh->hasColision()  )
    {
        if( control_plan.currentEngineState() != AVOID_COLISION && control_plan.currentEngineState() != ENGINE_STOP)
        {
            PlanAvoidColision();
        }
    }


    /*
    vector<Callback*>::iterator i;
    for(i=callbacks.begin(); i!=callbacks.end(); i++ )
    {
        if( ( Timer::gtimer.fsec - (*i)->t_last ) > 1./(*i)->freq )
        {
            (*i)->run();
            (*i)->t_last = Timer::gtimer.fsec;
        }
    }
    */

}

