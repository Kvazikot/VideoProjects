/********************************************

  Terrain это класс который делает тото

  Свойства:
  name - имя
  N - размер массива
  Terrain_algos - "points" "lines"

  Методы:
    Методы для отработки функций
  

*********************************************/

//REALISATION CPP
#include "../include/stdafx.h"
#include "../include/cinput.h"
#include "../include/curve.h"
#include "../include/str_utils.h"
#include "../include/timer.h"
#include "../include/vpnt.h"
#include "../include/geometry.h"
#include "../include/glcode.h"
#include "../include/var_parse.h"
#include "../include/matrix.h"
#include "../include/random_staff.h"
#include "../include/law.h"
#include "../include/vector3d.h"
#include "../include/kirpich.h"
#include "../include/terrain.h"
#include "../include/cobjregistry.h"
#include "../include/objectinspector.h"
#include <algorithm>
#include <iostream>
#include <time.h>


//#include "globals.h"
extern VarParser varparser;
static CObjRegistry& MREG = CObjRegistry::getInstance2();
//static ObjectInspector& inspector = ObjectInspector::getInstance();
static CInput* iinput = CInput::getInstance();
//extern Timer* gtimer;
Spline x_spl[100];
Spline y_spl[100];
PPnt null_pp;
TerrainDATA ter_data;
float X[1025][1025]; // для фрактального генератора


//char* atr_sets_arr[] = {" ",
const string terrains_listc = "Умеренные_горы Лесостепь_с_водоемами"; 
string ter_filters = "Рельеф Лес Поверхность Функции";
static char* atr_sets_arr[] = {"3-18","18-22","3-22","3"}; 

//====================================================================
// <OBJECT>   </OBJECT>
//====================================================================

Terrain::Terrain()
{
	name = "terrain";
	classname = "Terrain";
	isCanRender = true;
	isEntety = true;
	isGroup = true;
	terrain_mesh = 0;
	faces		 = 0;
	bGenerated = false;

	NSelTerrain = 0;
	Nrender    = (REN_TYPE)SOFT_REN;
	Ngenerator = (GEN_TYPE)SOFTSEL_GEN;
	cur_terrain = terains[0];
	terrains_list = terrains_listc;
    //contour_list = contour_listc;

	InitPallete();
	// красный цвет по умолчанию
	RGB2GL2(RGB(255,0,0),color);
	v_idx = 0;

    iinput->add_delegat(this);
	
    Connect();
}

	

void Terrain::OnSelect()
{

	//flags.bNeedUpdObjectList = true;
}

void Terrain::OnUnselect()
{
	prn("Terrain::OnUnselect()");
	//for(int i=0; i<pts.size(); i++)	
	//  pts[i]->isHidden = 2;
	//flags.bNeedUpdObjectList = true;
}


void Terrain::SelectAtrSet(int idx)
{
	bHideStdAttrs = true;
	HideAllAtrs();		
	n_filter = idx;
    //ShowAtrs(atr_sets_arr[idx]);
    //flags.bNeedReinitObjInspector = true;
}



// аппроксимация полигональной сетки примитивами 
// меньшего обьема
vector<Pnt> bpnt;  
Pnt			maxPik;
void Terrain::DoApproximation(GemObject* mesh)
{
	int i,j;
	float a,b,c;
	int istart,iend; 
	vector<int>::iterator pos;
	float Nfaces, Nvert;
	vector<int> bpnti;  // индексы точек пересечения огранич обьема и полигональной сетки
	float rmin[5];		// минимумы
	float maxH, minH;

	Nfaces = mesh->mesh.faces.size();
	Nvert = mesh->mesh.points.size();
	Mesh& m = mesh->mesh;

	//terrain_mesh = (GemObject*)MREG.FindObj("bo");
	Kirpich* sb =  (Kirpich*)MREG.FindObj("searchbox");
	Pnt O = sb->ei.coord;
	a = sb->a; b = sb->b; c = sb->c;
	bpnt.resize(4);
	bpnti.resize(4);
	
	//поиск максимальной и минимальной высоты
    maxH = -2147483647.0f;
    minH = 2147483647.0f;
	for(i = 0; i < Nvert; i++)
	{
		if( m.points[i].y > maxH )
			maxH = m.points[i].y;
		if( m.points[i].y < minH )
			minH = m.points[i].y;
	}
	
	bpnt[0] = Pnt( O.x - a/2, 0, O.z + c/2 );
	bpnt[1] = Pnt( O.x - a/2, 0, O.z - c/2 );
	bpnt[2] = Pnt( O.x + a/2, 0, O.z - c/2 );
	bpnt[3] = Pnt( O.x + a/2, 0, O.z + c/2 );
	//prn("maxH= %f a,b,c=%f,%f,%f O=%s", maxH, a, b, c, O.toStr().c_str());

    for(i=0; i<4; i++ ) rmin[i] = 2147483647.0f;
	
    char gstr[1000];
	//поиск индексов сетки
	sprintf(gstr,"");	
	for(i = 0; i < Nvert; i++)
		for(j = 0; j < 4; j++)
			if ( rasst(m.points[i], bpnt[j]) < rmin[j] )
			{
				rmin[j] = rasst(m.points[i], bpnt[j]);
				bpnti[j] = i;
			}	

	prn("bpnti %d %d %d %d ",bpnti[0],bpnti[1],bpnti[2],bpnti[3]);

	//base
	for(i=0; i<4; i++)
		if( m.IndexIsOk(bpnti[i]))
			bpnt[i] = m.points[bpnti[i]];
	
	// find and print minimum elements
    pos = min_element (bpnti.begin(), bpnti.end());
	istart = *pos;
    pos = max_element (bpnti.begin(), bpnti.end());
	iend = *pos;
	//stringstream sout;    sout << "min: " << *pos << endl; prn(sout.str().c_str());

	//поиск максимальной и минимальной высоты в обьеме SV
    maxH = -2147483647.0f;
    minH = 2147483647.0f;
	for(i = istart; i < iend; i++)
	{
		if( m.points[i].y > maxH )
		{
			maxH = m.points[i].y;
			maxPik = m.points[i];
		}
		if( m.points[i].y < minH )
		{
			minH = m.points[i].y;
		}
	}

	
	
	//prn("mesh indexes corresponded bpnt: %s",gstr);

}

