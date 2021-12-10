//---------------------------------------------------------------------------
// TODO: сделать команду set_2pnt и др. с отным смещением
//---------------------------------------------------------------------------


#ifndef geometryH
#define geometryH

//#include <vcl.h>

#include <string>
#include <limits.h>
#include <vector>
#include <stdio.h>

using namespace std;
//---------------------------------------------------------------------------
//                    К О Н С Т А Н Т Ы
//---------------------------------------------------------------------------
const double pi = 3.1415926535897932384626433832795;
// константа для перевода град. в радианы
#define CRAD pi / 180
#define GR 0.01745329251994f   // множитель перевода из градусов в радианы ( градус * GR = радиан)
#define Grad(B) (int)(B/GR)
#define ToGrad(B)  Pnt(B.x/GR,B.y/GR,B.z/GR)
#define ToRad(B)  Pnt(B.x*GR,B.y*GR,B.z*GR)
#define Minute(B) abs( ( B/GR - (int)(B/GR) )* 60. )
#define Second(B) abs( ( B/GR - (int)( B/GR ) ) * 60. - (int)(( B/GR - (int)( B/GR ) ) * 60. ) * 60)
#define sq(a) ((a) * (a))

extern char gstr3[1000000];
//---------------------------------------------------------------------------
//                           Т   И   П   Ы
//---------------------------------------------------------------------------
struct t_range
{
	t_range(){start=0; end=0;}
	int start;
	int end;
};
#define BIT(bit_no)         (1 << (bit_no))
#define CLEAR_BIT(reg, bit_no)   (reg) &= ~BIT(bit_no)
#define SET_BIT(reg, bit_no)   (reg) |= BIT(bit_no)
#define CHECK_BIT(reg, bit_no)   ( (reg) & BIT(bit_no) )
#define SWITCH_BIT(reg, bit_no)   (reg) ^= BIT(bit_no)
//точка в пространстве
class Pnt
{
public:


    Pnt(){x=0;y=0;z=0;}
    Pnt(const Pnt& B) { x = B.x; y = B.y; z = B.z; }
    Pnt(float _x, float _y, float _z) :
	x(_x), y(_y), z(_z) {}
    Pnt* operator->(){return this;}
    Pnt operator*(float s){return Pnt(x*s,y*s,z*s);}
    Pnt operator*(Pnt& B){return Pnt(B.x*x,B.y*y,B.z*z);}
    //Pnt operator*(double B){return Pnt(B*x,B*y,B*z);}
    Pnt operator+(Pnt p){return Pnt(x+p.x,y+p.y,z+p.z);}
    Pnt operator-(Pnt p){return Pnt(x-p.x,y-p.y,z-p.z);}
    bool operator==(Pnt p){return (x == p.x) && (y == p.y) && (z == p.z); }

	bool inRangeOfPnt(Pnt B, float R)
	{
		return( (x+R) > B.x )&&( (y+R) > B.y ) && ((x-R) < B.x )&&( (y-R) < B.y );
	}
	
    //оператор преобразования типа для совместимости с vec3
    //operator vec3() const{return vec3(x,y,z);}
	string toStr()
	{
        sprintf(gstr3,"%f,%f,%f",x,y,z);
		return gstr3;
	}

	char* toCStr()
	{
        sprintf(gstr3,"%f,%f,%f",x,y,z);
		return gstr3;
	}


	string toStr(char* fmt)
	{
        sprintf(gstr3,fmt,x,y,z);
		return gstr3;
	}


	void fromStr(const char* str)
	{
		
        sscanf(str, "%f,%f,%f",&x,&y,&z);
		
	}
	
	bool isNull()
	{
		return x==0 && y==0 && z==0;
	}


    float x,y,z;	
};


class Angles : public Pnt
{
public:
	float& yaw;
	float& roll;
	float& pitch;
	Angles() : Pnt(), yaw(y), pitch(x), roll(z)
	{
	
	}
	Angles(float p, float r, float yw)  : Pnt(), yaw(y), pitch(x), roll(z)
	{
		yaw = yw;
		pitch = p;
		roll = r;	
	}
	void operator=(Pnt& p){ 
		x = p.x; y = p.y; z = p.z; 
	};
	void operator=(Angles& p){ 
		yaw = p.yaw; pitch = p.pitch; roll = p.roll; 
	};

};

