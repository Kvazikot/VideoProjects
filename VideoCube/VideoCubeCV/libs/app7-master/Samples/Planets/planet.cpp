#include "planet.h"

//REALISATION CPP
#include "move.h"
#include "glcode.h"
#include "material.h"
#include "timer.h"

static CMaterial& mater = CMaterial::getInstance();

//====================================================================
// <OBJECT> Ўј– \ физическа€ модель м€ча  </OBJECT>
//====================================================================

Planet::Planet()
{
    name = "Planet01";
    classname = "Planet";
    isCanRender = true;
    isEntety = true;
    RGB2GL2(RGB(255,0,0),color);
    // запрос на апдейт
    C0 = Pnt(0,0,0);
    gentime = Timer::gtimer.msec;
    R = 5;
    Nseg = 20;
    Npts = 20;
    Nparal = 20;
    phi1 = 0;
    algo = "type1 triangles geosphere";
    algon = 0;
    rot = Pnt(0,0,0);
    scale = Pnt(1,1,1);


    bounds.x = R*scale.x;
    bounds.y = R*scale.y;
    bounds.z = R*scale.z;
    mesh = new GemObject();
    Generate();
    bNeedGenerate = false;
    mesh->isCanRender = true;
}


Planet::Planet(string _name, Pnt _C0, float _R,
           float r, float g, float b)
    :R(_R),Nseg(20),Nparal(20),Npts(20)
{
    name = _name;
    classname = "Planet";
    isCanRender = true;
    isEntety = true;
    RGB2GL2(RGB(r,g,b),color);
    C0 = _C0;
    // запрос на апдейт
    gentime = Timer::gtimer.msec;
    phi1 = 0;
    algo = "type1 triangles geosphere";
    algon = 0;
    rot = Pnt(0,0,0);
    scale = Pnt(1,1,1);
    myTilt = -1;
    bounds.x = R*scale.x;
    bounds.y = R*scale.y;
    bounds.z = R*scale.z;
    mesh = new GemObject();
    Generate();
    bNeedGenerate = false;
    mesh->isCanRender = true;
}

void Planet::SetTexture(string textureName)
{
    this->textureName = textureName;
}

void Planet::Generate()
{
    //mu_printf("Planet::Generate()");
    float x,y,z,a=0,b=0;

    if(algon==0)
    {
        mesh->SetNumVert(Npts*Nparal+10);
        float delta_tx = 1.0 / (float)(Npts-1);
        float delta_ty = 1.0 / (float)(Nparal-1);
        float ty = 0.0, tx = 0.0;
        //параллели
        for(int j=0; j<Nparal; j++)
        {
            b+=(180./(float)Nparal) * GR;
            a=0;
            ty+=delta_ty;
            tx=0;
            for(int i=j*Npts; i<j*Npts+Npts; i++)
            {
                a+=(360./(float)Npts) * GR;
                x = R * sin(b)*cos(a);
                z = R * sin(b)*sin(a);
                y = R * cos(b);
                mesh->vertices[3*i]=x;
                mesh->vertices[3*i+1]=y;
                mesh->vertices[3*i+2]=z;
                mesh->texcoords[2*i]=tx;
                mesh->texcoords[2*i+1]=ty;
                tx+=delta_tx;
            }
        }
    }


}

void Planet::Scale(float,float,float)
{

}

void Planet::Rotate(float,float,float)
{

}

void Planet::setOrbit(double orbit)
{
    ei.coord.x = orbit;
}

void Planet::setTilt(double tilt)
{
    myTilt = tilt;
}