void PointSelection::Render(QOpenGLShaderProgram& program)
{
    for(int i=0; i < (int)pts.size(); i++)
        draw_p(program, "", pts[i], rgb, 0);
}


Pnt C;	// центр меша (красный)

void Terrain::InitPallete()
{
    pal[0].col[0] = 0.0; pal[0].col[1] = 0.1f; pal[0].col[2] = 0.8f; pal[0].col[3] = 1.0;
	pal[1].col[0] = 1.0; pal[1].col[1] = 0.0; pal[1].col[2] = 1.0; pal[1].col[3] = 1.0;
	pal[2].col[0] = 0.0; pal[2].col[1] = 1.0; pal[2].col[2] = 1.0; pal[2].col[3] = 1.0;
	pal[3].col[0] = 0.0; pal[3].col[1] = 0.0; pal[3].col[2] = 0.0; pal[3].col[3] = 1.0;
	pal[4].col[0] = 1.0; pal[4].col[1] = 1.0; pal[4].col[2] = 1.0; pal[4].col[3] = 1.0;
	pal[5].col[0] = 0.0; pal[5].col[1] = 0.0; pal[5].col[2] = 1.0; pal[5].col[3] = 1.0;
	pal[6].col[0] = 1.0; pal[6].col[1] = 0.0; pal[6].col[2] = 0.0; pal[6].col[3] = 1.0;
	pal[7].col[0] = 1.0; pal[7].col[1] = 0.0; pal[7].col[2] = 0.0; pal[7].col[3] = 1.0;
	pal[8].col[0] = 1.0; pal[8].col[1] = 0.0; pal[8].col[2] = 0.0; pal[8].col[3] = 1.0;
	pal[9].col[0] = 1.0; pal[9].col[1] = 0.0; pal[9].col[2] = 0.0; pal[9].col[3] = 1.0;
	

}

