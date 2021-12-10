#include "stdafx.h"
#include "mterrain.h"
#include "material.h"
#include "fractalterrain.h"

Pnt PPnt::null_p;
PPnt nullp;
static CObjRegistry& MREG = CObjRegistry::getInstance2();

static DWORD Rainbow[6]=		// Rainbow Of Colors
{
    RGB(255,0,0),RGB(0,255,0),RGB(0,0,255),RGB(0,0,0),RGB(11,255,0),RGB(0,0,0)

};



MTerrain::MTerrain()
{
    name="defaultTerrain";
    terrain_mesh = 0;
    isCanRender = true;
    scale = Pnt(0.01f,0.01f,0.01f);
    selvert = 73;
}

void addface(unsigned int*  indexes, int fcount, int A, int B, int C)
{
    indexes[3*fcount] =  A;
    indexes[3*fcount+1] =  B;
    indexes[3*fcount+2] =  C;
}

void MTerrain::Generate(int detail, float roughness, float hconst)
{
    FractalTerrain* fter = new FractalTerrain(detail);
    fter->roughness = roughness;
    fter->generate();
    QPair<int,int> idx;
    for(int k=0; k < mprops.l_segs; k++)
       for(int i=0; i < mprops.w_segs; i++)
       {
           float level = fter->get(i,k);
           idx.first = i;
           idx.second = k;
           PPnt* p = get_pnt(index_map[idx]);
           *p->y = level+hconst;
       }
    terrain_mesh->bUpdateMesh = true;

}

GemObject* SetMesh(GemObject* mesh, int w_segs, int h_segs, int l_segs, float sizex, float sizey, float sizez)
{
    w_segs++; h_segs++; l_segs++;
    int n_vert = (w_segs) * (h_segs) * (l_segs);
    mesh->SetNumVert(n_vert);
    int n_faces = w_segs * h_segs * l_segs * 12;
    mesh->SetNumFaces(n_faces);

    float dw = sizex / (float)w_segs;
    float dh = sizey / (float)h_segs;
    float dl = sizez / (float)l_segs;
    int vcount=0;
    int fcount=0;
    for(int k=0; k < l_segs; k++)
        for(int j=0; j < h_segs; j++)
            for(int i=0; i < w_segs; i++)
            {
                mesh->vertices[3*vcount] = (float)i * dw - sizex/2.;
                mesh->vertices[3*vcount+1] = (float)j * dh - sizey/2.;
                mesh->vertices[3*vcount+2] = (float)k * dl - sizez/2.;
                vcount++;
            }

    for(int k=0; k < l_segs-1; k++)
        for(int j=0; j < h_segs-1; j++)
            for(int i=0; i < w_segs-1; i++)
            {

                int A = i + j*w_segs + k*w_segs*h_segs;
                int B = i+1  + j*w_segs + k*w_segs*h_segs;
                int C = i+1 + j*w_segs + (k+1)*w_segs*h_segs;
                int D = i + j*w_segs + (k+1)*w_segs*h_segs;
                int F = i + (j+1)*w_segs + k*w_segs*h_segs;
                int G = i+1 + (j+1)*w_segs + k*w_segs*h_segs;
                int H = i+1 + (j+1)*w_segs + (k+1)*w_segs*h_segs;
                int M = i + (j+1)*w_segs + (k+1)*w_segs*h_segs;
                addface(mesh->indexes, fcount, A, B, D); fcount++;
                addface(mesh->indexes, fcount, D, B, C); fcount++;
                addface(mesh->indexes, fcount, A, B, F); fcount++;
                addface(mesh->indexes, fcount, F, B, G); fcount++;
                addface(mesh->indexes, fcount, A, F, D); fcount++;
                addface(mesh->indexes, fcount, D, M, F); fcount++;
                addface(mesh->indexes, fcount, F, G, M); fcount++;
                addface(mesh->indexes, fcount, M, G, H); fcount++;
                addface(mesh->indexes, fcount, D, C, H); fcount++;
                addface(mesh->indexes, fcount, D, M, H); fcount++;
                addface(mesh->indexes, fcount, B, C, G); fcount++;
                addface(mesh->indexes, fcount, C, G, H); fcount++;
            }

    mesh->vert_count=vcount;
    mesh->face_count=fcount;
    t_range rng;
    rng.start = 0;
    rng.end = fcount;
    mesh->ranges.push_back(rng);

    return mesh;
}

