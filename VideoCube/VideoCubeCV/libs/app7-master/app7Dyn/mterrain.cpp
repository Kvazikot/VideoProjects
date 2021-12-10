#include <tuple>
#include "../include/cobjregistry.h"
#include "../include/mterrain.h"
#include "../include/material.h"
#include "../include/fractalterrain.h"
#include "../include/glcode.h"
#include "../include/print.h"

Pnt PPnt::null_p;
static PPnt nullp;
static CObjRegistry& MREG = CObjRegistry::getInstance2();

static uint Rainbow[6]=		// Rainbow Of Colors
{
    RGB(255,0,0),RGB(0,255,0),RGB(0,0,255),RGB(0,0,0),RGB(11,255,0),RGB(0,0,0)

};



MTerrain::MTerrain()
{
    terrain_mesh = 0;
    isCanRender = true;
    scale = Pnt(0.01f,0.01f,0.01f);
    selvert = 73;
    var = 0;
}

void addface(std::vector<GLushort>& indexes, int fcount, int A, int B, int C)
{
    indexes[3*fcount] =  B;
    indexes[3*fcount+1] =  A;
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

void MTerrain::Create(int w_segs, int h_segs, int l_segs, float sizex, float sizey, float sizez, float ylevel)
{
    isCanRender = false;

    this->ylevel = ylevel;
    if(terrain_mesh==0)
    {
        terrain_mesh = new GeometryObject();
        CMaterial& mater = CMaterial::getInstance();
        terrain_mesh->ei.texture_id = mater.FindTexture("textures/cube.png");
        qDebug() << "terrain_mesh->ei.texture_id " << terrain_mesh->ei.texture_id;
    }

    w_segs++; h_segs++; l_segs++;
    int n_vert = (w_segs) * (h_segs) * (l_segs);
    terrain_mesh->SetNumVert(n_vert);
    int n_faces = w_segs * h_segs * l_segs * 12;
    terrain_mesh->SetNumFaces(n_faces);

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

                terrain_mesh->vertices[vcount].position = QVector3D( (float)i * dw - sizex/2., (float)j * dh - sizey/2. , (float)k * dl - sizez/2.);
                //terrain_mesh->vertices[vcount].texCoord = QVector2D((float)rand()/RAND_MAX, (float)rand()/RAND_MAX);
                //terrain_mesh->vertices[vcount].position = QVector3D(1,2,3);
                terrain_mesh->vertices[vcount].texCoord = QVector2D(((float)rand()/RAND_MAX), ((float)rand()/RAND_MAX));
                if( j == h_segs-1 )
                {
                    QPair<int,int> idx;
                    idx.first = i;
                    idx.second = k;
                    index_map[idx] = vcount;
                    reverse_index_map[vcount] = idx;
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
                addface(terrain_mesh->indices, fcount, A, B, D); fcount++;
                addface(terrain_mesh->indices, fcount, D, B, C); fcount++;
                addface(terrain_mesh->indices, fcount, A, B, F); fcount++;
                addface(terrain_mesh->indices, fcount, F, B, G); fcount++;
                addface(terrain_mesh->indices, fcount, A, F, D); fcount++;
                addface(terrain_mesh->indices, fcount, D, M, F); fcount++;
                addface(terrain_mesh->indices, fcount, F, G, M); fcount++;
                addface(terrain_mesh->indices, fcount, M, G, H); fcount++;
                addface(terrain_mesh->indices, fcount, D, C, H); fcount++;
                addface(terrain_mesh->indices, fcount, D, M, H); fcount++;
                addface(terrain_mesh->indices, fcount, B, C, G); fcount++;
                addface(terrain_mesh->indices, fcount, C, G, H); fcount++;
            }

    terrain_mesh->bUpdateMesh = true;
    CreateLinkedLists();
    //AssignTextureCoords();
    AssignTextureCoords2();
    AssignNormals();
    terrain_mesh->init();
    terrain_mesh->isCanRender = true;
    isCanRender = true;

}

PPnt* MTerrain::set_pnt(PPnt* p, int i)
{
    p->i = i;
    p->x = ((float*)&terrain_mesh->vertices[i].position);
    p->y = (((float*)&terrain_mesh->vertices[i].position + 1));
    p->z = ((float*)&terrain_mesh->vertices[i].position + 2);
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
    PPnt* minP = &nullp;
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
    vertL.resize( terrain_mesh->GetNumVert() );
    //создать массив указателей
    for(int i = 0; i < terrain_mesh->GetNumVert(); i++ )
    {
        vertL[i] = new PPnt();
        for(int j=0; j<7; j++)
            vertL[i]->p[j] = new PPnt();
    }

    PPnt* start_v = vertL[0];
    start_v->i = 0;
    start_v->x = ((float*)&terrain_mesh->vertices[0]);
    start_v->y = ((float*)&terrain_mesh->vertices[0] + 1);
    start_v->z = ((float*)&terrain_mesh->vertices[0] + 2);

    nullp.x = start_v->x;
    nullp.y = start_v->y;
    nullp.z = start_v->z;

    //заполнить список вершин
    for(int i = 1; i < terrain_mesh->GetNumVert(); i++ )
    {
        PPnt* p = vertL[i];
        set_pnt(p, i);
    }

    for(int i = 0; i < terrain_mesh->GetNumVert(); i++ )
    {
        PPnt* p = get_pnt(i);
        //qDebug("%d %f %f %f", p->i,*p->x, *p->y, *p->z);
    }
    //scale
    if(scale.x!=0)
    for(int i = 0; i < terrain_mesh->GetNumVert(); i++ )
    {
        PPnt* p = vertL[i];
        *p->x = scale.x * *p->x;
        *p->y = scale.y * *p->y + ylevel;
        *p->z = scale.z * *p->z;
    }


    //заполнить соседей
    for(int i = 0; i < terrain_mesh->GetNumFaces(); i++ )
    {
        int i2 = terrain_mesh->indices[i*3];
        int i1 = terrain_mesh->indices[i*3+1];
        int i3 = terrain_mesh->indices[i*3+2];
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


void MTerrain::Render(QOpenGLShaderProgram& program)
{
    static bool ot = true;
    if(isCanRender==false) return;
/*
    if(ot)
    {
        //загрузка текстур
        CMaterial& mater = CMaterial::getInstance();
        mater.LoadTextures();
        ot = false;
    }
*/

    if(terrain_mesh!=0)
    {
        if(terrain_mesh->isCanRender)
        {
            // Use texture unit 0 which contains cube.png
            program.setUniformValue("texture", 0);
            program.setUniformValue("RenderMode", 2 );
            program.setUniformValue("n_ranges",(int)0);
            program.setUniformValue( "material.Ka", QVector3D( 0.6f, 0.6f, 0.6f ) );
            program.setUniformValue( "material.Kd", QVector3D( 1.0f, 1.0f, 1.0f ) );
            program.setUniformValue( "material.Ks", QVector3D( 0.4f, 0.4f, 0.4f ) );
            program.setUniformValue( "material.shininess", 50.0f );
            //glBindTexture(GL_TEXTURE_2D, terrain_mesh->ei.texture_id);

            terrain_mesh->Render(program);
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

void MTerrain::Slot1()
{

}

void MTerrain::SendToGPU()
{
    terrain_mesh->isCanRender = false;
    terrain_mesh->update();
    terrain_mesh->isCanRender = true;
}

void MTerrain::SetBumpImage(QImage& I, float h)
{
    QPair<int,int> idx;
    bump_texture = I;
    QTransform transform;
    transform.rotate(180,Qt::XAxis);
    transform.rotate(90,Qt::ZAxis);
    I = I.transformed(transform);
    double dw = ((double)I.width()) / ((double)mprops.w_segs);
    double dl = ((double)I.height()) / ((double)mprops.l_segs);
    prn("I.width=%d I.height=%d",I.width(),I.height());
    int offset =  0;//100 / mprops.w_segs;
    for(int k=0; k < mprops.w_segs; k++)
       for(int i=0; i < mprops.l_segs; i++)
       {
           float level = qBlue(I.pixel(qRound((double)k*dw),qRound((double)i*dl))) / 255  ;
           if(k==0 || i==0)
           qDebug("access pixel %d %d",qRound((double)k*dw),qRound((double)i*dl));
           if( level > 0 )
           {
               level = level * h;
               idx.first = i+offset;
               idx.second = k+offset;
               PPnt* p = get_pnt(index_map[idx]);
               p->w[0] = -1; // selected vertex
               //terrain_mesh->vertices[vcount].texCoord = QVector2D((float)rand()/RAND_MAX, (float)rand()/RAND_MAX);
               *p->y = *p->y + level;
           }
       }

    AssignTextureCoords2();
    AssignNormals();
    terrain_mesh->bUpdateMesh = true;
}

bool MTerrain::isGround(PPnt* A, PPnt* B, PPnt* C, PPnt* D)
{
    return (*A->y < mprops.sizey) && (*B->y < mprops.sizey) && (*C->y < mprops.sizey) && (*D->y < mprops.sizey);
}

QVector3D pnt2Vec(PPnt* p)
{
    return QVector3D(*p->x,*p->y,*p->z);
}

void calcNormal(PPnt* A, PPnt* B, PPnt* C)
{
    QVector3D v1(pnt2Vec(B) - pnt2Vec(A));
    QVector3D v2(pnt2Vec(C) - pnt2Vec(A));
    QVector3D normal = v1.crossProduct(v1,v2).normalized();
    A->setNormal(normal.x(),normal.y(),normal.z());
    B->setNormal(normal.x(),normal.y(),normal.z());
    C->setNormal(normal.x(),normal.y(),normal.z());
    //A->setNormal((float)rand()/RAND_MAX,(float)rand()/RAND_MAX,(float)rand()/RAND_MAX);
}

void MTerrain::AssignNormals()
{
    for(int i=0; i < terrain_mesh->indices.size()-3; i+=3)
    {
        int i1 = terrain_mesh->indices[i];
        int i2 = terrain_mesh->indices[i+1];
        int i3 = terrain_mesh->indices[i+2];
        PPnt* A = get_pnt(i1);
        PPnt* B = get_pnt(i2);
        PPnt* C = get_pnt(i3);
        calcNormal(A,B,C);
    }

}

void MTerrain::AssignTextureCoords2()
{
    float ds = 2. / (float)mprops.l_segs;
    float dt = 2. / (float)mprops.w_segs;

    float s,t;
    t = 0;

    prn("l=%d w=%d", mprops.l_segs, mprops.w_segs);
    for(int k=0; k < mprops.l_segs; k+=2)
    {
       s = 0;
       for(int i=mprops.w_segs; i > 0 ; i-=2)
       //for(int i=0; i < mprops.w_segs ; i+=2)
       {

           PPnt* A = get_pnt(index_map[qMakePair(i,k)]);
           PPnt* B = get_pnt(index_map[qMakePair(i+1,k)]);
           PPnt* C = get_pnt(index_map[qMakePair(i,k+1)]);
           PPnt* D = get_pnt(index_map[qMakePair(i+1,k+1)]);

           *A->getTexCoord0() = s;      *A->getTexCoord1() = t;
           *B->getTexCoord0() = s - ds; *B->getTexCoord1() = t;
           *C->getTexCoord0() = s;      *C->getTexCoord1() = t + dt;
           *D->getTexCoord0() = s - ds; *D->getTexCoord1() = t + dt;
           s+=ds;
       }
       t+=dt;
    }

    return;
}

void MTerrain::AssignTextureCoords()
{
    QVector2D facet[] = {QVector2D(0.0f, 0.0f), QVector2D(0.33f, 0.0f), QVector2D(0.0f, 0.5f),  QVector2D(0.33f, 0.5f) }; // v3
    QVector2D facet2[] = {QVector2D( 0.33f, 0.0f), QVector2D(0.66f, 0.0f), QVector2D(0.33f, 0.5f), QVector2D(0.66f, 0.5f)}; // v3
    QPair<int,int> idx;

    for(int k=0; k < mprops.l_segs; k+=2)
       for(int i=0; i < mprops.w_segs; i+=2)
       {

           PPnt* A = get_pnt(index_map[qMakePair(i,k)]);
           PPnt* B = get_pnt(index_map[qMakePair(i+1,k)]);
           PPnt* C = get_pnt(index_map[qMakePair(i,k+1)]);
           PPnt* D = get_pnt(index_map[qMakePair(i+1,k+1)]);
           *A->getTexCoord0() = facet2[0].x(); *A->getTexCoord1() = facet2[0].y();
           *B->getTexCoord0() = facet2[1].x(); *B->getTexCoord1() = facet2[1].y();
           *C->getTexCoord0() = facet2[2].x(); *C->getTexCoord1() = facet2[2].y();
           *D->getTexCoord0() = facet2[3].x(); *D->getTexCoord1() = facet2[3].y();

       }

    return;
    //проход по соседям
    for(int i = 1; i < terrain_mesh->GetNumVert(); i++ )
    {
        PPnt* p = vertL[i];
        if( p->w[0] == -1)
        {
            *p->getTexCoord0() = facet[0].x();
            *p->getTexCoord1() = facet[0].y();
            p->w[0] = 0;
            QPair<int,int> idx, idx2;
            idx = reverse_index_map[p->i];
            idx2 = idx;
            //1
            idx2.first = idx.first+1;
            PPnt* p1 = get_pnt(index_map[idx2]);
            *p1->getTexCoord0() = facet[1].x();
            *p1->getTexCoord1() = facet[1].y();
            p1->w[0] = 0;
            idx2 = idx;
            //2
            idx2.second = idx.second+1;
            PPnt* p2 = get_pnt(index_map[idx2]);
            *p2->getTexCoord0() = facet[2].x();
            *p2->getTexCoord1() = facet[2].y();
            p2->w[0] = 0;
/*
            //3
            idx2 = idx;
            idx2.first = idx.first-1;
            p1 = get_pnt(index_map[idx2]);
            *p1->getTexCoord0() = facet2[0].x();
            *p1->getTexCoord1() = facet2[0].y();
             p1->w[0] = 0;
             //4
             idx2 = idx;
             idx2.second = idx.second-1;
             p2 = get_pnt(index_map[idx2]);
             *p2->getTexCoord0() = facet2[1].x();
             *p2->getTexCoord1() = facet2[1].y();
             p2->w[0] = 0;
*/


        }

        for(int i=0; i < 7; i++)
        {
            if( p->p[i]->i!=-1 )
            {
                if(*p->p[i]->getTexCoord0()!=0)
                {
                    //*p->p[i]->getTexCoord0() =
                }
                //*p->p[i]->getTexCoord1() = (float)rand()/RAND_MAX;
            }
        }
    }
}


void MTerrain::heightMapFromMesh()
{
    float meanY=0;
    for(int i = 0; i < terrain_mesh->vertices.size()-1; i++ )
    {
       PPnt* p = vertL[i];
       meanY+=*p->y;
    }
    meanY=meanY/terrain_mesh->vertices.size();
    qDebug("meanY=%f", meanY);

    height_map = QImage(terrain_mesh->bounds.x*10, terrain_mesh->bounds.z*10, QImage::Format_ARGB32);
    QPainter painter(&height_map);
    for(int i = 0; i < terrain_mesh->indices.size(); i++ )
    {
        int i1 = terrain_mesh->indices[i*3];
        int i2 = terrain_mesh->indices[i*3+1];
        int i3 = terrain_mesh->indices[i*3+2];
        float y_average = fabs(2*meanY) + (*get_pnt(i1)->y + *get_pnt(i2)->y + *get_pnt(i3)->y) / 3;
        QPointF p1(*get_pnt(i1)->x + terrain_mesh->bounds.x/2 , *get_pnt(i1)->z + terrain_mesh->bounds.z/2);
        QPointF p2(*get_pnt(i2)->x + terrain_mesh->bounds.x/2 , *get_pnt(i2)->z + terrain_mesh->bounds.z/2);
        QPointF p3(*get_pnt(i3)->x + terrain_mesh->bounds.x/2 , *get_pnt(i3)->z + terrain_mesh->bounds.z/2);
        QPolygonF     poly;
        poly.append(p1*10);        poly.append(p2*10);    poly.append(p3*10);
        QPainterPath path;
        painter.setPen(QColor(int(5*y_average)));
        painter.setBrush(QColor(int(5*y_average)));
        path.addEllipse(p2*10,11,11);
        path.addEllipse(p3*10,11,11);
        path.addEllipse(p1*10,11,11);
        painter.fillPath(path,painter.brush());

        //qDebug("y_average=%f %d",y_average, int(10*y_average));

    }
    //height_map.save("height_map.png");


}