void Terrain::SoftRender(QOpenGLShaderProgram& program)
{




	unsigned int*  indexes = terrain_mesh->indexes;
	float*			vertices  = terrain_mesh->vertices;
	terrain_mesh->isCanRender = false;

	glEnableClientState( GL_VERTEX_ARRAY );
	glVertexPointer( 3, GL_FLOAT, 0, vertices );

		
// структурный элемент
    for(int i=0; i<(int)sels.size(); i++)
        sels[i].Render(program);

// центр
    //draw_navp("C0", C, RGB(255,0,0), 0, 1 );

	//return;
	glEnable(GL_COLOR_MATERIAL);
    glDisable(GL_TEXTURE_2D);

    // выделенные грани (для красной дорожки)
      Pnt b[4];
      b[0] = _face[0];
      b[1] = _face[1];
      b[2] = _face[2];
      b[3] = _face[3];
      glColor4f(1,0,0,1);
      glBegin(GL_TRIANGLES);
          glVertex3f(b[0].x, b[0].y, b[0].z);
          glVertex3f(b[1].x, b[1].y, b[1].z);
          glVertex3f(b[2].x, b[2].y, b[2].z);
          glVertex3f(b[3].x, b[3].y, b[3].z);
          glVertex3f(b[0].x, b[0].y, b[0].z);
          glVertex3f(b[1].x, b[1].y, b[1].z);

      glEnd();


    // выделенные грани
      for(int i=0; i<(int)selranges.size(); i++)
      {
         glColor4f(1,0,0,1);
         if(selranges[i].what == 1) // выделяемые фейсы
            glDrawElements( GL_TRIANGLES, (selranges[i].end - selranges[i].start)*3, GL_UNSIGNED_INT, indexes+selranges[i].start*3);
         else // выделяемые точки
         {
             glPointSize(5);
             if(selranges[i].what == 0) // выделяемые точки
                glDrawElements( GL_POINTS, (selranges[i].end - selranges[i].start)*3, GL_UNSIGNED_INT, indexes+selranges[i].start*3);
             glPointSize(1);
         }

      }


      //секлетченный список вершин
     Pnt p;
     for(int i=0; i<(int)selvertices.size(); i++)
     {
         glPointSize(5);
         //glDrawElements( GL_POINTS, 1, GL_UNSIGNED_INT, indexes+selvertices[i]*3);
         p = Pnt(vertices[3*selvertices[i]],vertices[3*selvertices[i]+1],vertices[3*selvertices[i]+2]);
         if( bShowWeghts == false)
            draw_p(program, "",p,RGB(255,0,0),0);
         else
         {
             PPnt* pp = get_pnt(selvertices[i]);
             draw_p(program, DoubleToStr(pp->w[0],"%1.1lf"),p,RGB(255,0,0),0);
         }

         glPointSize(1);
     }


//полигональная сетка
  //for(int i=0; i<ranges.size(); i++)
  {
	  
	
	 // отрисовывать вершины
	 if(Nrender == 1)
	 {
	    glPointSize(2);
		 glColor4f(0,0,1,1);
		 glMaterialfv ( GL_FRONT, GL_AMBIENT, pal[0].col );
		 glDrawElements( GL_POINTS,  terrain_mesh->face_count*3, GL_UNSIGNED_INT, indexes);
	     glPointSize(1);
	 }

	if( Nrender == 3  )
	{
		glColor4f(0,0,1,1);
		//glPolygonMode(GL_FRONT, GL_FILL);
		glMaterialfv ( GL_FRONT, GL_AMBIENT, pal[0].col );
		glDrawElements( GL_TRIANGLES,  terrain_mesh->face_count*3,  GL_UNSIGNED_INT, indexes);
	}


	 // отрисовать перестроенные к четырехугольной сетке
	 if((faces!=0) && (bGenerated) )
	 {
		 if( Nrender == 3  )
		 {
				glColor4f(1,1,1,1);
				glMaterialfv ( GL_FRONT, GL_DIFFUSE, pal[4].col );
		 }
		 if( Nrender == 2  )
		 {
				glColor4f(1,0,0,1);
				glMaterialfv ( GL_FRONT, GL_DIFFUSE, pal[1].col );
		 }

		 if( Nrender == 4  )
		 {

			 Pnt p;
			 for(int i=0; i<terrain_mesh->vert_count; i++)
			 {
				 //glDrawElements( GL_POINTS, 1, GL_UNSIGNED_INT, indexes+selvertices[i]*3);	  
				 p = Pnt(vertices[3*i],vertices[3*i+1],vertices[3*i+2]); 
                 draw_p(program, IntToStr(i),p,RGB(255,0,0),0);
			 }
		 }

         int j;
         if( (Nrender == 3) || (Nrender == 2) )
         {
             for(int i=1; i<N4faces; i++)
             {
 /*				if( find(selfaces.begin(), selfaces.end(), i)!= selfaces.end() )

                 {
                     glColor4f(1,0,0,1);
                     glMaterialfv ( GL_FRONT, GL_DIFFUSE, pal[1].col );
                 }
                 else
                 {
                     glColor4f(0,0,1,1);
                     glMaterialfv ( GL_FRONT, GL_DIFFUSE, pal[1].col );
                 }
 */

                 glBegin(GL_LINES);
                     j = faces[i*4];
                     glVertex3f(vertices[j*3], vertices[j*3+1], vertices[j*3+2]);
                     j = faces[i*4+1];
                     glVertex3f(vertices[j*3], vertices[j*3+1], vertices[j*3+2]);
                 glEnd();
             }


             for(int i=1; i<N4faces; i++)
             {
                 glBegin(GL_LINES);
                     j = faces[i*4];
                     glVertex3f(vertices[j*3], vertices[j*3+1], vertices[j*3+2]);
                     j = faces[i*4+2];
                     glVertex3f(vertices[j*3], vertices[j*3+1], vertices[j*3+2]);
                 glEnd();
             }
         }


      }
     }
/*
     glEnable(GL_COLOR_MATERIAL);
     glColor4f(1,0,0,1);
     // фрактальный тест
     for(int i=0; i<1024; i++)
         for(int j=0; j<1024; j++)
         {
             Pnt p0;

             p0.x =  normal3(2,10);
             p0.y =  0;
             p0.z =  normal3(2,10);
             float X = p0.x;
             float Y = 0;
             float Z = p0.z;
             glBegin(GL_LINES);
             glVertex3f(p0.x,p0.y,p0.z);
             glVertex3f(X,Y,Z);
             glEnd();

         }
     return;
*/
/*

     // выделенные грани
       for(int i=0; i<selranges.size(); i++)
       {
          glColor4f(1,0,0,1);
          if(selranges[i].what == 1) // выделяемые фейсы
             glDrawElements( GL_TRIANGLES, (selranges[i].end - selranges[i].start)*3, GL_UNSIGNED_INT, indexes+selranges[i].start*3);
          else // выделяемые точки
          {
              glPointSize(5);
              if(selranges[i].what == 0) // выделяемые точки
                 glDrawElements( GL_POINTS, (selranges[i].end - selranges[i].start)*3, GL_UNSIGNED_INT, indexes+selranges[i].start*3);
              glPointSize(1);
          }

       }


       //секлетченный список вершин
      Pnt p;
      for(int i=0; i<selvertices.size(); i++)
      {
          glPointSize(5);
          //glDrawElements( GL_POINTS, 1, GL_UNSIGNED_INT, indexes+selvertices[i]*3);
          p = Pnt(vertices[3*selvertices[i]],vertices[3*selvertices[i]+1],vertices[3*selvertices[i]+2]);
          if( bShowWeghts == false)
             draw_p(IntToStr(i),p,RGB(255,0,0),0);
          else
          {
              PPnt* pp = get_pnt(selvertices[i]);
              draw_p(DoubleToStr(pp->w[0],"%1.1lf"),p,RGB(255,0,0),0);
          }
          glPointSize(1);
      }
*/
      glDisable(GL_COLOR_MATERIAL);

}

