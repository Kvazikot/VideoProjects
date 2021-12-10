/**********************************************************************
 *  Project App6
 *
 *  material.cpp
 *
 *  Author: Vladimir Baranov
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
**********************************************************************/

#include <QtOpenGL>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <QFile>
#include <QDir>
#include <QString>
#include <QMap>
#include "../include/TGATexture.H"
#include "../include/material.h"
#include "../include/var_parse.h"
#include "../include/print.h"


QMap<QString,QString> checkedDirs;
bool bAbort = false;
QString resultFn;

CMaterial CMaterial::instance;

CMaterial& CMaterial::getInstance()
{
    return instance;
}

CMaterial::CMaterial()
{
    string mname;

    isNotSaving	= true;

    matnames.push_back("brick0");
    matmap["brick0"].name = "brick0";
    matmap["brick0"].vert_program = "shaders/metal.glsl";
    matmap["brick0"].frag_program = "shaders/metal.glsl";
    matmap["brick0"].texture = "textures/brick/kirpich.jpg";
    GLfloat col[4] = {0.0, 0.0, 0.0, 0.0};
    memcpy(&matmap["brick0"].diffuse,&col,sizeof(col));

    matnames.push_back("metal");
    matmap["metal"].name = "metal";
    matmap["metal"].vert_program = "shaders/road.glsl";
    matmap["metal"].frag_program = "shaders/road.glsl";
    matmap["metal"].texture = "textures/medal_accuracy.tga";
    GLfloat col2[4] = {1.0, 0.0, 1.0, 0.0};
    memcpy(&matmap["metal"].diffuse,&col2,sizeof(col));


    matnames.push_back("road");
    matmap["road"].name = "road";
    matmap["road"].vert_program = "shaders/road.glsl";
    matmap["road"].frag_program = "shaders/road.glsl";
    matmap["road"].texture = "textures/ROAD1.tga";
    memcpy(&matmap["road"].diffuse,&col2,sizeof(col));

    mname = "test";
    matnames.push_back(mname);
    matmap[mname].name = mname;
    matmap[mname].vert_program = "shaders/checkboard.vert.glsl";
    matmap[mname].frag_program = "shaders/checkboard.glsl";
    matmap[mname].texture = "textures/test.tga";
    GLfloat col3[4] = {1.0, 0.0, 1.0, 0.0};
    memcpy(&matmap[mname].diffuse,&col3,sizeof(col));

    exe_path = QDir::current().absolutePath();

    isCanRender = false;

    cur = matmap["metal"];

    for(int i=0; i<(int)matnames.size(); i++)
        matname = matname + matnames[i]+" ";

    Connect();
}
//"{type=multi name=w0_mso {id=1 type=bitmap name=inner_rim file=G:\3d_scenes\scenes\models2\CARS\LOTUS\wheel0.tga U_Tiling=1.0 V_Tiling=1.0 amient=0.0,0.0,0.0 diffuse=204.0,204.0,204.0 specular=127.5,127.5,127.5} {id=2 type=bitmap name=rim file=G:\3d_scenes\scenes\models2\CARS\LOTUS\wheel.tga U_Tiling=1.0 V_Tiling=1.0 amient=127.5,127.5,127.5 diffuse=127.5,127.5,127.5 specular=229.5,229.5,229.5} {id=3 type=bitmap name=tire file=G:\3d_scenes\scenes\models2\CARS\LOTUS\tirea0.tga U_Tiling=1.0 V_Tiling=1.0 amient=0.0,0.0,0.0 diffuse=204.0,204.0,204.0 specular=127.5,127.5,127.5} }"
Material* CMaterial::parse(string matstr)
{
    //VarParser vp;
    //string col_s = vp.ParseIntArray();
    VarParser vp;
    MyTable*   table = new MyTable();
    vp.ParseTable(matstr, table);
    MyTable*   mat = table->first_child();
    if( mat->get_prop("type") == "multi" )
    {
        MultiMaterial* multi = new MultiMaterial();
        multi->name = mat->get_prop("name");
        multi->type = mat->get_prop("type");
        multimatmap[multi->name] = multi;
        for(int i=0; i < mat->children.size(); i++)
        {
            MyTable* mat2 = mat->children[i];
            if( mat2->get_prop("type") == "standart" )
            {
                StandartMaterial* standart = new StandartMaterial();
                standart->id = StrToInt(mat2->get_prop("id"));
                standart->type = mat2->get_prop("type");
                standart->name = mat2->get_prop("name");
                multi->id2mat[standart->id] = standart;
                standart->diffuse_map = mat2->get_prop("file");
                standart->texture_id = LoadTexture(standart->diffuse_map, imageSampler());
                if(standart->texture_id == NO_TEXTURE_ID)
                {
                    imageTGA* im = new imageTGA(standart->diffuse);
                    im->filename = standart->diffuse_map;
                    standart->texture_id = im->getId();
                    Textures.push_back(im);
                }
                vp.ParseColor(mat2->get_prop("ambient"), standart->ambient);
                vp.ParseColor(mat2->get_prop("diffuse"), standart->diffuse);
                vp.ParseColor(mat2->get_prop("specular"), standart->specular);
            }

        }
        return multi;
    }
    if( mat->get_prop("type") == "bitmap" )
    {
        StandartMaterial* standart = new StandartMaterial();
        standart->diffuse_map = mat->get_prop("file");
        standart->U_Tiling = StrToDouble(mat->get_prop("U_Tiling"));
        standart->V_Tiling = StrToDouble(mat->get_prop("V_Tiling"));
        standart->texture_id = LoadTexture(standart->diffuse_map, imageSampler(GL_NEAREST_MIPMAP_NEAREST,GL_NEAREST_MIPMAP_NEAREST,GL_REPEAT,GL_REPEAT));
        if(standart->texture_id == NO_TEXTURE_ID)
        {
            imageTGA* im = new imageTGA(standart->diffuse);
            im->filename = standart->diffuse_map;
            standart->texture_id = im->getId();
            Textures.push_back(im);
        }
        return standart;

    }

    if( mat->get_prop("type") == "color" )
    {
        StandartMaterial* standart = new StandartMaterial();
        vp.ParseColor(mat->get_prop("wire_color"), standart->ambient);
        vp.ParseColor(mat->get_prop("wire_color"), standart->diffuse);
        standart->U_Tiling = StrToDouble(mat->get_prop("U_Tiling"));
        standart->V_Tiling = StrToDouble(mat->get_prop("V_Tiling"));
        imageTGA* im = new imageTGA(standart->diffuse);
        standart->texture_id = im->getId();
        return standart;

    }

    return new Material();

}