class PntEx : public Pnt
{
public:
	int fg;
	PntEx() : Pnt()
	{
	
	}
    PntEx(Pnt p)
	{
		x = p.x; y = p.y;z = p.z;
	}

	void Select()
	{
		SET_BIT(fg, 4);
	}

	void UnSelect()
	{
		CLEAR_BIT(fg, 4);
	}

	bool isSel()
	{
        if( CHECK_BIT(fg, 4))
            return true;
        else
            return false;

	}
    void operator=(Pnt& p){ x = p.x; y = p.y; z = p.z; }
};


class PntExx : public Pnt
{
public:
	int fg;
	float w1;
	float w2;
	PntExx() : Pnt()
	{
	
	}
	PntExx(Pnt& p)
	{
		x = p.x; y = p.y;z = p.z;
	}

	void Select()
	{
		SET_BIT(fg, 4);
	}

	void UnSelect()
	{
		CLEAR_BIT(fg, 4);
	}

	bool isSel()
	{
        if( CHECK_BIT(fg, 4) )
            return true;
        else
            return false;

	}
    void operator=(Pnt& p){ x = p.x; y = p.y; z = p.z; }
};


//Бокс
class Box
{
public:
    Box(){}
    Box(int A, int B, int H) :
    a(A),b(B),h(H)
    {
        points[0] = Pnt(0,0,0);
        points[1] = Pnt((float)a,0,0);
        points[2] = Pnt((float)a,(float)b,0);
        points[3] = Pnt(0,(float)b,0);
        points[4] = Pnt(0,0,(float)h);
        points[5] = Pnt((float)a,0,(float)h);
        points[6] = Pnt((float)a,(float)b,(float)h);
        points[7] = Pnt(0,(float)b,(float)h);
    }
    Pnt points[8];
    int a,b,h;
};

//вектор
class Vector
{
public:
    Vector(){A=Pnt(0,0,0);B=A;}
    Vector(Pnt AA, Pnt BB){A = AA; B = BB;}
    Pnt A, B;
};

// векор в сферической системе
typedef struct
{
    float Angx;
    float Angy;
    float Angz;
} VectorSp;


// вектор
/*class Vector3d
{
    Pnt A, B;
	
    Vector3d(Pnt _A, Pnt _B){}
    // TODO Оформить векторную математику
    Vector3d operator+(Vector3d A) {}
    Vector3d operator*(Vector3d A) {}
    Vector3d operator-(Vector3d A) {}
    Vector3d operator/(Vector3d A) {}
	
	
};
*/

// векор в сферической системе кооринат
typedef struct
{
    float Angx;
    float Angy;
    float Angz;
} VectorS3d;

// полигон
class Polygon
{
public:
	
    Polygon(){}
    std::vector<Pnt> vert;
};


typedef struct tkrug
{
	int xc;
	int yc;     // положение центра
	int R;      // радиус
	int n;      // количество концентрических кругов
	int grow;   // индекс роста (меняется от 0 до 4)
	int id;     // номер круга в списке
	float wReserv1; // зарезервировано
	float wReserv2; // зарезервировано
} tkrug;

typedef struct trange
{
	float min;
	float max;
} trange;

/*typedef struct tvect
{
Pnt A;
Pnt B;
void set_Ax(float x) {A.x = x;}
void set_Ay(float y) {A.y = y;}
void set_Bx(float x) {B.x = x;}
void set_By(float y) {B.y = y;}
} tvect;
*/

class tvect
{
public:
	Pnt A;
	Pnt B;
    tvect(){}
	tvect(Pnt AA, Pnt BB){A=AA;B=BB;}
	void set_Ax(float x) {A.x = x;}
	void set_Ay(float y) {A.y = y;}
	void set_Bx(float x) {B.x = x;}
	void set_By(float y) {B.y = y;}
	void set_A(float x, float y) { A.x = x; A.y = y;}
	void set_B(float x, float y){ B.x = x; B.y = y;}
	Pnt get_A() { return A; }
	Pnt get_B() { return B; }
	
};