// выбрать грани окружающие вершину
void Terrain::SelectFacesByVetex(int i)
{
    i=0;
	if(terrain_mesh==0) return;

}

// выбрать соседние вершины
void Terrain::SelectNeibVertices(int i)
{
	if(terrain_mesh==0) return;
	if(i<0) return;
	if(i>terrain_mesh->vert_count) return;
	selvertices.clear();
	selvertices.push_back(i);
	selvertices.push_back(i+1);
	if( (i+f) < terrain_mesh->vert_count)
	{
		selvertices.push_back(i+f);
		selvertices.push_back(i+f-1);
	}

}


// выбор направления
int next_dir(int i)
{
    i++; 
	//if(i==1) { i = 4; return i; }
	//if(i==2) { i = 3; return i; }
	//if(i==3) { i = 2; return i; }
	if(i==4) i = 0;
    return i;
}


// для обхода сетки кругами вокруг точки (ВЫЧИСЛЯЕТ вершину)
int nex_step2(int& c, int& a, int& b, int& i, int& f)
{
	

	switch( c%4 )
	{
		case 0:			
			i+=a;		
		break;
		case 1:			
			i+=b;		
		break;
		case 2:			
			i-=a;		
		break;
		case 3:			
			i-=b;		
		break;	
	}

	if( (c%2) == 1 )
	{
		a = a + 1;
		b = b + f;
	}
	
	c++;
	
    return i;
}

void  Terrain::FixRange(int& i)
{
		if( i < 0) i = 0 ;
		if( i > N) i = N-1 ;
}

void  Terrain::FillMidPoints( vector<int>& src, vector<int>& dst)
{
    int i0,i1,tmp;


    for( int i=1; i < (int)src.size(); i++ )
	{
		i0 = src[i-1]; i1 = src[i];
		if( i0 > i1 )
		{
			tmp = i0;
			i0 = i1;
			i1 = tmp;
		}

		if( abs(i0 - i1) < f )
		{
			for(int i = i0; i < i1; i++)
				dst.push_back( i );	
		}
		else
		{
			for(int i = i0; i < i1; i+=f)
				dst.push_back( i );			
		}

		
	}
	
	return;
	
}

void  PrintSelVertexes(vector<int>& arr)
{
	string s,s2;   
    for(int i=1; i<(int)arr.size()-1; i++)
	{
		s+=IntToStr( arr[i] - arr[i-1] ) + " ";
		s2+=IntToStr( arr[i] ) + " ";
	}
	//prn(s.c_str());
	prn(s2.c_str());
}

void make_step(int& rot, int& i, int& f)
{
	switch( rot )
	{
		case 0:			
			i+=1;		
		break;
		case 1:			
			i+=f;		
		break;
		case 2:			
			i-=1;		
		break;
		case 3:			
			i-=f;		
		break;	
	}
}

// для обхода сетки кругами вокруг точки (ВЫЧИСЛЯЕТ вершину)
int nex_step(int& c, int& nex_rot, int& i, int& f, int& rot, int& a, bool& povtor)
{
	make_step(rot,i,f);
	if( c == nex_rot )
	{
		rot++; if( rot == 4 ) rot = 0;
		c++;
		if( povtor ) a++;
		nex_rot = c + a;	
		povtor=!povtor;		 
		return i;
	}
	c++;
    return i;
}

PPnt*  Terrain::FindNearestP(Pnt& p)
{
	float minR = 1000;
	float r;
	vector<PPnt*>::iterator it;
	PPnt* minP = 0;
	for(it=vertL.begin(); it!=vertL.end(); it++)
	{
		Pnt p2 = Pnt( *(*it)->x, *(*it)->y, *(*it)->z);
		r = rasst(p, p2);
		if(  r < minR ) 
		{
			minR = r; minP = *it;			
		}
	}
	return minP;	
}


// расстояние между точками
double rasstD(Pnt p1, Pnt p2)
{
     return sqrt( (double) ( sq((double)p1.x - (double)p2.x)  +  sq((double)p1.y - (double)p2.y ) + sq((double)p1.z - (double)p2.z))) ;
}

extern bool equalTo(double lv, double rv, double prec);
//{	return ( fabs(lv - rv) < prec );}