void MTerrain::Create(int w_segs, int h_segs, int l_segs, float sizex, float sizey, float sizez)
{
    isCanRender = false;

    if(terrain_mesh==0)
    {
        terrain_mesh = new GemObject();
    }

    w_segs++; h_segs++; l_segs++;
    int n_vert = (w_segs) * (h_segs) * (l_segs);
    terrain_mesh->SetNumVert(n_vert);
    int n_faces = w_segs * h_segs * l_segs * 12;
    terrain_mesh->SetNumFaces(n_faces);
    terrain_mesh->texcoords = (float*)malloc((n_faces+1)*6*sizeof(float));
    for(int i=0; i<n_faces; i++)
    {
        terrain_mesh->texcoords[i*6]=0.0;
        terrain_mesh->texcoords[i*6+1]=1.0;
        terrain_mesh->texcoords[i*6+2]=1.0;
        terrain_mesh->texcoords[i*6+3]=0.0;
        terrain_mesh->texcoords[i*6+4]=1.0;
        terrain_mesh->texcoords[i*6+5]=0.0;
    }

    mprops.h_segs = h_segs;
    mprops.w_segs = w_segs;
    mprops.l_segs = l_segs;
    mprops.sizex = sizex;
    mprops.sizey = sizey;
    mprops.sizez = sizez;

    float dw = sizex / (float)w_segs;
    float dh = sizey / (float)h_segs;
    float dl = sizez / (float)l_segs;
    int vcount=0;
    int fcount=0;
    for(int k=0; k < l_segs; k++)
        for(int j=0; j < h_segs; j++)
            for(int i=0; i < w_segs; i++)
            {
                terrain_mesh->vertices[3*vcount] = (float)i * dw - sizex/2.;
                terrain_mesh->vertices[3*vcount+1] = (float)j * dh - sizey/2.;
                terrain_mesh->vertices[3*vcount+2] = (float)k * dl - sizez/2.;
                if( j == h_segs-1 )
                {
                    QPair<int,int> idx;
                    idx.first = i;
                    idx.second = k;
                    index_map[idx] = vcount;
                }
                vcount++;
            }

    for(int k=0; k < l_segs-1; k++)
        for(int j=0; j < h_segs-1; j++)
            for(int i=0; i < w_segs-1; i++)
            {

                int A = i + j*w_segs + k*w_segs*h_segs;
                int B = i+1  + j*w_segs + k*w_segs*h_segs;
                int C = i+1 + j*w_segs + (k+1)*w_segs*h_segs;
                int D = i + j*w_segs + (k+1)*w_segs*h_segs;
                int F = i + (j+1)*w_segs + k*w_segs*h_segs;
                int G = i+1 + (j+1)*w_segs + k*w_segs*h_segs;
                int H = i+1 + (j+1)*w_segs + (k+1)*w_segs*h_segs;
                int M = i + (j+1)*w_segs + (k+1)*w_segs*h_segs;
                addface(terrain_mesh->indexes, fcount, A, B, D); fcount++;
                addface(terrain_mesh->indexes, fcount, D, B, C); fcount++;
                addface(terrain_mesh->indexes, fcount, A, B, F); fcount++;
                addface(terrain_mesh->indexes, fcount, F, B, G); fcount++;
                addface(terrain_mesh->indexes, fcount, A, F, D); fcount++;
                addface(terrain_mesh->indexes, fcount, D, M, F); fcount++;
                addface(terrain_mesh->indexes, fcount, F, G, M); fcount++;
                addface(terrain_mesh->indexes, fcount, M, G, H); fcount++;
                addface(terrain_mesh->indexes, fcount, D, C, H); fcount++;
                addface(terrain_mesh->indexes, fcount, D, M, H); fcount++;
                addface(terrain_mesh->indexes, fcount, B, C, G); fcount++;
                addface(terrain_mesh->indexes, fcount, C, G, H); fcount++;
            }

    terrain_mesh->vert_count=vcount;
    terrain_mesh->face_count=fcount;
    t_range rng;
    rng.start = 0;
    rng.end = fcount;
    terrain_mesh->ranges.push_back(rng);

    terrain_mesh->bUpdateMesh = true;
    CreateLinkedLists();
    isCanRender = true;

}