typedef struct tpolygon4
{
	Pnt pnt1;
	Pnt pnt2;
	Pnt pnt3;
	Pnt pnt4;
} tpolygon4;

typedef struct trect
{
	float Left;
	float Right;
	float Top;
	float Bottom;
} trect;

void InitGeoumetry();


//--------------------------------------------
//   СФЕРИЧЕСКИЕ КООРДИНАТЫ
//--------------------------------------------
Pnt line_eq(Pnt A, Pnt B, float t);
void decart2polar(Pnt& polar, Pnt decart);
void polar2decart(Pnt& decart, Pnt polar);


//--------------------------------------------
//   О П Е Р А Ц И И   С   В Е К Т О Р А М И
//--------------------------------------------
double dot_product(Pnt A, Pnt B); // скалярное произведение
double len_vec(Pnt p1);			  // длинна вектора
double vect_angle(Pnt A, Pnt B);  // угол между векторами
void translate_vector(tvect* v);    // -- перенос вектора в начало координат
void normalize_vector(tvect* v);
void normalize_vector(Pnt& v);
double vector_anglX(tvect v);       // -- угол между осью ox и вектором
double vector_anglY(tvect v);
void rotateAboutZ(Pnt& p, float theta);
void rotateAboutY(Pnt& p, float theta);
void rotateAboutX(Pnt& p, float theta);
void sum_vectors(tvect v1, tvect v2); // -- сумма векторов по правилу параллелограмма
float rasst(Pnt p1, Pnt p2); // расстояние между точками
int vect_isnull(tvect v);
tvect resize_v(tvect v, float factor); // изменить размер вектрора



//--------------------------------------------
//  П Е Р Е Н О С Ы  И  П О В О Р О Т Ы
//--------------------------------------------
//namespace Affine
//{

