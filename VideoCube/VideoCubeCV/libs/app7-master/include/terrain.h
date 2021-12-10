#ifndef Terrain_H
#define Terrain_H
#include "group.h"
#include "gemobject.h"
#include "vpnt.h"
#include "interp.h"
#include "terrain_params.h"

// выбранные точки
struct PointSelection
{
	vector<PntExx> pts;
    uint	 rgb;
	string   name;
	float	 dr; // расстояние от центра
    void	 Render(QOpenGLShaderProgram& program);
};

// связанный список вершин
struct PPnt
{ 
    static  Pnt null_p;
	int		i;
	float   w[4];
	float* x;	float* y;	float* z; 
	PPnt()	{	
        i = -1; x = &null_p.x; y = &null_p.y; z = &null_p.z;
	}
    PPnt* p[7];
    float* getTexCoord0()
    {
        return (float*)z + 1;
    }
    float* getTexCoord1()
    {
        return (float*)z + 2;
    }
    void setNormal(float xx,  float yy, float zz)
    {
        *(float*)(z + 3) = xx;
        *(float*)(z + 4) = yy;
        *(float*)(z + 5) = zz;
    }
    void AddNeib(PPnt* neib)
    {
        bool has = false;
        for(char i=0; i < 7; i++)
        {
            if(p[i]->i==neib->i)
                has = true;
        }
        if(!has)
        {
            for(char i=0; i < 7; i++)
            {
                if(p[i]->i==-1)
                {
                    p[i] = neib;
/*                    p[i]->i = neib->i;
                    p[i]->x = neib->x;
                    p[i]->y = neib->y;
                    p[i]->z = neib->z;
*/
                    break;
                }

            }
        }
    }
};

//связанный список ребер
struct PEdge
{
	int		i;
	int     fg;
	PEdge()	{	i=-1; fg=0;}
	PPnt* p[2];
};

// связанный список граней
struct PFace
{
	int i;
	PPnt* p[4];
	PFace* f[3];
};

struct t_rangeE
{
	t_rangeE(){start=0; end=0;}
	int start;
	int end;
	int what; // 0 - vertex range 1 - faces range
};


// мастер-обьект
extern string ter_filters;

class Terrain :public Group
{
//    Q_OBJECT
public:

	Terrain();
    Terrain(const Terrain& B)
	{
        N=B.N;
	}
	void DrawGraf();
	void GenerateTerrain( GemObject* terrain_mesh, Terrain_params& ter_params );
	void GenerateTerrain2( GemObject* terrain_mesh, Terrain_params& ter_params );
	void DoApproximation(GemObject* mesh);
    void SoftRender(QOpenGLShaderProgram& program);
	void InitPallete();

	// обработка поверхности
	void CreateLinkedLists();
	void HighlightFaces(int i_start, int i_end, int what);
	void SelectFacesByVetex(int i);
	void SelectNeibVertices(int i);
	void FillMidPoints( vector<int>& src, vector<int>& dst);
    void FindClosestFace(Pnt p[4], Pnt to_p, int from_idx,  int count, float delta);
	void FixRange(int& i);
	void SelectFacesBySphere( Pnt C0, float R );
	PPnt* get_pnt(int i);
	Pnt	  get_p(int i);
	PPnt* set_pnt(PPnt* p, int i);
	void set_pnt_neib(PPnt* p, int i, int f);
	void get_face(PPnt* p, int i, int f);

	//			NEW
	
	char GetFreeVert(PPnt p); // 0100
	void SelFreeVert(PPnt p);
	void MakeSelection(int start_v, char contour[100][2], vector<PPnt>& sel); // выбрать вершины по контуру
	void DilateSelection(vector<PPnt>& sel);								  // расширить выделение
	void DilateContour(int n_iter, char contour[100][2]);					  // расширить контур
	void ContourFromString(string str, char contour[100][2]);				  // контур из ASCII строки
	void SelectVertexesAround(int center_idx, int count, vector<int>& arr);
	void SelectVertexesAround(int center_idx, float R, int WeithFunc, vector<int>& arr);
	void SelectVertexesAroundTest(int i, int cnt);
	void SelectContour(int center_idx, char arr[100][100], int w, int h, vector<int>& selection);
	void MakeHolm(int center_idx, float H, float R);
    void BrounovskLine();
	PPnt*  FindClosestTo(Pnt p, int from_idx,  int count, float delta);
	//int  FindNearestIdx(Pnt& p);
	PPnt*  FindNearestP(Pnt& p);
	bool   IsNeib(int i1, int i2);
	bool   IsIdxValid(int i);


