#include "../include/terraindlg.h"
#include "ui_terraindlg.h"
#include "../include/mterrain.h"
#include "../include/cobjregistry.h"
#include "linuxoid.h"
#include "../include/str_utils.h"
#include "../include/mazegen.h"
#include "../include/print.h"

MTerrain* mterrain;
static CObjRegistry& MREG = CObjRegistry::getInstance2();

TerrainDlg::TerrainDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TerrainDlg)
{
    ui->setupUi(this);
    //mterrain = new MTerrain();
    //mterrain->scale = Pnt(1,1,1);
    //MREG.AddObj(mterrain);
}

void TerrainDlg::OnObjsLoad()
{
    vector<Object*> find_results;
    find_results = MREG.FindObjByClass("MTerrain");
    if(find_results.size() > 0)
    {
       mterrain = (MTerrain*)find_results[0];
    }
    else
        mterrain = 0;
    on_generateMaze_clicked();
}

TerrainDlg::~TerrainDlg()
{
    delete ui;
}

void TerrainDlg::on_createButton_clicked()
{
    if(mterrain)
    mterrain->Create(ui->widthSegs->value(),
                     ui->HeightSegs->value(),
                     ui->LengthSegs->value(),
                     ui->SizeX->value(),
                     ui->SizeY->value(),
                     ui->SizeZ->value(),
                     ui->ylevel->value());
}

void TerrainDlg::on_generateButton_clicked()
{
    if(mterrain)
    mterrain->Generate(ui->detail->value(), ui->Roughness->value(),ui->hconst->value());
}

inline bool checkColor( QImage& image, int x, int y)
{
    return qBlue(image.pixel(x,y)) == 0;
}

inline bool checkColorH( QImage& image, int x0, int y0, int x1)
{
    bool noBlue=true;
    for(int i=x0; i < x1; i++)
        if(qBlue(image.pixel(i,y0)) != 0)
            noBlue=false;
    return noBlue;
}

inline bool checkColorV( QImage& image, int y0, int x0, int y1)
{
    bool noBlue=true;
    for(int i=y0; i < y1; i++)
        if(qBlue(image.pixel(x0,i)) != 0)
            noBlue=false;
    return noBlue;
}

void pixels_to_world(int xpix, int ypix, float& xworld, float& zworld, QImage& image, float pixels_per_unit)
{
    xworld = (image.width()/2 - xpix)/pixels_per_unit;
    zworld = (image.height()/2 - ypix)/pixels_per_unit;
}

bool TerrainDlg::fit_coin(Pnt bounds, float X, float Z, QImage& image, float pixels_per_unit)
{
    int x = image.width()/2 -  X * pixels_per_unit;
    int z = image.height()/2 - Z * pixels_per_unit;

    float bndx = (bounds.x/2) * pixels_per_unit;
    float bndz = (bounds.z/2) * pixels_per_unit;
    bool b1 = checkColor(image,  x, z);
    bool b2 = checkColorH(image, x-bndx, z-bndz, x+bndz );
    bool b3 = checkColorV(image, z-bndz, x+bndx, z+bndz );
    bool b4 = checkColorV(image, z-bndz, x-bndx, z+bndz );
    bool b5 = checkColorH(image, x-bndx, z+bndz, x+bndx );
    return b1 && b2 && b3 && b4 && b5;
}

#define Random11()  (1. - 2.*(double)rand()/RAND_MAX)

GemObject* create_coin(string nominal, float X, float Z)
{
    GemObject* coin = new GemObject();
    coin->CopyMesh(nominal);
    coin->CenterModel();
    coin->Declare();
    coin->update();
    coin->ei.coord.x = X;
    coin->ei.coord.z = Z;
    coin->isCanRender = true;
    coin->model_matrix.setToIdentity();
    coin->model_matrix.translate(X, 0, Z);
    return coin;
}

void TerrainDlg::create_n_coins(string nominal, int N, Group* group, Pnt bounds, QImage& image, float pixels_per_unit )
{
    int n=0;
    while( n!=N )
    {
        int i = rand() % ui->cellsX->value();
        int j = rand() % ui->cellsY->value();
        int cell_size = ui->SizeX->value() / ui->cellsX->value();
        //float X = Random11() * ui->SizeX->value()/2;
        //float Z = Random11() * ui->SizeZ->value()/2;
        float X =  ui->SizeX->value()/2 - i * cell_size +  Random11()*cell_size;
        float Z =  ui->SizeZ->value()/2 - j * cell_size +  Random11()*cell_size;;
        if( fit_coin(bounds, X, Z, image, pixels_per_unit) )
        {
           group->AddObj(create_coin(nominal,X,Z));
           n++;
        }
        //prn("X = %f Z = %f",X,Z);
    }
}