//параллельный перенос точки
Pnt translate_p(Pnt& p, float dx, float dy, float dz);

	
void GetEiler(Pnt p, Pnt& ang);
void Eiler2Matrix(float angle_x, float angle_y, float angle_z, float res[3][3]);

    template<typename T>
        void ascaleM(T p, int N, float Sx, float Sy, float Sz)
    {
        int i;
        for(i=0; i < N; i++)
        {
            p[i]->x = p[i]->x * Sx;
            p[i]->y = p[i]->y * Sy;
            p[i]->z = p[i]->z * Sz;
        }

    }

    template<typename T1, typename T2>
        void ascale(T1 dest, T2 source, int N, float Sx, float Sy, float Sz)
    {
        int i;
        for(i=0; i < N; i++)
        {
            dest[i]->x = source[i]->x * Sx;
            dest[i]->y = source[i]->y * Sy;
            dest[i]->z = source[i]->z * Sz;
        }

    }

    template<typename T1, typename T2>
        void atranslate(T1 dest, T2 source, int N, float Tx, float Ty, float Tz)
    {
        int i;
        for(i=0; i < N; i++)
        {
            dest[i]->x = source[i]->x + Tx;
            dest[i]->y = source[i]->y + Ty;
            dest[i]->z = source[i]->z + Tz;
        }

    }

    template<typename T>
        void atranslateM(T p, int N, float Tx, float Ty, float Tz )
    {
        int i;
        for(i=0; i < N; i++)
        {
            p[i]->x = p[i]->x + Tx;
            p[i]->y = p[i]->y + Ty;
            p[i]->z = p[i]->z + Tz;
        }

    }


    // Affine Translate Points in Range (p1, p2)
    template<typename T>
        void atranslate_rangeM(Pnt* p, int i_start, int i_end, int Tx, int Ty, int Tz)
    {
        int i;
        for(i = i_start; i < i_end; i++)
        {
            p[i]->x = p[i]->x + Tx;
            p[i]->y = p[i]->y + Ty;
            p[i]->z = p[i]->z + Tz;

        }
    }



    // Affine Scale  (относительно выбранной точки)
    template<typename T>
        void ascalePM(T p, int N, float Sx, float Sy, float Sz, Pnt C)
    {
        int i;
        atranslateM(p, N, -C.x, -C.y, -C.z);
        for(i=0; i < N; i++)
        {
            p[i]->x = p[i]->x * Sx;
            p[i]->y = p[i]->y * Sy;
            p[i]->z = p[i]->z * Sz;
        }
        atranslateM(p, N, C.x, C.y, C.z);

    }

    // Affine Scale in range
    template<typename T>
    void ascale_rangeM(T p, int i_start, int i_end, float Sx, float Sy, float Sz)
    {
        int i;
        for(i=i_start; i < i_end; i++)
        {
            p[i]->x = p[i]->x * Sx;
            p[i]->y = p[i]->y * Sy;
            p[i]->z = p[i]->z * Sz;
        }

    }


    // поиск макс x
    template<typename T>
    float findXmin(T p, int N)
    {
        int i;
        float xmin = INT_MAX;
        for(i=0; i < N; i++)
        {
            if(p[i]->x < xmin)
                xmin = p[i]->x;
        }

        return xmin;
    }


    // поиск макс x
    template<typename T>
    float findXmax(T p, int N)
    {
        int i;
        float xmax = 0;
        for(i=0; i < N; i++)
        {
            if(p[i]->x > xmax)
                xmax = p->x;
        }
        return xmax;
    }

        // поиск макс y
    template<typename T>
    float findymin(T p, int N)
    {
        int i;
        float ymin = INT_MAX;
        for(i=0; i < N; i++)
        {
            if(p[i]->y < ymin)
                ymin = p[i]->y;
        }

        return ymin;
    }


    // поиск макс y
    template<typename T>
    float findymax(T p, int N)
    {
        int i;
        float ymax = 0;
        for(i=0; i < N; i++)
        {
            if(p[i]->y > ymax)
                ymax = p->y;
        }
        return ymax;
    }

            // поиск макс z
    template<typename T>
    float findzmin(T p, int N)
    {
        int i;
        float zmin = INT_MAX;
        for(i=0; i < N; i++)
        {
            if(p[i]->z < zmin)
                zmin = p[i]->z;
        }

        return zmin;
    }


    // поиск макс z
    template<typename T>
    float findzmax(T p, int N)
    {
        int i;
        float zmax = 0;
        for(i=0; i < N; i++)
        {
            if(p[i]->z > zmax)
                zmax = p->z;
        }
        return zmax;
    }


    // Поиск центра масс фигуры
    template<typename T>
    Pnt findcenter(T p, int N)
    {
        Pnt C0;
        float  xmin = (float)INT_MAX, xmax = -(float)INT_MAX, ymax = -(float)INT_MAX, ymin = (float)INT_MAX, zmax = -(float)INT_MAX, zmin = (float)INT_MAX;
        int i;


        for(i=0; i < N; i++)
        {
            if(p[i]->z < zmin)
                zmin = p[i]->z;
            if(p[i]->z > zmax)
                zmax = p[i]->z;

            if(p[i]->y < ymin)
                ymin = p[i]->y;
            if(p[i]->y > ymax)
                ymax = p[i]->y;

            if(p[i]->x < xmin)
                xmin = p[i]->x;
            if(p[i]->x > xmax)
                xmax = p[i]->x;

        }

        C0.x = xmin + (xmax - xmin) / 2;
        C0.y = ymin + (ymax - ymin) / 2;
        C0.z = zmin + (zmax - zmin) / 2;

        return C0;
    }



    // Affine Rotate (с переносом в геометрический центр )
    template<typename T>
    void arotateCM(T p, int N, Pnt rot)
    {
        int i;
        Pnt C;

        C = findcenter(p, N);

        atranslateM(p, N, -C.x, -C.y, -C.z);
#if __WIN32
        for(i = 0; i < N; i++)
        {
            //prn("bf:%s",p[i]->toStr().c_str());

            if(rot.z > 0.001)
            {
                p[i]->x  = p[i]->x * cos(GR * rot.z) + p[i]->y * sin(GR * rot.z);
                p[i]->y  = -p[i]->x * sin(GR * rot.z) + p[i]->y * cos(GR * rot.z);

            }
            if(rot.y > 0.001)
            {
                p[i]->x  = p[i]->x * cos(GR * rot.y) - p[i]->z * sin(GR * rot.y);
                p[i]->z  = p[i]->x * sin(GR * rot.y) + p[i]->z * cos(GR * rot.y);

            }
            if(rot.x> 0.001)
            {
                p[i]->y  = p[i]->y * cos(GR * rot.x) + p[i]->z * sin(GR * rot.x);
                p[i]->z  = -p[i]->y * sin(GR * rot.x) + p[i]->z * cos(GR * rot.x);

            }

            //prn("af:%s",p[i]->toStr().c_str());

        }
#endif
        atranslateM(p, N, C.x, C.y, C.z);
    }

    // Affine Rotate (относительно начала координат)
    template<typename T>
    void arotateM(T p, int N, Pnt rot)
    {
        int i;


        for(i = 0; i < N; i++)
        {
            //prn("bf:%s",p[i]->toStr().c_str());
#if __WIN32
            if(rot.z > 0.001)
            {
                p[i]->x  = p[i]->x * cos(GR * rot.z) + p[i]->y * sin(GR * rot.z);
                p[i]->y  = -p[i]->x * sin(GR * rot.z) + p[i]->y * cos(GR * rot.z);

            }
            if(rot.y > 0.001)
            {
                p[i]->x  = p[i]->x * cos(GR * rot.y) - p[i]->z * sin(GR * rot.y);
                p[i]->z  = p[i]->x * sin(GR * rot.y) + p[i]->z * cos(GR * rot.y);

            }
            if(rot.x> 0.001)
            {
                p[i]->y  = p[i]->y * cos(GR * rot.x) + p[i]->z * sin(GR * rot.x);
                p[i]->z  = -p[i]->y * sin(GR * rot.x) + p[i]->z * cos(GR * rot.x);

            }
#endif
            //prn("af:%s",p[i]->toStr().c_str());

        }

    }



    // Affine Rotate (относительно точки)
    template<typename T>
    void arotatePM(T p, int N, float alfa, Pnt rot, Pnt C)
    {
        int i;


        atranslate(p, N, -C.x, -C.y, -C.z);

        for(i = 0; i < N; i++)
        {
            //prn("bf:%s",p[i]->toStr().c_str());
#if __WIN32
            if(rot.z > 0.001)
            {
                p[i]->x  = p[i]->x * cos(GR * rot.z) + p[i]->y * sin(GR * rot.z);
                p[i]->y  = -p[i]->x * sin(GR * rot.z) + p[i]->y * cos(GR * rot.z);

            }
            if(rot.y > 0.001)
            {
                p[i]->x  = p[i]->x * cos(GR * rot.y) - p[i]->z * sin(GR * rot.y);
                p[i]->z  = p[i]->x * sin(GR * rot.y) + p[i]->z * cos(GR * rot.y);

            }
            if(rot.x> 0.001)
            {
                p[i]->y  = p[i]->y * cos(GR * rot.x) + p[i]->z * sin(GR * rot.x);
                p[i]->z  = -p[i]->y * sin(GR * rot.x) + p[i]->z * cos(GR * rot.x);

            }
#endif
            //prn("af:%s",p[i]->toStr().c_str());

        }

        atranslate(p, N, C.x, C.y, C.z);
    }

	

//перевод из полярных координат в декаровые
Pnt polar2dec(float alfa, float r);

bool point_inside_box(Pnt bounds, Pnt C0, Pnt p );
bool intersect_boxes(Pnt bounds1, Pnt C01,
					 Pnt bounds2, Pnt C02);

//---------------------------------------------------------------------------
#endif
