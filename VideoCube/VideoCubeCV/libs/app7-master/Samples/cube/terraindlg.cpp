#include "terraindlg.h"
#include "ui_terraindlg.h"
#include "mterrain.h"
#include "cobjregistry.h"
#include "mazegen.h"

MTerrain* mterrain;
static CObjRegistry& MREG = CObjRegistry::getInstance2();

TerrainDlg::TerrainDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TerrainDlg)
{
    ui->setupUi(this);
    mterrain = new MTerrain();
    mterrain->scale = Pnt(1,1,1);
    MREG.AddObj(mterrain);
}

TerrainDlg::~TerrainDlg()
{
    delete ui;
}

void TerrainDlg::on_createButton_clicked()
{
    mterrain->Create(ui->widthSegs->value(),
                     ui->HeightSegs->value(),
                     ui->LengthSegs->value(),
                     ui->SizeX->value(),
                     ui->SizeY->value(),
                     ui->SizeZ->value());
}

void TerrainDlg::on_generateButton_clicked()
{
    mterrain->Generate(ui->detail->value(), ui->Roughness->value(),ui->hconst->value());
}

void TerrainDlg::on_generateMaze_clicked()
{
    QImage image(ui->cellsX->value()*30, ui->cellsY->value()*30, QImage::Format_RGB32);
    image.fill(Qt::black);
    MazeGen mazegen(image, ui->cellsX->value(), ui->cellsY->value(), ui->wall_width->value());
    image.save("maze.png");
    mterrain->Create(ui->widthSegs->value(),
                     ui->HeightSegs->value(),
                     ui->LengthSegs->value(),
                     ui->SizeX->value(),
                     ui->SizeY->value(),
                     ui->SizeZ->value());
    mterrain->SetBumpImage(image);
    mterrain->SendToGPU();
}

void TerrainDlg::on_pushButton_clicked()
{
    mterrain->SendToGPU();
}
