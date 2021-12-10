#include <string>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include <QImage>
#include "Wm5Vector4.h"
#include "Wm5Quaternion.h"
#include "Wm5HMatrix.h"
#include "Wm5HPoint.h"
#include "geometry.h"
#include "planet.h"
#include "timer.h"
#include "cobjregistry.h"

static CObjRegistry& MREG = CObjRegistry::getInstance2();

class AnimationPath;

class SolarSystem
{

public:
    double _radiusSpace;
    double _radiusSun;
    double _radiusMercury;
    double _radiusVenus;
    double _radiusEarth;
    double _radiusMoon;
    double _radiusMars;
    double _radiusJupiter;

    double _RorbitMercury;
    double _RorbitVenus;
    double _RorbitEarth;
    double _RorbitMoon;
    double _RorbitMars;
    double _RorbitJupiter;

    double _rotateSpeedSun;
    double _rotateSpeedMercury;
    double _rotateSpeedVenus;
    double _rotateSpeedEarthAndMoon;
    double _rotateSpeedEarth;
    double _rotateSpeedMoon;
    double _rotateSpeedMars;
    double _rotateSpeedJupiter;

    double _tiltEarth;

    std::string _mapSpace;
    std::string _mapSun;
    std::string _mapVenus;
    std::string _mapMercury;
    std::string _mapEarth;
    std::string _mapEarthNight;
    std::string _mapMoon;
    std::string _mapMars;
    std::string _mapJupiter;

    double _rotateSpeedFactor;
    double _RorbitFactor;
    double _radiusFactor;

    SolarSystem()
    {
        _radiusSpace    = 500.0;
        _radiusSun      = 109.0;
        _radiusMercury  = 0.38;
        _radiusVenus    = 0.95;
        _radiusEarth    = 1.0;
        _radiusMoon     = 0.1;
        _radiusMars     = 0.53;
        _radiusJupiter  = 5.0;

        _RorbitMercury  = 11.7;
        _RorbitVenus    = 21.6;
        _RorbitEarth    = 30.0;
        _RorbitMoon     = 1.0;
        _RorbitMars     = 45.0;
        _RorbitJupiter  = 156.0;

                                                // orbital period in days
        _rotateSpeedSun             = 0.0;      // should be 11.97;  // 30.5 average
        _rotateSpeedMercury         = 4.15;     // 87.96
        _rotateSpeedVenus           = 1.62;     // 224.70
        _rotateSpeedEarthAndMoon    = 1.0;      // 365.25
        _rotateSpeedEarth           = 1.0;      //
        _rotateSpeedMoon            = 0.95;     //
        _rotateSpeedMars            = 0.53;     // 686.98
        _rotateSpeedJupiter         = 0.08;     // 4332.71

        _tiltEarth                  = 23.45; // degrees

        std::string path = "textures/";
        _mapSpace       = path + "Images/spacemap2.jpg";
        _mapSun         = path + "SolarSystem/sun256128.jpg";
        _mapMercury     = path + "SolarSystem/mercury256128.jpg";
        _mapVenus       = path + "SolarSystem/venus256128.jpg";
        _mapEarth       = path + "Images/land_shallow_topo_2048.jpg";
        _mapEarthNight  = path + "Images/land_ocean_ice_lights_2048.jpg";
        _mapMoon        = path + "SolarSystem/moon256128.jpg";
        _mapMars        = path + "SolarSystem/mars256128.jpg";
        _mapJupiter     = path + "SolarSystem/jupiter256128.jpg";

        _rotateSpeedFactor = 0.5;
        _RorbitFactor   = 15.0;
        _radiusFactor   = 10.0;
    }

    Wm5::HMatrix* createTranslationAndTilt( double translation, double tilt );
    AnimationPath* createRotation( double orbit, double speed );
    //osg::Geode* createSpace( const std::string& name, const std::string& textureName );
    Planet* createPlanet( double radius, const std::string& name, const Wm5::HPoint& color , const std::string& textureName );
    //osg::Geode* createPlanet( double radius, const std::string& name, const Wm5::HPoint& color , const std::string& textureName1, const std::string& textureName2);
    //osg::Group* createSunLight();

    void rotateSpeedCorrection()
    {
        _rotateSpeedSun             *= _rotateSpeedFactor;
        _rotateSpeedMercury         *= _rotateSpeedFactor;
        _rotateSpeedVenus           *= _rotateSpeedFactor;
        _rotateSpeedEarthAndMoon    *= _rotateSpeedFactor;
        _rotateSpeedEarth           *= _rotateSpeedFactor;
        _rotateSpeedMoon            *= _rotateSpeedFactor;
        _rotateSpeedMars            *= _rotateSpeedFactor;
        _rotateSpeedJupiter         *= _rotateSpeedFactor;

        std::cout << "rotateSpeed corrected by factor " << _rotateSpeedFactor << std::endl;
    }