bool IsPntInsideFace(Pnt& p, Pnt f[4])
{
    //for(int i=0; i<4; i++) f[i].y = 0;
    //p.y = 0;
    float minZ=10000000000000.0f;
    float minX=10000000000000.0f;
    float maxZ=-10000000000000.0f;
    float maxX=-10000000000000.0f;
    for(int i=0; i<4; i++)
    {
        if(f[i].z < minZ) minZ = f[i].z;
        if(f[i].x < minX) minX = f[i].x;
        if(f[i].z > maxZ) maxZ = f[i].z;
        if(f[i].x > maxX) maxX = f[i].x;

    }

    return ((p.x > minX && p.x < maxX) && (p.z > minZ && p.z < maxZ));

}

void  Terrain::FindClosestFace(Pnt* res, Pnt to_p, int from_idx,  int count, float delta)
{
    count=0;delta=0;
    Pnt f1[4]; Pnt f2[4]; Pnt f3[4]; Pnt f4[4];
    int i = from_idx;
    f1[0] = get_p(i+f);
    f1[1] = get_p(i+1);
    f1[2] = get_p(i+f+1);
    f1[3] = get_p(i);

    f2[0] = get_p(i-f);
    f2[1] = get_p(i+1);
    f2[2] = get_p(i-f+1);
    f2[3] = get_p(i);

    f3[0] = get_p(i-f);
    f3[1] = get_p(i-1);
    f3[2] = get_p(i-f-1);
    f3[3] = get_p(i);

    f4[0] = get_p(i+f);
    f4[1] = get_p(i-1);
    f4[2] = get_p(i+f-1);
    f4[3] = get_p(i);


    if( IsPntInsideFace(to_p, f1) ) for(int i=0;i<4;i++) res[i] = f1[i];
    if( IsPntInsideFace(to_p, f2) ) for(int i=0;i<4;i++) res[i] = f2[i];
    if( IsPntInsideFace(to_p, f3) ) for(int i=0;i<4;i++) res[i] = f3[i];
    if( IsPntInsideFace(to_p, f4) ) for(int i=0;i<4;i++) res[i] = f4[i];
}




PPnt*  Terrain::FindClosestTo(Pnt p, int from_idx,  int count, float delta)
{
    PPnt* res = get_pnt(from_idx);
    double minR = 1000;
    double r;
    if(terrain_mesh==0) return 0;
    int a,c,nex_rot,rot;
    bool povtor;
    delta=0;
    int i = from_idx;
    a = 0; c = 0; nex_rot = 1; rot = 1; povtor = false;
    while( --count>0 )
    {
        nex_step(c,nex_rot,i,f,rot,a,povtor);
        FixRange(i);
        r = rasstD(get_p(i), p);

        if(  r < minR  )
        //if( equalTo(r, 0.003, 0.0001) )
        //if( equalTo(r, 0.003, 0.0001) )
        {
            minR = r;
            //if(r < 1)
            res = get_pnt(i);
            //break;
        }
    }

    return res;
}



bool   Terrain::IsNeib(int i1, int i2)
{
	if(! IsIdxValid (i1) ) return false;
	if(! IsIdxValid (i2) ) return false;
	if( i1 == i2 ) return true;
	if( ( abs( i1 - i2 ) - f ) < 2 ) return true;
	if( abs( i1 - i2 ) == 1 ) return true;
	//prn("%d", abs( i1 - i2 ));

/*	for(int i=0; i<4; i++)
	{
		if( p1->p[i] == p2 ) return true;
		if( p2->p[i] == p1 ) return true;
	}
*/
	return false;
}

bool   Terrain::IsIdxValid(int i)
{
	if( i < 0) return false;
	if( i > N) return false;
	return true;

}

void  Terrain::SelectVertexesAround(int center_idx, int count, vector<int>& arr)
{
	if(terrain_mesh==0) return;
    int c; int i0,b,a,nex_rot,rot;
	bool povtor;
	int i = center_idx;
	a = 0; b = f; c = 0; i0 = i; 
	nex_rot = 1; rot = 1; povtor = false;
    while( --count>0 )
    {
		nex_step(c,nex_rot,i,f,rot,a,povtor); 
		FixRange(i);		
		arr.push_back( i );   	
    }
}

void Terrain::MakeHolm(int center_idx, float H, float R)
{
	vector<int> vertices;
	PPnt* p;	
	
	selvertices.clear();
	SelectVertexesAround(center_idx, R, W_LIN, vertices);
    for(int i=0; i < (int)vertices.size(); i++)
	{
		p = get_pnt(vertices[i]);
		(*p->y)+=H * p->w[0];

	}
	selvertices = vertices;
}