void StandartMaterial::Apply(QOpenGLShaderProgram& program, QGLFunctions& gl)
{
    program.setUniformValue("RenderMode", (GLuint)2);
    program.setUniformValue("texture", 0);
    program.setUniformValue("U_Tiling", U_Tiling);
    program.setUniformValue("V_Tiling", V_Tiling);
    gl.glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);

}

void MultiMaterial::Apply(QOpenGLShaderProgram& program, QGLFunctions& gl)
{
    GLint       map_flags[50];
    QVector4D diffuse_colors[50];
    map<int,StandartMaterial*>::iterator i;
    int j=0;
    for(i=id2mat.begin(); i!=id2mat.end(); i++)
    {
        int id = (*i).first - 1;

        if(id < 50)
            diffuse_colors[id] = QVector4D((*i).second->diffuse[0]/255.,
                                           (*i).second->diffuse[1]/255.,
                                            (*i).second->diffuse[2]/255.,
                                            (*i).second->diffuse[3]/255.);
        if( (*i).second->diffuse_map!=" " )
            map_flags[id] = 1;
        else
            map_flags[id] = 0;
        gl.glActiveTexture(GL_TEXTURE0 + id);
        glBindTexture(GL_TEXTURE_2D, (*i).second->texture_id);

        j++;
    }
    program.setUniformValue("RenderMode", (GLuint)1);
    program.setUniformValue("texture0", 0);
    program.setUniformValue("texture1", 1);
    program.setUniformValue("texture2", 2);
    program.setUniformValue("texture3", 3);
    program.setUniformValue("texture4", 4);
    program.setUniformValue("texture5", 5);
    program.setUniformValue("texture6", 6);
    program.setUniformValueArray("diffuse_colors", diffuse_colors, 50);
    program.setUniformValueArray("map_flags", map_flags, 50);
    program.setUniformValue( "material.Ka", QVector3D( 0.6f, 0.6f, 0.6f ) );
    program.setUniformValue( "material.Kd", QVector3D( 1.0f, 1.0f, 1.0f ) );
    program.setUniformValue( "material.Ks", QVector3D( 0.4f, 0.4f, 0.4f ) );
    program.setUniformValue( "material.shininess", 50.0f );

}