void Planet::Render()
{
    Pnt p1[9];
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

     if(!isCanRender)
        return;


    glTranslatef ( ei.coord.x, ei.coord.y, ei.coord.z );
    glRotatef(ei.rot.x, ei.coord0.x, ei.coord0.y, ei.coord0.z);
    if(myTilt!=-1)
      glRotatef(myTilt,1,0,0);


    static float cl=0;
    if(isSelected!=2)
    {
        glEnable(GL_COLOR_MATERIAL);
        glColor4f(0.5,0.5,0.5,1);
        if(rot.x!=0)
            glRotatef(rot.x, 1, 0, 0);
        if(rot.y!=0)
            glRotatef(rot.y, 0, 1, 0);
        if(rot.z!=0)
            glRotatef(rot.z, 0, 0, 1);

        glScalef(scale.x, scale.y, scale.z);

    }
    else
    {

        glPolygonMode( GL_FRONT_AND_BACK, GL_QUADS );
        glMaterialfv ( GL_FRONT_AND_BACK, GL_DIFFUSE, color );


        if(rot.x!=0)
            glRotatef(rot.x, 1, 0, 0);
        if(rot.y!=0)
            glRotatef(rot.y, 0, 1, 0);
        if(rot.z!=0)
            glRotatef(rot.z, 0, 0, 1);

        glScalef(scale.x, scale.y, scale.z);

    }

    texture_id = mater.FindTexture(textureName);
    if( texture_id == 0 )
        texture_id = mater.LoadTexture(textureName);
    if(algon==0)
    {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texture_id);
            for(int i=0; i<Npts; i++)
            {

                for(int j=1; j<Nparal; j++)
                {
                    int k;
                    glBegin(GL_QUAD_STRIP);
                    k = (j*Npts+i);
                    glTexCoord2f(mesh->texcoords[2*k],mesh->texcoords[2*k+1]);
                    glVertex3f(mesh->vertices[3*k],mesh->vertices[3*k+1],mesh->vertices[3*k+2]);
                    k = (j*Npts+i+1);
                    glTexCoord2f(mesh->texcoords[2*k],mesh->texcoords[2*k+1]);
                    glVertex3f(mesh->vertices[3*k],mesh->vertices[3*k+1],mesh->vertices[3*k+2]);
                    k = ((j-1)*Npts+i);
                    glTexCoord2f(mesh->texcoords[2*k],mesh->texcoords[2*k+1]);
                    glVertex3f(mesh->vertices[3*k],mesh->vertices[3*k+1],mesh->vertices[3*k+2]);
                    k = ((j-1)*Npts+i+1);
                    glTexCoord2f(mesh->texcoords[2*k],mesh->texcoords[2*k+1]);
                    glVertex3f(mesh->vertices[3*k],mesh->vertices[3*k+1],mesh->vertices[3*k+2]);
                    glEnd();
                }
            }
            glDisable(GL_TEXTURE_2D);


    }
    if(isSelected!=2)
    {
        //glPointSize(1);
        glDisable(GL_COLOR_MATERIAL);
    }
    glPopMatrix();




}


//object inspector event
void Planet::OnAttrChange(t_atr& a)
{
    if( (a.name == "Nseg") || (a.name == "R")  || (a.name == "Npts") || (a.name == "Nparal"))
    {
        gentime = Timer::gtimer.msec;
        bNeedGenerate = true;
        isCanRender = false;
        Npts = Nseg;
        Nparal = Nseg;
    }

    if (a.name == "algo" )
    {
        algon = a.data;
        bNeedGenerate = true;
    }





}

//obj update slot
void Planet::Slot1()
{
    //если обновились параметры
    if( bNeedGenerate && ( (Timer::gtimer.msec - gentime)> 1000 ) )
    {
        mesh->isCanRender = false;
        Generate();
        mesh->isCanRender = true;
        isCanRender = true;
        bNeedGenerate = false;

    }


    Generate();
    if(m_Updated)
    {
        bounds.x = R*scale.x;
        bounds.y = R*scale.y;
        bounds.z = R*scale.z;
        bNeedGenerate = true;
        //Generate();
        m_Updated = false;
        //bNeedGenerate = false;
        //mu_printf("%s accept new params %s", name.c_str(), bounds.toStr().c_str());
    }

    w = v.x / R;
    f = w / (2 * M_PI);

    phi1 = phi1 + 360 * 0.05 * f;  //50ms * частота
    rot = Pnt(0, 0, phi1);


    if ( phi1 > 360 )
        phi1 = phi1 - 360;

    if ( phi1 < 0 )
        phi1 = phi1 + 360;

    /*
    mov->CalcTrenie(this);
    mov->CalcGravity(this);
    mov->SchislenieCoord(this);
    */


}
