//#include "ascii3d.h"
#include "../include/camera.h"
#include "../include/move.h"
#include "../include/geometry.h"
#include "../include/grid.h"
#include "../include/kirpich.h"
//#include "material.h"
#include "../include/mesh.h"
#include "../include/curve.h"
#include "../include/move.h"
//#include "primirives.h"
#include "../include/vector3d.h"
#include "../include/vehicle.h"
//#include "ball.h"
//#include "points_array.h"
#include "../include/vpnt.h"
#include "../include/group.h"
#include "../include/curve.h"
#include "../include/plane.h"
#include "../include/maxcurve.h"
#include "../include/terrain.h"
#include "../include/animation.h"
#include "animsprite.h"
#include "biped.h"
#include "../include/graviball.h"
#include "../include/spherevehiclecolision.h"
#include "../include/mterrain.h"
#include "geometrictests.h"
#include "../include/car_autopilot.h"
#include "../include/shared_data.h"
//#include "trajectoryLA.h"
//#include "terrain.h"


CPlane* asCPlane(Object* o){
	return (CPlane*)(o);
}
//TrajectoryLA* asTrajectoryLA(Object* o){return (TrajectoryLA*)(o);}
//Terrain* asTerrain(Object* o){return (Terrain*)(o);}
CVehicle* asCVehicle(Object* o){return (CVehicle*)(o);}
CMovement* asCMovement(Object* o){return (CMovement*)(o);}
//CMaterial* asCMaterial(Object* o){return (CMaterial*)(o);}
//CLight* asCLight(Object* o){return (CLight*)(o);}
Kirpich* asKirpich(Object* o){return (Kirpich*)(o);}
Grid* asGrid(Object* o){return (Grid*)(o);}
RenderObject* asRenderObject(Object* o){return (RenderObject*)(o);}
Object* asObject(Object* o){return (Object*)(o);}
Camera* asCamera(Object* o){return (Camera*)(o);}
//HelperObject* asHelperObject(Object* o){return (HelperObject*)(o);}
//ShapeObject* asShapeObject(Object* o){return (ShapeObject*)(o);}
GemObject* asGemObject(Object* o){return (GemObject*)(o);}
//Ball* asBall(Object* o){return (Ball*)(o);}
//PointsArray* asPointsArray(Object* o){return (PointsArray*)(o);}
VPnt* asVPnt(Object* o){return (VPnt*)(o);}
Curve* asCurve(Object* o){return (Curve*)(o);}

void RegisterClasses()
{
    qRegisterMetaType<Camera>("Camera");
    //qRegisterMetaType<CMovement>("CMovement");
    qRegisterMetaType<GemObject>("GemObject");
    qRegisterMetaType<AnimSprite>("AnimSprite");
    qRegisterMetaType<CPlane>("CPlane");
    qRegisterMetaType<Data>("Data");
    qRegisterMetaType<MaxCurve>("MaxCurve");
    qRegisterMetaType<CVehicle>("CVehicle");
    qRegisterMetaType<Curve>("Curve");
    qRegisterMetaType<Grid>("Grid");
    qRegisterMetaType<Group>("Group");
    qRegisterMetaType<Terrain>("Terrain");
    qRegisterMetaType<CMovement>("CMovement");
    qRegisterMetaType<Animation>("Animation");
    qRegisterMetaType<Animations>("Animations");
    qRegisterMetaType<Biped>("Biped");
    //qRegisterMetaType<GraviBall>("GraviBall");
    qRegisterMetaType<SphereVehicleColision>("SphereVehicleColision");
    qRegisterMetaType<MTerrain>("MTerrain");
   // qRegisterMetaType<GeometricTests>("GeometricTests");
    qRegisterMetaType<Kirpich>("Kirpich");
    qRegisterMetaType<CarAutopilot>("CarAutopilot");

}
