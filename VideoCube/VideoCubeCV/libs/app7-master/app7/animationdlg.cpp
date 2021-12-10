#include "animationdlg.h"
#include "ui_animationdlg.h"
#include "biped.h"
#include "../include/cobjregistry.h"

//static CObjRegistry& MREG = CObjRegistry::getInstance2();


AnimationDlg::AnimationDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnimationDlg)
{
    ui->setupUi(this);
}

AnimationDlg::~AnimationDlg()
{
    delete ui;
}

void AnimationDlg::OnObjsLoad()
{

}

void AnimationDlg::on_horizontalSlider_sliderMoved(int position)
{
    CObjRegistry& MREG = CObjRegistry::getInstance2();
    Biped* bip = (Biped*)MREG.FindObj("biped01");
    bip->cur_frame = position;
    ui->horizontalSlider->setMaximum(bip->N_frames);
    ui->frameNumber->display(position);
}
