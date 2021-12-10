#ifndef __MATER_H__
#define __MATER_H__

#include <string>
#include <QOpenGLShaderProgram>
#include <QtOpenGL>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "object.h"
#include "TGATexture.H"

const int NO_TEXTURE_ID = 0xFFFF666;

struct Material
{
    string type;
    virtual void Apply(QOpenGLShaderProgram& program, QGLFunctions& gl){}
};

struct StandartMaterial : public Material
{
    int     id;
    string  name;
    GLfloat diffuse[4];
    GLfloat ambient[4];
    GLfloat specular[4];
    string  diffuse_map;
    float   U_Tiling;
    float   V_Tiling;
    GLuint  texture_id;
    void Apply(QOpenGLShaderProgram& program, QGLFunctions& gl);
};

struct MultiMaterial : public Material
{
    map<int,StandartMaterial*> id2mat;
    void Apply(QOpenGLShaderProgram& program, QGLFunctions& gl);
    string name;
};

struct MatSet
{
    string name;
    string vert_program;
    string frag_program;
    string texture;
    bool   btextureloaded;
    GLfloat diffuse[4];
    GLfloat ambient[4];
    imageTGA     image;
    unsigned int          id;
    int          width;
    int          height;
    MatSet()
    {
        btextureloaded = false;
    }

};

class CMaterial: public Object
{
    //    Q_OBJECT
	public:
		CMaterial();
        static CMaterial instance;
		MatSet cur;
        string objname;
        QString exe_path;

        vector<imageTGA*>    Textures;
		map<string, MatSet> matmap;
        map<string, Material*> multimatmap;
		vector<string>      matnames;
		string matname;
		
        Material* parse(string matstr);
		void Bind(string name);
		void Unbind(string name);
        void LoadTextures();
        int  GenTexture(float fill_col[4]);
        unsigned int  LoadTexture(string fn, imageSampler sampler);
        bool IsTextureLoaded(string fn);
        unsigned int  GetTexureID(string matname);
        int  FindTexture(string fn);
        string CheckFixExist(string fn);
        static CMaterial& getInstance();
        int print();

		void Declare()
		{
			classname = "CMaterial";
			SetName("matman");
			SET_OBJECT(this,(char*)name.c_str());
			SET_ATTR("matname", (void*)&matname, "selector");
			SET_ATTR("vert_program", (void*)&cur.vert_program, "string");
			SET_ATTR("frag_program", (void*)&cur.frag_program, "string");
			SET_ATTR("texture", (void*)&cur.texture, "string");
			SET_ATTR("textureloaded", (void*)&cur.btextureloaded, "bool");
			SET_ATTR("diffuse",&cur.diffuse,"GLfloat*");
			SET_ATTR("ambient",&cur.ambient,"GLfloat*");
			ei.name = name;
			ei.groupname = "";
            char gstr[255];
			sprintf(gstr,"%d", this);
			ei.GUID = gstr;
			ei.material = "no material";
			ei.model = "no model";	
		}
    void Slot1();
};


#endif