void TerrainDlg::create_n_coins2(string nominal, int N, Group* group, MazeGen* maze)
{
    int n=0;
    while( n!=N )
    {
        int i = rand() % ui->cellsX->value();
        int j = rand() % ui->cellsY->value();
        Cell* cell = maze->getCell(i,j);
        if( cell->value == 0)
        {
            vector<string> parts = StrSplitE(nominal,"_",false);
            cell->value = StrToInt(parts[0]);
            cell->object = create_coin(nominal,cell->xworld,cell->zworld);
            group->AddObj( cell->object );
            n++;
        }
        //prn("X = %f Z = %f",X,Z);
    }
}


Group* TerrainDlg::createCoins(int Ncoins, QImage& image, MazeGen* maze)
{
     GemObject* ruble1 = (GemObject*)MREG.DeepFindObj("1_ruble");
     if(!ruble1) return 0;

     Group* original_group = (Group*)MREG.DeepFindObj(ruble1->groupname);
     original_group->isCanRender = false;

     int N10rubles = Ncoins / 2;
     int N1rubles = Ncoins / 4;
     int N100rubles = Ncoins / 8;
     int N500rubles = Ncoins / 8;

     Group* group;
     group = (Group*)MREG.FindObj("coins_group");
     if(group == 0)
     {
        group = new Group();
        group->SetName("coins_group");
        group->Declare();
        group->isCanRender = true;
        MREG.AddObjNew(group);
    }

     float pixels_per_unit = image.width() / ui->SizeX->value();
     // распределяем 10 рублевые
     //for(int i=0; i < N100rubles; i++)

     //create_n_coins("10_ruble",  N10rubles, group, ruble1->bounds, image, pixels_per_unit);
     //create_n_coins("1_ruble",   N1rubles, group, ruble1->bounds, image, pixels_per_unit);
     //create_n_coins("100_ruble", N100rubles, group, ruble1->bounds, image, pixels_per_unit);
     //create_n_coins("500_ruble", N500rubles, group, ruble1->bounds, image, pixels_per_unit);
     create_n_coins2("10_ruble",  N10rubles, group,  maze );
     create_n_coins2("1_ruble",  N1rubles, group,  maze );
     create_n_coins2("500_ruble",  N500rubles, group,  maze );
     create_n_coins2("100_ruble",  N100rubles, group,  maze );

     return group;
}

void TerrainDlg::on_generateMaze_clicked()
{
    if(mterrain==0) return;
    QImage image(ui->cellsX->value()*30, ui->cellsY->value()*30, QImage::Format_RGB32);
    image.fill(Qt::black);
    mazegen = new MazeGen(image, ui->cellsX->value(), ui->cellsY->value(), ui->wall_width->value());
    mazegen->CreateMaze();
    image.save("maze.png");
    mterrain->Create(ui->widthSegs->value(),
                     ui->HeightSegs->value(),
                     ui->LengthSegs->value(),
                     ui->SizeX->value(),
                     ui->SizeY->value(),
                     ui->SizeZ->value(),
                     ui->ylevel->value());
    mterrain->SetBumpImage(image, ui->wall_height->value());
    mterrain->SendToGPU();

    //convert pixel coordinate to world
    float pixels_per_unit = image.width() / ui->SizeX->value();
    for(int i=0; i < mazegen->getCellsX(); i++)
      for(int j=0; j < mazegen->getCellsY(); j++)
    {
        Cell* cell = mazegen->getCell(i,j);
        pixels_to_world(cell->xpix, cell->ypix, cell->xworld, cell->zworld, image, pixels_per_unit);
    }

    // delete coins
    Group* group;
    group = (Group*)MREG.FindObj("coins_group");
    if(group != 0)
    {
        group->Clear();
    }
    createCoins(ui->Ncoins->value(), image, mazegen);

    GemObject* sprite = (GemObject*)MREG.DeepFindObj("Linux");
    if(sprite && !MREG.FindObj("linuxoid01"))
    {
        Linuxoid* linuxoid = new Linuxoid("Linux", mazegen, image);
        linuxoid->Declare();
        MREG.AddObj(linuxoid);
    }
    Linuxoid* linuxoid =  (Linuxoid*) MREG.FindObj("linuxoid01");
    if(linuxoid!=0)
        linuxoid->maze = mazegen;
}

void TerrainDlg::on_pushButton_clicked()
{
    if(mterrain==0) return;
    mterrain->SendToGPU();
}

void TerrainDlg::on_genCoins_clicked()
{
    QImage image;
    image.load("maze.png");
    createCoins(ui->Ncoins->value(), image, mazegen);
}

void TerrainDlg::on_clear_clicked()
{

}