	vector<PPnt*>   vertL;
	vector<PFace*>   faceL;
	unsigned int*  faces;								// массив фейсов квадратной сетки (ТОЛЬКО ДЛЯ ОТРИСОВКИ)

	// ТИП ГЕНЕРАТОРА 
	enum    GEN_TYPE {NURBS_GEN, SOFTSEL_GEN};			
	// ТИП РЕНДЕРА
	enum    REN_TYPE {SOFT_REN, WIRE, FACES};			
	// ТИП ВЕСОВОЙ ФУНКЦИИ
	enum    WEIGHT_FUN{W_LIN, W_EXP, W_QUAD, W_SQRT};

	GLfloat color[4];
	int		n_filter;
	int		NSelTerrain;
	int		Nspl;						// колво сплайнов в патче
	int		N4faces;
	int	    N;						   // колво вершин		
	int		f;						  // фактор сетки
	GEN_TYPE	 Ngenerator;			
	REN_TYPE	 Nrender;		
	bool	 bGenerated;
	double   Rmax;
	vector<PointSelection>	sels;		// выбранные вершины
	Pnt			  highP;				// подсветка фейсов
    Pnt				 _face[4];

	Terrain_params cur_terrain;	 	
	Terrain_params terains[20];			// параметры местности
	Surface_params surface_params;		// математические параметры генерируемой поверхности
	GemObject* terrain_mesh;
	string	   terrains_list;
	string	   contour_list;
    string     null_string;
	t_col		pal[11];
	vector<t_rangeE> selranges;
	vector<int>		 selvertices; string v_idxs;	// список выделенных вершин
	vector<int>		 selfaces;	  string f_idxs;	// список выделенных фейсов
	float			 v_idx, f_idx, v_idx2;
	Pnt				 MakeHolmARG;
	bool			 bShowWeghts;

	void Declare()
	{
		classname = "Terrain";
	
		if(name=="")
		  SetName("terrain");
		
		SET_OBJECT(this,(char*)name.c_str());
        SET_ATTR("Filter", (void*)&ter_filters, "selector");
        SET_ATTR("Template", (void*)&terrains_list, "selector");
		SET_ATTR("Ngenerator", &Ngenerator, "int");
		SET_ATTR("Nrender", &Nrender, "int");
        SET_ATTR("GENERATE", &null_string, "func"); atrs.back().ReadOnly(); atrs.back().NotSaving();
		SET_ATTR("HighlightFaces", &highP, "Pnt"); 
		SET_ATTR("v_idxs", &v_idxs, "string"); 
		SET_ATTR("f_idxs", &f_idxs, "string"); 
		SET_ATTR("NeiboursOfVertex", &v_idx, "float"); 
		SET_ATTR("NeiboursAround",  &v_idx2,  "float"); 
		SET_ATTR("MakeHolm",  &MakeHolmARG,  "Pnt"); 
        SET_ATTR("ClearSel",  &null_string,  "func");
		SET_ATTR("showWeghts",  &bShowWeghts,  "bool"); 
        SET_ATTR("brounovskSquare",&null_string,"func");

        //SET_ATTR("Контур", (void*)&contour_list, "selector");
		//SET_ATTR("Right", &b,  "bool"); 
		//SET_ATTR("Up",    &b,  "bool"); 
		//SET_ATTR("Down",  &b,  "bool"); 

		//SET_ATTR("FacesByVertex", &v_idx, "float"); 

		FillEntetyInfo("Geometry","no material",name);

		bHideStdAttrs = true;
		Group::Declare();
		Object::Declare();
	}
	//void mouse_move(int x, int y);
    void Render(QOpenGLShaderProgram& program);
	void OnSelect();
	void OnUnselect();
	void OnAttrChange(t_atr& a);
    void Slot1(); //timer slot
	void SelectAtrSet(int idx);

protected:

};



struct SearchVolume
{
	Pnt Co;
	float sx; 
	float sy;
	float sz;
};
#endif 