void  Terrain::SelectVertexesAround(int center_idx, float R, int WeithFunc, vector<int>& arr)
{
	if(terrain_mesh==0) return;
    int c; int i0,b,a,nex_rot,rot,count;
	bool povtor;	
	float r;
	int i = center_idx;
	a = 0; b = f; c = 0; i0 = i; 
	nex_rot = 1; rot = 1; povtor = false;
	count = terrain_mesh->vert_count-1; // MAX
    WeithFunc=0;
    while( --count>0 )
    {
		r = rasst(get_p(i), get_p(center_idx)) + 0.00000000001;
		PPnt* p = get_pnt(i);
		p->w[0] = 1. / r;
		if(p->w[0] > 1) p->w[0] = 1;
		if( r > R ) break;
		nex_step(c,nex_rot,i,f,rot,a,povtor); 
		FixRange(i);		
		arr.push_back( i ); 
    }
}


void Terrain::SelectContour(int center_idx, char arr[100][100], int w, int h, vector<int>& selection)
{
	int ii = center_idx; // - (w/2) - (h/2) * f;
	w = 3;
	h = 3;
	for(int j=0; j<h; j++)	
	{
		
		for(int i=0; i<w; i++)
		{
			arr[i][j] = 1;
			ii = ii + i;
			if( arr[i][j]!= 0 )
				selection.push_back(ii);
		}
		ii = ii + f;
	}
	PrintSelVertexes(selection);
}

// выбрать соседние вершины
void Terrain::SelectVertexesAroundTest(int i, int cnt)
{
	if(terrain_mesh==0) return;
	if(i<0) return;
	if(i>terrain_mesh->vert_count) return;
	selvertices.clear();
	SelectVertexesAround(i, cnt, selvertices);

}


PPnt* Terrain::set_pnt(PPnt* p, int i)
{
	p->i = i;
	p->x = &terrain_mesh->vertices[i*3];
	p->y = &terrain_mesh->vertices[i*3+1];
	p->z = &terrain_mesh->vertices[i*3+2];	
	return p;
}

PPnt* Terrain::get_pnt(int i)
{
	return vertL[i];
}

Pnt Terrain::get_p(int i)
{
	return Pnt(*vertL[i]->x, *vertL[i]->y, *vertL[i]->z);
}


void Terrain::set_pnt_neib(PPnt* p, int i, int f)
{
	p->p[0] = get_pnt(i-1);
	p->p[1] = get_pnt(i+1);
	p->p[2] = get_pnt(i+f+1);
	p->p[3] = get_pnt(i+f);
}

void Terrain::get_face(PPnt* p, int i, int f)
{
	p->p[0] = get_pnt(i+1);
	p->p[1] = get_pnt(i-1);
	p->p[2] = get_pnt(i+f+1);
	p->p[3] = get_pnt(i+f);
}

// создать связанные списки
void Terrain::CreateLinkedLists()
{
	f = (int)floor( sqrt((double)terrain_mesh->vert_count))  ;
	faces =  (unsigned int*)malloc( sizeof(unsigned int) * terrain_mesh->vert_count * 4 );

	// первая вершина
	vertL.clear();
	vertL.resize( terrain_mesh->vert_count );	
	//создать массив указателей
	for(int i = 0; i < terrain_mesh->vert_count; i++ )
	{
		vertL[i] = new PPnt();
		for(int j=0; j<4; j++) 
			vertL[i]->p[j] = new PPnt();
	}
	PPnt* start_v = vertL[0];
	start_v->i = 0;
	start_v->x = &terrain_mesh->vertices[0];
	start_v->y = &terrain_mesh->vertices[1];
	start_v->z = &terrain_mesh->vertices[2];	
	
	//заполнить список вершин
	for(int i = 1; i < terrain_mesh->vert_count; i++ )
	{
		PPnt* p = vertL[i];
		set_pnt(p, i);
	}

	//заполнить соседей
	for(int i = 1; i < terrain_mesh->vert_count - f - 1; i++ )
	{
		PPnt* p = vertL[i];
		set_pnt_neib(p, i, f);
	}

	
	PPnt* p = vertL[0];
	//faces = 0;
	//return;
	//переход к прямоугольной сетке 
	N4faces = 0;
    faceL.resize( terrain_mesh->vert_count );
    for(int i = 1; i < terrain_mesh->vert_count  -f -1 ; i++)
    {
        p = get_pnt(i);
        get_face(p, i, f);
        faceL[i] = new PFace();
        for(int j=0; j<4; j++)
            faceL[i]->p[j] = new PPnt();
        faces[i*4]		 = p->p[0]->i;
        faces[i*4+1]	 = p->p[1]->i;
        faces[i*4+2]	 = p->p[2]->i;
        faces[i*4+3]	 = p->p[3]->i;

        faceL[i]->p[0]  =  p->p[0];
        faceL[i]->p[1]  =  p->p[1];
        faceL[i]->p[2]  =  p->p[2];
        faceL[i]->p[3]  =  p->p[3];

        N4faces++;

    }


}

bool PntInBox(Pnt& box, Pnt& A, Pnt& C, float R)
{
    R=0;
	return ( fabs(A.x - C.x) < box.x ) && ( fabs(A.y - C.y) < box.y ) && ( fabs(A.z - C.z) < box.z );
}