void CMaterial::LoadTextures()
{
    //load textures
    for(int i=0; i<(int)matnames.size(); i++)
    {
        if(!matmap[matnames[i]].btextureloaded)
        {
            string fn = matmap[matnames[i]].texture;
            imageTGA im(fn, imageSampler());
            matmap[matnames[i]].id = im.getId();
            matmap[matnames[i]].width = im.m_width;
            matmap[matnames[i]].height = im.m_height;
            matmap[matnames[i]].btextureloaded = true;
            prn("%s id=%d", matmap[matnames[i]].texture.c_str(), im.getId());
            qDebug("%s id=%d", matmap[matnames[i]].texture.c_str(), im.getId());
        }

    }
	


	
}

unsigned int  CMaterial::GetTexureID(string matname)
{
    if( matmap.find(matname)!= matmap.end() )
    {
        return matmap[matname].id;
    }
    else
        return 0;
}

void CMaterial::Bind(string name)
{
   if( matmap.find(name)!= matmap.end())
   {

           glEnable(GL_TEXTURE_2D);
           glBindTexture(GL_TEXTURE_2D, matmap[name].id);
    }

}

void CMaterial::Unbind(string name)
{
    glDisable(GL_TEXTURE_2D);
}



void CMaterial::Slot1()
{

}

bool recursiveFindFile(QString fn, QDir& dir)
{
    dir.setFilter(QDir::Files | QDir::Dirs | QDir::Hidden );
    dir.setSorting(QDir::Size | QDir::Reversed);
    QFileInfoList list = dir.entryInfoList();
    if(bAbort) return true;
    //if( dir.exists(fn) )        return true;
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        if( fileInfo.fileName().contains(fn) )
        {
            bAbort = true;
            resultFn = dir.absoluteFilePath(fileInfo.fileName());
            return true;
        }
        if( fileInfo.fileName().contains("..") )
            continue;
        if( fileInfo.fileName().contains(".") )
            continue;
        if( fileInfo.isDir() )
        {
            if(checkedDirs[fileInfo.fileName()]!="OK")
            {
                dir.cd(fileInfo.fileName());
                checkedDirs[fileInfo.fileName()] = "OK";
                recursiveFindFile(fn, dir);
                dir.cdUp();
            }
        }
        //std::cout << qPrintable(QString("%1 %2").arg(fileInfo.size(), 10)
        //                        .arg(fileInfo.fileName()));
        //std::cout << std::endl;
    }
    return false;
}

string CMaterial::CheckFixExist(string fn)
{
    QFile f(fn.c_str());
    if( !f.exists() )
    {
        int i1 = fn.find_last_of('\\');
        int i2 = fn.find_last_of('/');
        if(i1 > i2)
            i2 = i1;
        fn = fn.substr(i2+1, fn.size()-i2-1);

        QDir dir;
        dir.setCurrent(exe_path);

        if( dir.cd(dir.absolutePath() + "/textures") )
        {
            bAbort = false;
            checkedDirs.clear();
            recursiveFindFile(QString(fn.c_str()),dir);
            if( bAbort )
            {
                QString new_fn = resultFn;
                return string(new_fn.toUtf8().data());
            }

        }
    }
    return fn;
}

int CMaterial::GenTexture(float fill_col[4])
{
    imageTGA* im = new imageTGA(fill_col);
    prn("Gen texture... %dx%d %d", im->m_width, im->m_height, im->getId());
    return im->m_id;
}

unsigned int CMaterial::LoadTexture(string fn, imageSampler sampler)
{
    if( IsTextureLoaded(fn) == true ) return FindTexture(fn);    
    if(fn=="no_file")
        return -1;
    string fn0 = fn;
    fn = CheckFixExist(fn);
    QFile f(fn.c_str());
    if( f.exists() )
    {
        imageTGA* im = new imageTGA(fn, sampler);
        im->filename = fn0;
        Textures.push_back(im);
        prn("Load texture... %dx%d %s %d", im->m_width, im->m_height, fn.c_str(),im->getId());
        return im->m_id;
    }
    else
       return NO_TEXTURE_ID;


}

bool CMaterial::IsTextureLoaded(string fn)
{
    for(int i=0; i<(int)Textures.size(); i++)
        if ( Textures[i]->filename == fn )
            return true;
    return false;
}

int CMaterial::FindTexture(string fn)
{
    for(int i=0; i<(int)Textures.size(); i++)
        if ( Textures[i]->filename == fn )
            return Textures[i]->m_id;
    return -1;
}

int CMaterial::print()
{
    for(int i=0; i<(int)Textures.size(); i++)
       prn("id=%d, %s ",Textures[i]->m_id,Textures[i]->filename.c_str());
    return 0;
}