    void RorbitCorrection()
    {
        _RorbitMercury  *= _RorbitFactor;
        _RorbitVenus    *= _RorbitFactor;
        _RorbitEarth    *= _RorbitFactor;
        _RorbitMoon     *= _RorbitFactor;
        _RorbitMars     *= _RorbitFactor;
        _RorbitJupiter  *= _RorbitFactor;

        std::cout << "Rorbits corrected by factor " << _RorbitFactor << std::endl;
    }

    void radiusCorrection()
    {
        _radiusSpace    *= _radiusFactor;
        //_radiusSun      *= _radiusFactor;
        _radiusMercury  *= _radiusFactor;
        _radiusVenus    *= _radiusFactor;
        _radiusEarth    *= _radiusFactor;
        _radiusMoon     *= _radiusFactor;
        _radiusMars     *= _radiusFactor;
        _radiusJupiter  *= _radiusFactor;

        std::cout << "Radius corrected by factor " << _radiusFactor << std::endl;
    }
    void printParameters();

};  // end SolarSystem

struct PathKey
{
    Wm5::Vector3<double> pos;
    Wm5::Quaternion<double> rot;
    double time;
};


class AnimationPath : public Object
{
public:
    bool                 mLoopMode;
    double               t0,t1,total_time;
    std::vector<PathKey> keys;
    Object*              myNode;
    PathKey              null_key;

    AnimationPath()
    {
        myNode = 0;
    }
    void Play()
    {
        t0 = Timer::gtimer.fsec;
    }
    void setLoopMode(bool mode)    {        mLoopMode = mode;    }
    void setNode(Object* node)    { myNode = node; }
    void insert(double time,Wm5::Vector3<double> pos, Wm5::Quaternion<double> rot)
    {
        PathKey key;
        key.time = time;
        key.pos = pos;
        key.rot = rot;
        keys.push_back(key);
        total_time = time;
    }
    void interp(float t,
                   Wm5::Vector3<double>& pos, Wm5::Quaternion<double>& rot)
    {
        PathKey* from_key = FindPrevKey(t);
        PathKey* to_key = FindNextKey(t);
        double t0 = from_key->time;
        double t1 = to_key->time;
        double tm = t1 - t0;
        double tt = (t - t0) / tm;
        pos = from_key->pos + tt * ( to_key->pos - from_key->pos);
        rot = rot.Slerp(tt, from_key->rot, to_key->rot);
    }
    PathKey* FindPrevKey(double t)
    {
        for(int i=0; i < (int)keys.size()-1; i++)
        {
            if( ( keys[i].time < t ) && ( keys[i+1].time > t ))
                return &keys[i];
        }
        return &null_key;
    }

    PathKey* FindNextKey(double t)
    {
        for(int i=0; i < (int)keys.size()-1; i++)
        {
            if( ( keys[i].time < t ) && ( keys[i+1].time > t ))
                return &keys[i+1];
        }
        return &null_key;
    }
    void Slot1()
    {
        t1 = Timer::gtimer.fsec - t0;
        if(mLoopMode && t1 > total_time)
          t0 = Timer::gtimer.fsec;
        if( myNode == 0 ) return;
        Wm5::Vector3<double> pos;
        Wm5::Quaternion<double> rot;
        interp(t1,pos,rot);
        double angle;
        Wm5::Vector3<double> axis;
//        Wm5::Matrix3<double> basis_mat;
//        basis_mat.MakeEulerXYZ(180*GR,0,0);
//        basis_mat = basis_mat.Inverse();
//        axis = axis * basis_mat;
        rot.ToAxisAngle(axis,angle);
        myNode->ei.coord0 = Pnt(axis.X(),axis.Y(),axis.Z());
        myNode->ei.rot.x = angle / GR;
        myNode->ei.coord = Pnt(-pos.X(),pos.Z(),pos.Y());
    }

};