/*


//Подсветка фейсов по сфере
void Terrain::SelectFacesBySphere( Pnt O, float R )
{
	Pnt box(R,R,R);

	PntInBox()
	


	//поиск индексов сетки
	sprintf(gstr,"");	
	for(i = 0; i < Nvert; i++)
	for(j = 0; j < 4; j++)
	if ( rasst(m.points[i], bpnt[j]) < rmin[j] )
	{
		rasst(m.points[i], bpnt[j]);
		
	}	
	prn("bpnti %d %d %d %d ",bpnti[0],bpnti[1],bpnti[2],bpnti[3]);

}
*/

//Подсветка фейсов
void Terrain::HighlightFaces(int i_start, int i_end, int what )
{
	if( terrain_mesh == 0 ) return;

	selranges.clear();	
	t_rangeE r;
	r.start = i_start;
	r.end = i_end;
	r.what = what;
	selranges.push_back(r);

}

// Генерация местности 2 алгоритм мягкого выделения
void Terrain::GenerateTerrain2( GemObject* terrain_mesh, Terrain_params& ter_params )
{

	int Nfaces, Nvert;

    ter_params = ter_params;
	if( terrain_mesh == 0 ) return;
	N = terrain_mesh->vert_count;
	f = (int)floor( sqrt((double)terrain_mesh->vert_count));
	
	prn("sof PPnt %d", sizeof(PPnt));

	CreateLinkedLists();

	bGenerated = false;
	Nfaces = terrain_mesh->face_count;
	Nvert =  terrain_mesh->vert_count;
	f = (int)floor( sqrt((double)terrain_mesh->vert_count)) ;
	prn("Mesh f = %d Nvert = %d Nfaces = %d", f, Nvert, Nfaces);

	bGenerated = true;

}

//------------------------ Алгоритм фрактального построения рельефа -----------------------------

 void DIVIDE(float* X, float ratio, float std, int i0, int i2, int level, int maxlevel)
 {
    int i1 = (i0 + i2) / 2;
    X[i1] = 0.5 * (X[i0] + X[i2]) + std * normal3(2,0.5);
    if(level < maxlevel)
    {
        float srdmid = std * ratio;
        DIVIDE(X,ratio,srdmid,i0,i2,level+1,maxlevel);
        DIVIDE(X,ratio,srdmid,i0,i2,level+1,maxlevel);
    }
 }


// H - размерность графика
// maxlevel - максимальное число рекурсий
// scale - вертикальный масштаб
void init_terrain(double H, int maxlevel, double scale)
{
    int N = pow((double)maxlevel,2);
    float* X = (float*)malloc(N*sizeof(float));
    int level = 1;
    int i0 = 1;
    int i2 = N + 1;
    for(int i=0; i < N + 1; i++)
    {
        int ratio = 1.0 / pow(2,H);
        int std = scale * ratio;
        X[1] = scale * normal3(2,0.5);
        X[N+1] = scale * normal3(2,0.5);
        DIVIDE(X,ratio,std,i0,i2,level,maxlevel);
    }
}


void Terrain::BrounovskLine()
{
    for(int i=0; i < (int)selvertices.size(); i++)
    {
        int ii=0, jj=0;
        if(i < f)
        {
            ii = i;
            jj = 0;
        }
        else
        {
            ii = i % f;
            jj = i / f;
        }
        if(ii > 1024) ii = 0;
        if(jj > 1024) jj = 0;
        PPnt* p = get_pnt(selvertices[i]);
        (*p->y)+= X[ii][jj];

    }
}



void init_terrain2(float H, int maxlevel, float scale, float mean, float std)
{
    int N = 100;
    float r = scale;
    X[0][0] = 0;
    X[0][N] = 0;
    X[N][0] = 0;
    X[N][N] = 0;
    int D = N ;
    int d = N / 2;



    srand((unsigned)time(NULL));
    for(int L=1; L<maxlevel; L++)
    {
        r = (float)r / powf(2,H/4.);
        //printf("r = %f \n",r);
        qDebug("r = %f \n",r);

        for(int i=d; i<(N-d); i+=D )
        {

            for(int j=d; j<(N-d);j+=D )
            {
                //printf("j=%d\n",j);
                X[i][j] = (X[i+d][j+d] + X[i+d][j-d] + X[i-d][j+d] + X[i-d][j-d])/4 + (float)r * normal3(std,mean);

            }
        }
        r = (float)r / pow(2,(float)H/2.);

        for(int i=d; i< N-d; i+=D)
        {
            X[i][0] = (X[i+d][0] + X[i-d][0] + X[i][d])/3 + (float)r * normal3(std,mean);
            X[i][N] = (X[i+d][N] + X[i-d][N] + X[i][N-d])/3 + (float)r * normal3(std,mean);
            X[0][i] = (X[0][i+d] + X[0][i-d] + X[d][i])/3 + (float)r * normal3(std,mean);
            X[N][i] = (X[N][i+d] + X[N][i-d] + X[N-d][i])/3 + (float)r * normal3(std,mean);
        }

        for(int i=d; i<N-d; i+=D)
        {
            for(int j=d; j<N-d;j+=D )
            {
                X[i][j] = (X[i][j+d] + X[i][j-d] + X[i+d][j] + X[i-d][j]) / 4 + (float)r * normal3(std,mean);

            }
        }

        for(int i=d; i<N-d; i+=D)
            for(int j=d; j<N-d;j+=D )
            {
                X[i][j] = (X[i][j+d] + X[i][j-d] + X[i+d][j] + X[i-d][j]) / 4 + (float)r * normal3(std,mean);
                //printf("i=%d j=%d\n",i,j);
            }

        D = D/2;
        d = d/2;
        if(D == 0) return;
        if(d == 0)
        {

            break;
        }
    }

    // поверхность сгенерирована спектр
/*
    XX  = new double* [N];
    dct = new double* [N];
    dct_t = new double* [N];
    tmp = new double* [N];
    SPC = new double* [N];
    for(int i = 0; i < N; i++)
    {
           XX[i] = new double [N];
           dct[i] = new double [N];
           dct_t[i] = new double [N];
           tmp[i] = new double [N];
           SPC[i] = new double [N];
    }
    for(int j=0; j<N; j++)
    for(int i=0; i<N; i++) XX[i][j] = X[i][j];

    CreateDCTMatrix(dct,dct_t,N);
    dmatr(XX,dct_t,tmp,N);
    dmatr(dct,tmp,SPC,N);
    dmatr(SPC,dct,tmp,N);
    // фильтрация частотного спектра
//    for(int j=0; j<N; j++)
//    for(int i=0; i<N; i++)
//        if(SPC[i][j]<10)
//            SPC[i][j] = 0;


    dmatr(dct_t,tmp,XX,N);

*/

}


