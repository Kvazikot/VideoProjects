#include "geometrictests.h"
#include <Wm5Vector3.h>
#include <Wm5Matrix3.h>
#include "glcode.h"
#include "print.h"
#include "matrix.h"

using namespace Wm5;

Vector3<float> A(0,0,3);
Vector3<float> B;
Vector3<float> D(1,1,1);

GeometricTests::GeometricTests()
{
    Test1();
    isCanRender = true;
}

void GeometricTests::Test1()
{
    double beta = atan(D.X()/D.Z());
    double alfa = asin(D.Y()/D.Length());
    Wm5::Matrix3<float> maty;
    Wm5::Matrix3<float> matz;
    maty.MakeEulerXYZ(0,-beta,0);
    matz.MakeEulerXYZ(alfa,0,0);
    B = A * matz;
    B = B * maty;
    prn("B=%f %f %f",B.X(),B.Y(),B.Z());

    /*Vector3<float> axis(D.x,D.y,D.z);
      axis.Normalize();
Wm5::Matrix3<float> mat;
mat.MakeEulerXYZ(D.retXAng(),D.retYAng(),D.retZAng());
BB = mat * AA;
*(Pnt*)A = Pnt(AA.X(),AA.Y(),AA.Z());
*(Pnt*)B = Pnt(BB.X(),BB.Y(),BB.Z());
 */
}


void GeometricTests::Render()
{
}