PPnt* MTerrain::set_pnt(PPnt* p, int i)
{
    p->i = i;
    p->x = &terrain_mesh->vertices[i*3];
    p->y = &terrain_mesh->vertices[i*3+1];
    p->z = &terrain_mesh->vertices[i*3+2];
    return p;
}

PPnt* MTerrain::get_pnt(int i)
{
    if(!IsIdxValid(i)) return &nullp;
    return vertL[i];
}

bool   MTerrain::IsIdxValid(int i)
{
    if( i < 0) return false;
    if( i > (int)vertL.size()) return false;
    return true;
}

PPnt*  MTerrain::FindNearestP(Pnt& p)
{
    float minR = 10000000;
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
double rasstDD(Pnt p1, Pnt p2)
{
     return ( (double) ( sq((double)p1.x - (double)p2.x)  +  sq((double)p1.y - (double)p2.y ) + sq((double)p1.z - (double)p2.z))) ;
}

PPnt*  MTerrain::FindClosestTo(Pnt p, int from_idx,  int count, float delta)
{
    PPnt* res;
    PPnt* cur_p;
    PPnt* seek_p;
    PPnt* min_p;
    map<double,PPnt*> rMap;
    map<double,PPnt*>::iterator it;
    int prev_rmap_size=0;

    double r;
    cur_p = get_pnt(from_idx);
    if(cur_p == &nullp) return &nullp;
    double minR = 10000000000;
    while( --count>0 )
    {

        for(unsigned i=0; i<6;i++)
        {
            seek_p = cur_p->p[i];
            if(seek_p->i == -1) continue;
            r = rasstDD(Pnt(*seek_p->x,*seek_p->y,*seek_p->z), p);
            //draw_p(IntToStr(count),Pnt(*cur_p->x,*cur_p->y,*cur_p->z),RGB(255,0,0),0);
            if(  r < minR  )
            {
                minR = r;
                min_p = seek_p;
                res = min_p;
                rMap[minR] = seek_p;
            }
        }
        cur_p = min_p;
        if(minR < delta )
            break;
        if((count%20) == 0)
        {
           if(prev_rmap_size == rMap.size())
           {
               res = rMap.begin()->second;
//               prn("====");
//               for(it=rMap.begin(); it!=rMap.end();it++)
//               {
//                   prn("%f",it->first);
//               }
//               prn("====");
               break;
           }
           prev_rmap_size = rMap.size();
        }
    }
   // prn("points %d", count);

    return res;
}


void MTerrain::CreateLinkedLists()
{

   // scale = terrain_mesh->ei.scale;
    // первая вершина
    vertL.clear();
    vertL.resize( terrain_mesh->vert_count );
    //создать массив указателей
    for(int i = 0; i < terrain_mesh->vert_count; i++ )
    {
        vertL[i] = new PPnt();
        for(int j=0; j<7; j++)
            vertL[i]->p[j] = new PPnt();
    }

    PPnt* start_v = vertL[0];
    start_v->i = 0;
    start_v->x = &terrain_mesh->vertices[0];
    start_v->y = &terrain_mesh->vertices[1];
    start_v->z = &terrain_mesh->vertices[2];
    nullp.x = start_v->x;
    nullp.y = start_v->y;
    nullp.z = start_v->z;

    //заполнить список вершин
    for(int i = 1; i < terrain_mesh->vert_count; i++ )
    {
        PPnt* p = vertL[i];
        set_pnt(p, i);
    }

    //scale
    if(scale.x!=0)
    for(int i = 0; i < terrain_mesh->vert_count; i++ )
    {
        PPnt* p = vertL[i];
        *p->x = scale.x * *p->x;
        *p->y = scale.y * *p->y;
        *p->z = scale.z * *p->z;
    }


    //заполнить соседей
    for(int i = 0; i < terrain_mesh->face_count; i++ )
    {
        int i1 = terrain_mesh->indexes[i*3];
        int i2 = terrain_mesh->indexes[i*3+1];
        int i3 = terrain_mesh->indexes[i*3+2];
        get_pnt(i1)->AddNeib(get_pnt(i2));
        get_pnt(i1)->AddNeib(get_pnt(i3));
        get_pnt(i2)->AddNeib(get_pnt(i1));
        get_pnt(i2)->AddNeib(get_pnt(i3));
        get_pnt(i3)->AddNeib(get_pnt(i1));
        get_pnt(i3)->AddNeib(get_pnt(i2));
    }


    //отсортировать соседей
//    for(int i = 0; i < terrain_mesh->vert_count-1; i++ )
//    {
//        PPnt* p = vertL[i];
//        PPnt* tmp;
//        tmp = p->p[4];
//        p->p[4] = p->p[5];
//        p->p[5] = tmp;
//        PPnt* p_new[4];
//        p_new[0] = p->p[0];
//        p_new[1] = p->p[2];
//        p_new[2] = p->p[3];
//        p_new[3] = p->p[5];
//        for(char i=0; i<4; i++)
//            p->p[i] = p_new[i];
//    }

}


void MTerrain::Render()
{

    if(terrain_mesh!=0)
    {
        if(terrain_mesh->isCanRender)
        {
            terrain_mesh->Render();
//            glBegin(GL_LINES);
//            for(unsigned int i=0; i<vertL.size(); i++)
//            {
//                PPnt* p = vertL[i];
//                glVertex3f(*p->x, *p->y, *p->z);
//            }
//            glEnd();

        }
    }
    else
      return;

    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    //glScalef(scale.x,scale.y,scale.z);

//    if(vertL.size()>0)
//    {
//        PPnt* p = get_pnt(21);
//        Pnt p2 = Pnt(*p->x,*p->y,*p->z+5);
//        PPnt* found = FindClosestTo(p2,selvert,20,50);
//        draw_p(IntToStr(p->i),Pnt(*p->x,*p->y,*p->z),Rainbow[2],0);
//        draw_p(IntToStr(found->i),Pnt(*found->x,*found->y,*found->z),Rainbow[1],0);
//    }


    for(unsigned int i=0; i<vertL.size(); i++)
    {
        PPnt* p = vertL[i];

        for(char i=0; i<6; i++)
        {
            if( p->p[i]->i != -1  ) //&& i !=1 && i!=4
            {
                glColor3f(1,1,1);
                glBegin(GL_LINES);
                    glVertex3f(*p->x, *p->y, *p->z);
                    glVertex3f(*p->p[i]->x, *p->p[i]->y, *p->p[i]->z);
                glEnd();
            }
        }

   }

    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();
}

bool MTerrain::FindTerrainObject(string goname)
{
    GemObject* o;
    o = (GemObject*)MREG.DeepFindObj(goname);
    if(o!=0)
    {
        terrain_mesh = o;
        //terrain_mesh->isCanRender = false; // особая отрисовка сетки поверхности
        return true;
    }
    else
        return false;

}

void MTerrain::Slot1()
{
    if( terrain_mesh == 0 )
    {
        if( FindTerrainObject("Terrain01") )
            CreateLinkedLists();
    }

}

void MTerrain::SetBumpImage(QImage& I)
{
    QPair<int,int> idx;

    int dw = I.width() / mprops.w_segs;
    int dl = I.height() / mprops.l_segs;
    for(int k=0; k < mprops.l_segs; k++)
       for(int i=0; i < mprops.w_segs; i++)
       {
           float level = 255. / qBlue(I.pixel(k*dw,i*dl))  ;
           level = level * mprops.sizey;
           idx.first = i;
           idx.second = k;
           PPnt* p = get_pnt(index_map[idx]);
           *p->y = level;
       }
    terrain_mesh->bUpdateMesh = true;
}