void Terrain::Render(QOpenGLShaderProgram& program)
{

	if(!isCanRender)
		return;
	glPushMatrix();

	switch ( Ngenerator )
	{
		case NURBS_GEN:
		{
		}
		break;

		case SOFTSEL_GEN:
		{
			if(terrain_mesh!=0)
                SoftRender(program);
		}
		break;

	
	}
	

	

	
	glPopMatrix();
	

	//glDisable (GL_LINE_SMOOTH);          // Enable Anti-Aliasing

}



//object inspector event
void Terrain::OnAttrChange(t_atr& a)
{

    if( isLoading ) return;
	//prn("Terrain::OnAttrChange %s", a.name.c_str());
    if(a.name == "Template")\
	{
		terains[NSelTerrain] = cur_terrain;
		NSelTerrain = a.data;
		cur_terrain = terains[a.data];
        //flags.bNeedReinitObjInspector = true;
	}

    //if (a.name == "Filter" )
		//SelectAtrSet(a.data);

    if(a.name == "GENERATE")
		GenerateTerrain2(terrain_mesh, cur_terrain);
	
	if(a.name == "HighlightFaces")
	{
		HighlightFaces(highP.x, highP.y, highP.z);
	}

	if(a.name == "v_idxs")
	{		
        string s = v_idxs;
		varparser.ParseIntArray(s, selvertices,",");
        if(s.find("-")!=std::string::npos)
        {
            vector<string> parts = StrSplitE(s,"-",0);
            for(int i=StrToInt(parts[0]); i<StrToInt(parts[1]); i++ )
                selvertices.push_back(i);
        }
        else
        {
            s = "[" + v_idxs + ",1]";
            varparser.ParseIntArray(s, selvertices,",");
        }
	}

	if(a.name == "f_idxs")
	{
		string s = "[" + f_idxs + ",0]";
		varparser.ParseIntArray(s, selfaces,",");
	}

	if(a.name == "NeiboursOfVertex")
		SelectNeibVertices((int)v_idx);

	if(a.name == "FacesByVertex")
		SelectFacesByVetex((int)v_idx);
	
	if(a.name == "coord")
	{
		//GenerateTerrain(terrain_mesh, cur_terrain);
		if( terrain_mesh != 0 )
			terrain_mesh->ei.coord = ei.coord;
	}

	if(a.name == "NeiboursAround")
		SelectVertexesAroundTest(v_idx2, 10);

	if(a.name == "MakeHolm")
		MakeHolm((int)MakeHolmARG.x, MakeHolmARG.y, MakeHolmARG.z);
	
	if(a.name == "ClearSel")
		selvertices.clear();
    if(a.name == "brounovskLine")
        BrounovskLine();



}


//obj update slot
void Terrain::Slot1()
{
	//if( terrain_mesh !=0) DoApproximation(terrain_mesh);

    if( Timer::gtimer.fsec < 0.81) return;
	
	if( terrain_mesh == 0 )
	{
		GemObject* o;	
        o = (GemObject*)MREG.DeepFindObj("Terrain01");
		terrain_mesh = o;
        terrain_mesh->isCanRender = false; // особая отрисовка сетки поверхности
		//DoApproximation(terrain_mesh);
		if(o!=0)
		{

            init_terrain2(10,100,0.8f,0.5f,0.25f);

            GenerateTerrain2( terrain_mesh, terains[NSelTerrain] );
            //MakeHolm((int)MakeHolmARG.x, MakeHolmARG.y, MakeHolmARG.z);
			//SelectVertexesAroundTest(v_idx2, 20);
		}
		
	}




}