AnimationPath* createAnimationPath(const Wm5::Vector3<double>& center,float radius,double speed)
{
    // set up the animation path
    AnimationPath* animationPath = new AnimationPath;
    animationPath->setLoopMode(true);

    double looptime = 10.0f;
    int numSamples = 1000;
    float yaw = 0.0f;
    float yaw_delta = -2.0f*(float)M_PI/((float)numSamples-1.0f);
    float roll = 30.0f * GR;


    double time=0.0f;
    double time_delta = looptime/(double)numSamples;
    //speed = speed / (double)numSamples;
    for(int i=0;i<numSamples;++i)
    {
        //osg::Vec3 position(center+osg::Vec3(sinf(yaw)*radius,cosf(yaw)*radius,0.0f));
        Wm5::Vector3<double> position(center+Wm5::Vector3<double>(sinf(yaw)*radius,cosf(yaw)*radius,0.0f) );
        //osg::Quat rotation(osg::Quat(roll,osg::Vec3(0.0,1.0,0.0))*osg::Quat(-(yaw+osg::inDegrees(90.0f)),osg::Vec3(0.0,0.0,1.0)));
        Wm5::Quaternion<double> rotation(Wm5::Quaternion<double>(Wm5::Vector3<double>(0.0,1.0,0.0), roll)*Wm5::Quaternion<double>(Wm5::Vector3<double>(0.0,1.0,0.0),-(yaw+(90.0f*GR))));

        animationPath->insert(time,position,rotation);

        yaw += yaw_delta;
        time += time_delta / speed;
        //printf("t=%lf\n",time);

    }
    animationPath->Play();
    MREG.AddObj(animationPath);
    return animationPath;
}// end createAnimationPath


AnimationPath* SolarSystem::createRotation( double orbit, double speed )
{
    Wm5::Vector3<double> center( 0.0, 0.0, 0.0 );
    float animationLength = 10.0f;
    AnimationPath* animationPath = createAnimationPath( center, orbit, speed );
    return animationPath;
    //rotation->setUpdateCallback( new osg::AnimationPathCallback( animationPath, 0.0f, speed ) );
}

Wm5::HMatrix* SolarSystem::createTranslationAndTilt( double translation, double tilt )
{
  Wm5::HMatrix*  moonPositioned = new Wm5::HMatrix();
  moonPositioned->MakeRotation(Wm5::AVector(0.0f,0.0f,1.0f), tilt*GR );
  moonPositioned->SetColumn(4,Wm5::HPoint( 0.0, translation, 0.0, 1.0));
  return moonPositioned;

}

Planet* SolarSystem::createPlanet( double radius, const std::string& name, const Wm5::HPoint& color , const std::string& textureName)
{
    Planet* planet = new Planet(name, Pnt(0,0,0), radius, color.X(), color.Y(), color.Z());
    planet->SetTexture(textureName);
    MREG.AddObj(planet);
    return planet;
}

QImage* createBillboardImage(const Wm5::Vector4<float>& centerColour, unsigned int size, float power)
{
    Wm5::Vector4<float> backgroundColour = centerColour;
    backgroundColour[3] = 0.0f;

    //osg::Image* image = new osg::Image;
    //image->allocateImage(size,size,1,
    //                     GL_RGBA,GL_UNSIGNED_BYTE);

    QImage* image = new QImage(size,size,QImage::Format_ARGB32);


    float mid = (float(size)-1)*0.5f;
    float div = 2.0f/float(size);
    for(unsigned int r=0;r<size;++r)
    {
        unsigned char* ptr = image->scanLine(r);
        for(unsigned int c=0;c<size;++c)
        {
            float dx = (float(c) - mid)*div;
            float dy = (float(r) - mid)*div;
            float r = powf(1.0f-sqrtf(dx*dx+dy*dy),power);
            if (r<0.0f) r=0.0f;
            Wm5::Vector4<float> color = centerColour*r+backgroundColour*(1.0f-r);
            // color.set(1.0f,1.0f,1.0f,0.5f);
            *ptr++ = (unsigned char)((color[0])*255.0f);
            *ptr++ = (unsigned char)((color[1])*255.0f);
            *ptr++ = (unsigned char)((color[2])*255.0f);
            *ptr++ = (unsigned char)((color[3])*255.0f);
        }
    }
    return image;

    //return osgDB::readImageFile("spot.dds");
}

GLuint textureFromQImage(QImage& image)
{
    GLuint m_id;
    glGenTextures(1,&m_id);
    glBindTexture(GL_TEXTURE_2D,m_id);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
    return m_id;
}

class SunBillboard : public RenderObject
{
public:
    SunBillboard()
    {
         image = createBillboardImage( Wm5::Vector4<float>( 1.0, 1.0, 0, 1.0f), 64, 1.0);
        //image->save("billboard.tga");
        texture_id = -1;
        isCanRender = true;
    }
    void Render()
    {
        if(texture_id == -1)
          texture_id = textureFromQImage(*image);
        glPushMatrix();
        glEnable (GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glClearDepth(1.0f);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glRotatef(90,1,0,0);
        glEnable(GL_TEXTURE_2D);
        glColor4f(1.0,1.0,1.0,1.0);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glBegin(GL_QUAD_STRIP);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-150.0f,0.0f,-150.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-150.0f,0.0f,150.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(150.0f,0.0f,-150.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(150.0f,0.0f,150.0f);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

    }
    QImage* image;
    GLuint texture_id;
};



void CreateSolarSystem()
{
    SolarSystem solarSystem;

    // create the sun
    //Planet* solarSun = solarSystem.createPlanet( solarSystem._radiusSun, "Sun", Wm5::HPoint( 1.0f, 1.0f, 1.0f, 1.0f), solarSystem._mapSun );
    SunBillboard* sunBillboard = new SunBillboard();
    MREG.AddObj(sunBillboard);


    /*
    *********************************************
    **  earthMoonGroup and Transformations
    *********************************************
    */
    // create earth and moon
    Planet* earth = solarSystem.createPlanet( solarSystem._radiusEarth, "Earth", Wm5::HPoint( 1.0f, 1.0f, 1.0f, 1.0f), solarSystem._mapEarth);
    //Planet* moon = solarSystem.createPlanet( solarSystem._radiusMoon, "Moon", Wm5::HPoint( 1.0f, 1.0f, 1.0f, 1.0f), solarSystem._mapMoon );
    earth->setOrbit(solarSystem._RorbitEarth);
    earth->setTilt(solarSystem._tiltEarth);
    // create transformations for the earthMoonGroup
    AnimationPath* aroundSunRotationEarthMoonGroup = solarSystem.createRotation( solarSystem._RorbitEarth, solarSystem._rotateSpeedEarthAndMoon );
    aroundSunRotationEarthMoonGroup->setNode(earth);
    //    Wm5::HMatrix* earthMoonGroupPosition = solarSystem.createTranslationAndTilt( solarSystem._RorbitEarth, solarSystem._tiltEarth );
    // Wm5::HMatrix* earthMoonGroupPosition = solarSystem.createTranslationAndTilt( solarSystem._RorbitEarth, 0.0 );


    //transformation to rotate the earth around itself
    // Wm5::HMatrix* earthAroundItselfRotation = solarSystem.createRotation ( 0.0, solarSystem._rotateSpeedEarth );

    //transformations for the moon
    // Wm5::HMatrix* moonAroundEarthRotation = solarSystem.createRotation( solarSystem._RorbitMoon, solarSystem._rotateSpeedMoon );
    // Wm5::HMatrix* moonTranslation = solarSystem.createTranslationAndTilt( solarSystem._RorbitMoon, 0.0 );



    /*
    *********************************************
    **  Mercury and Transformations
    *********************************************
    */
    Planet* mercury = solarSystem.createPlanet( solarSystem._radiusMercury, "Mercury", Wm5::HPoint( 1.0f, 1.0f, 1.0f, 1.0f ), solarSystem._mapMercury );

    AnimationPath* aroundSunRotationMercury = solarSystem.createRotation( solarSystem._RorbitMercury, solarSystem._rotateSpeedMercury );
    mercury->setOrbit(solarSystem._RorbitMercury);
    aroundSunRotationMercury->setNode(mercury);


    /*
    *********************************************
    **  Venus and Transformations
    *********************************************
    */
    Planet* venus = solarSystem.createPlanet( solarSystem._radiusVenus, "Venus", Wm5::HPoint( 1.0f, 1.0f, 1.0f, 1.0f ), solarSystem._mapVenus );

    AnimationPath* aroundSunRotationVenus = solarSystem.createRotation( solarSystem._RorbitVenus, solarSystem._rotateSpeedVenus );
    venus->setOrbit(solarSystem._RorbitVenus);
    aroundSunRotationVenus->setNode(venus);

    /*
    *********************************************
    **  Mars and Transformations
    *********************************************
    */
    Planet* mars = solarSystem.createPlanet( solarSystem._radiusMars, "Mars", Wm5::HPoint( 1.0f, 1.0f, 1.0f, 1.0f ), solarSystem._mapMars);

    AnimationPath* aroundSunRotationMars = solarSystem.createRotation( solarSystem._RorbitMars, solarSystem._rotateSpeedMars );
    mars->setOrbit(solarSystem._RorbitMars);
    aroundSunRotationMars->setNode(mars);
    /*
    *********************************************
    **  Jupiter and Transformations
    *********************************************
    */
    Planet* jupiter = solarSystem.createPlanet( solarSystem._radiusJupiter, "Jupiter", Wm5::HPoint( 1.0f, 1.0f, 1.0f, 1.0f ), solarSystem._mapJupiter);

    AnimationPath* aroundSunRotationJupiter = solarSystem.createRotation( solarSystem._RorbitJupiter, solarSystem._rotateSpeedJupiter );
    jupiter->setOrbit(solarSystem._RorbitJupiter);
    aroundSunRotationJupiter->setNode(jupiter);


}

