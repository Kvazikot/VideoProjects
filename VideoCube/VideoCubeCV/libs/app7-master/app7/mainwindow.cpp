#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QElapsedTimer>
#include "mainwindow.h"
#include "../include/print.h"
#include "../include/flags.h"
#include "../include/objectinspector.h"
#include "../include/var_parse.h"
#include "../include/camera.h"
#include "../include/cobjregistry.h"
#include "../include/plane.h"
#include "../include/cmaininterface.h"
#include "../include/flags.h"
#include "../include/timer.h"
#include "../include/polet.h"
#include "../include/ploter.h"
#include "../include/material.h"
#include "../include/param_screen_bcb.h"
#include "../include/terraindlg.h"
#include "../include/glwidget2.h"
#include "ui_mainwindow.h"
#include "../include/traj_gen.h"
#include "../include/gemobject.h"
#include "../include/shared_data.h"
#include "../include/cmaininterface.h"
#include "animationdlg.h"
#include "selsetsdlg.h"
#include "../include/osd.h"
#include "../include/window.h"
#include "trajectorydialog.h"

extern void RegisterClasses();
static CObjRegistry& MREG = CObjRegistry::getInstance2();
ObjectInspector& inspector = ObjectInspector::getInstance();
ObjectInspector& inspector2 = ObjectInspector::getInstance2();
COSD& osd = COSD::getInstance();
//static t_flags flags;
static t_flags& flags = GetGFlags();
MainWindow* mw;   // это синглетон
extern TrajGen traj_gen; // это синглетон
static ParamScreen& vt = ParamScreen::getVtInstance();
//VarParser varparser;
extern VarParser varparser;
t_objlistflags objlistflags;


MainWindow::MainWindow(QWidget *parent, std::string scenefile, bool embed_opt) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    map_to_prn(this);
    mw = this;


    MREG.SetSceneFile(scenefile);

    //создание синглетонов
    CMaterial* mater = new CMaterial();
    mater->Declare();
    MREG.AddObj(mater);

    MREG.AddObj(&osd);

    //регистраци€  классов
    RegisterClasses();

    //создание диалогов
    SelSetsDlg* ss_dialog = new SelSetsDlg(ui->dialogBox);
    ui->dialogBox->removeItem(0);
    TerrainDlg* ter_dialog = new TerrainDlg();
    AnimationDlg* anim_dialog = new AnimationDlg(ui->dialogBox);
    //диалог траекторий
    TrajectoryDialog* traj_dialog = new TrajectoryDialog(0);
    QGridLayout *gl2 = new QGridLayout;
    gl2->setObjectName("Trajectorywidget");
    gl2->addWidget(traj_dialog,0,0);
    ui->tab_4->setLayout(gl2);

    ui->dialogBox->addItem(ter_dialog,"Terrain");
    ui->dialogBox->addItem(ss_dialog,"Selection Sets");
    ui->dialogBox->addItem(anim_dialog,"Animation");


    /*
    QGLFormat glFormat;
    glFormat.setVersion(4, 5);
    glFormat.setProfile(QGLFormat::NoProfile);
    glFormat.setSampleBuffers(true);
    glFormat.setDefaultFormat(glFormat);
    glFormat.setSwapInterval(1);
    QGLWidget widget(glFormat);
    widget.makeCurrent();
    const QGLContext *context = widget.context();
    qDebug() << "Really used OpenGl: " << context->format().majorVersion() << "." << context->format().minorVersion();
    widget.close();
*/


        if( embed_opt )
        {
            //контексты
            /*
            QGLContext* glContext=new QGLContext (QGLFormat::defaultFormat());
            glContext->create();// create() always fail without painter device
            QGLFormat f = QGLFormat::defaultFormat();
            f.setDoubleBuffer(true);
            glContext->setFormat(f);

            QGLContext* glContext2=new QGLContext (QGLFormat::defaultFormat());
            QGLFormat f2 = QGLFormat::defaultFormat();
            f2.setDoubleBuffer(true);
            glContext2->setFormat(f2);

            glContext->areSharing(glContext,glContext2);
            */


            //виджет OpenGL
            glw = new GlWidget2( ui->gl_widget );
            if(glw)
            {
                //glw->setGeometry(QRect(0,0,640,480));
                glw->setGeometry( ui->tabWidget->widget(1)->geometry());
                const QGLContext *context = glw->context();
                qDebug() << "Really used OpenGl: " << context->format().majorVersion() << "." << context->format().minorVersion();
                if(context->format().majorVersion() < 4)
                    return;
                QGridLayout *gl = new QGridLayout;
                gl->setObjectName("Agl_widget");
                gl->addWidget(glw,0,0);
                ui->gl_widget->setLayout(gl);
                QObject::connect(glw,SIGNAL(ObjectsIsLoaded()),this,SLOT(OnObjsLoad()));
                QObject::connect(glw,SIGNAL(ObjectsIsLoaded()),ss_dialog,SLOT(OnObjsLoad()));
                QObject::connect(glw,SIGNAL(ObjectsIsLoaded()),anim_dialog,SLOT(OnObjsLoad()));
                QObject::connect(glw,SIGNAL(ObjectsIsLoaded()),ter_dialog,SLOT(OnObjsLoad()));
            }

            //glw->setFocus();
            prn("Test");
        }
        else
        {

            glw = new GlWidget2( 0  );
            if(glw)
            {
                glw->show();
                const QGLContext *context = glw->context();
                qDebug() << "Really used OpenGl: " << context->format().majorVersion() << "." << context->format().minorVersion();
                if(context->format().majorVersion() < 4)
                    return;

                QObject::connect(glw,SIGNAL(ObjectsIsLoaded()),this,SLOT(OnObjsLoad()));
                QObject::connect(glw,SIGNAL(ObjectsIsLoaded()),ss_dialog,SLOT(OnObjsLoad()));
                QObject::connect(glw,SIGNAL(ObjectsIsLoaded()),anim_dialog,SLOT(OnObjsLoad()));
                QObject::connect(glw,SIGNAL(ObjectsIsLoaded()),ter_dialog,SLOT(OnObjsLoad()));
            }


        }




    //установка флагов
    //t_flags& flags = GetGFlags();
    flags.bColorRendering = true;
    flags.bWire = false;
    flags.bShowHiddenAtrs = true;
    flags.iRenderVersion = 1;
    flags.bShading = true;
    flags.bShowGrid = true;

    flags.bdisable_timer1 = true;
    prn("flags1 %x",&flags);
    //Sleep(50);

    //график
    Ploter* p = new Ploter(ui->plotFrame);
    p->plot->setGeometry(ui->plotFrame->rect());


    //загрузка таблиц с законами
    LoadTables();


    // запуск апи тредов
    //ApiThread* api_thread = new ApiThread();
    //api_thread->start();
    //CreateSharedMemory();

    // создание обьекта держащего указатели на ренедеренные кадры
    Data* data = new Data();
    data->Declare();
    MREG.AddObj(data);



    //виждет OpenGL вид сверху
/*
    GLWidget* glw_top = new GLWidget( glContext2, ui->gl_widget2 );
    glw_top->setObjectName("Agl_widget2");
    QGridLayout *gl2 = new QGridLayout;
    gl2->addWidget(glw_top,0,0);
    ui->gl_widget2->setLayout(gl2);
    glw_top->SetMinimapMode();

*/
    //настройка кнопок
/*
    QIcon ricon;
    ricon.addFile("../res/move1.bmp");
    ui->moveButton->setIcon(ricon);
    QIcon ticon;
    ticon.addFile("../res/pick.bmp");
    ui->pickButton->setIcon(ticon);
    QIcon picon;
    picon.addFile("../res/sel.bmp");
    ui->selButton->setIcon(picon);
    QIcon picon2;
    picon.addFile("../res/game.bmp");
    ui->gameButton->setIcon(picon2);
*/

    ui->objectList->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(ui->objectList, SIGNAL( customContextMenuRequested(QPoint) ), this, SLOT(onObjectListContext(QPoint)) );



    //QApplication::connect(QApplication::instance(), SIGNAL(focusChanged(QWidget*, QWidget*)), this, SLOT(setWidgets(QWidget*, QWidget*)));
    connect(ui->bdisable_timer1,SIGNAL(clicked()),this,SLOT(changeFlag()));
    connect(ui->bdisable_timer2,SIGNAL(clicked()),this,SLOT(changeFlag()));
    connect(ui->bGeom,SIGNAL(clicked()),this,SLOT(changeFlag()));
    connect(ui->bPrim,SIGNAL(clicked()),this,SLOT(changeFlag()));
    connect(ui->bGeom,SIGNAL(clicked()),this,SLOT(changeFlag()));
    connect(ui->bGroups,SIGNAL(clicked()),this,SLOT(changeFlag()));
    connect(ui->bHelp,SIGNAL(clicked()),this,SLOT(changeFlag()));
    connect(ui->bHidden,SIGNAL(clicked()),this,SLOT(changeFlag()));
    connect(ui->bVeh,SIGNAL(clicked()),this,SLOT(changeFlag()));
    connect(ui->bUngroop,SIGNAL(clicked()),this,SLOT(changeFlag()));
    connect(ui->bCurves,SIGNAL(clicked()),this,SLOT(changeFlag()));

    connect(ui->bColorRendering,SIGNAL(clicked()),this,SLOT(changeFlag()));
    connect(ui->bWire,SIGNAL(clicked()),this,SLOT(changeFlag()));
    connect(ui->bShading,SIGNAL(clicked()),this,SLOT(changeFlag()));
    //changeFlag();

    startTimer(250);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateObj()));
    timer->start(1);
    flags.bdisable_timer1 = false;
    inspector.SetTableWidget( ui->inspectorTable );

    ui->camBox->addItem("cam01");
    ui->camBox->addItem("cam02");
    ui->camBox->addItem("cam_3person");

    vt.seti("polet_mode",ui->simMode->currentIndex());

    //mater->print();

   // InitObjectBox();

    if(MREG.allObj.size()!=0)
        inspector.GetObjectDetails((char*)MREG.allObj[0]->name.c_str());

}

int  MainWindow::getResX()
{
    return ui->gl_widget2->width();
}
int  MainWindow::getResY()
{
    return ui->gl_widget2->height();
}


int getResX(){   return mw->getResX(); }
int getResY(){   return mw->getResX(); }

void MainWindow::LoadTables()
{

    QFile f("traj.ini");
    if( !f.open( QFile::ReadOnly ) )
    {
        print("Cannot open traj.ini file!");
    }

    QTextStream ts(&f);
    ts.setCodec("Windows-1251");
    QString allStrings = ts.readAll();
    ui->varsEdit->setPlainText(allStrings);

    QFile f2("app6.ini");
    if( !f2.open( QFile::ReadOnly ) )
    {
        print("Cannot open app6.ini file!");
    }
    QTextStream ts2(&f2);
    ts2.setCodec("Windows-1251");
    QString allStrings2 = ts2.readAll();
    QString allStrings3 = allStrings + allStrings2;
    varparser.inputS = allStrings3.toStdString();
    varparser.ParseInput();
    // классы traj_gen берет данные из таблицы vt
    //return;
    traj_gen.Reset();
    traj_gen.Init();
    //traj_gen.PrintInfo();
    traj_gen.GenerateLaws();


}

void MainWindow::updateObj()
{
    // ƒелаем цикл симул€ции
    Timer::gtimer.msec+=Timer::gtimer._timer.elapsed();
    Timer::gtimer.fsec+= Timer::gtimer._timer.elapsed()/1000.;
    //prn("%f %d",Timer::gtimer.fsec, Timer::gtimer._timer.elapsed());
    if(flags.bdisable_timer2) return;
    CObjRegistry* MREG = CObjRegistry::getInstance();
    vector<Object*>::iterator obj;
    for(obj = MREG->allObj.begin(); obj != MREG->allObj.end(); obj++ )
    {
            (*obj)->Slot1();
    }
    Timer::gtimer._timer.start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnObjsLoad()
{
   QString defaultCam = "cam01";//vt.gets("defaultCam").c_str();
   if(defaultCam!="")
   {
       glw->setCamName(defaultCam);
       //glw->SetCamName(defaultCam);
       if(defaultCam.contains("1"))
        ui->camBox->setCurrentIndex(0);
       else
           ui->camBox->setCurrentIndex(1);
       on_camBox_activated(ui->camBox->currentText());
   }

   UpdateObjectList();

   QString autoselect = "car_autopilot"; //;

    for(int i=0; i<(int)ui->objectList->count(); i++ )
        if(ui->objectList->item(i)->text()==autoselect)
        {
            ui->objectList->setCurrentRow(i);
            ui->objectList->setCurrentItem( ui->objectList->item(i) );
            on_objectList_itemClicked(ui->objectList->currentItem());
        }




}

void MainWindow::print(const QString& str)
{
    ui->console->append(str);
    ui->console->moveCursor(QTextCursor::End);
}


void MainWindow::UpdatePscrVars()
{
    ParamScreen& pscr = ParamScreen::getInstance();


    //UPDATEVAR LIST
    if( pscr.paramMap.size()!=ui->varsList->count() ) //always update
    {

        ui->varsList->clear();
        std::map<string, string >::iterator i;
        std::string val;
        int cnt1 = 0;
        for(i = pscr.paramMap.begin(); i!=pscr.paramMap.end(); i++)
        {

            if(pscr.paramSetMap[i->first].timeout<=0)
            {
                //str=" ";
                continue;
            }

            if(i->second == "")
                continue;

            if(i->first != "info")
                val = i->first + "=" + i->second + " ";
            else
                val = i->second + " ";

            if( 1 )
            {
                QListWidgetItem * item = new QListWidgetItem(ui->varsList);
                if( pscr.paramSetMap[i->first].disabled )
                    item->setCheckState(Qt::Unchecked);
                else
                    item->setCheckState(Qt::Checked);
                item->setText(val.c_str());
                ui->varsList->addItem(item);
            }
            cnt1++;
        }

    }

    //update states
    if(1)
    {
        for(int j=0; j<ui->varsList->count(); j++)
        {
            QString text = ui->varsList->item(j)->text();
            QStringList parts = text.split("=");
            QString name = parts[0];
            string val = pscr.paramMap[name.toUtf8().constData()];
            ui->varsList->item(j)->setText(name + "=" + val.c_str() );
        }
    }


}


//dialog timer 50 ms
void MainWindow::timerEvent(QTimerEvent* event)
{
    event=event;
    if( flags.bdisable_timer3 ) return;
    UpdatePscrVars();
    t_flags& flags = GetGFlags();
    if( flags.bNeedUpdObjectList )
    {
        OnObjsLoad();
        flags.bNeedUpdObjectList = false;
    }
    if( flags.bNeedReinitObjInspector )
    {
        inspector.GetObjectDetails((char*)inspector.obj->name.c_str());
        flags.bNeedReinitObjInspector = false;
    }
    if( flags.bNeedUpdFocus )
    {
    }


}

void MainWindow::changeFlag()
{
    t_flags& flags = GetGFlags();



   // flags.bdisable_timer1 = ui->bdisable_timer1->isChecked();
    flags.bdisable_timer2 = ui->bdisable_timer2->isChecked();
    //flags.bdisable_timer3 = ui->bdisable_timer3->checkState();
    flags.bWire = ui->bWire->isChecked();
    flags.bShading = ui->bShading->isChecked();
    flags.bColorRendering = ui->bColorRendering->isChecked();
/*
    objlistflags.bGeom = ui->bGeom->isChecked();
    objlistflags.bVeh = ui->bVeh->isChecked();
    objlistflags.bPrim = ui->bPrim->isChecked();
    objlistflags.bHelp = ui->bHelp->isChecked();
    objlistflags.bGroups = ui->bGroups->isChecked();
    objlistflags.bUngroop = ui->bUngroop->isChecked();
    objlistflags.bHide = ui->bHide->isChecked();
    objlistflags.bHidden = ui->bHidden->isChecked();
    objlistflags.bCurves = ui->bCurves->isChecked();

    UpdateObjectList();
*/
     QCheckBox* but = (QCheckBox*)sender();
     if(but!=0)
     if( (but->objectName() == "bWire") || (but->objectName() == "bShading") || (but->objectName() == "bColorRendering") )
         flags.bChangedRenderMode = true;

    QTimer::singleShot(1000,this,SLOT(returnFlag()));

}

void MainWindow::returnFlag()
{
    flags.bChangedRenderMode = false;
}

void MainWindow::on_simButton_clicked()
{
    static bool pushed=0;

    CPlane* p = (CPlane*)MREG.FindObj("plane01");
    vt.seti("polet_mode",1);
    pushed=!pushed;
    if(pushed)
    {
        if(p!=0)   p->StartSimulation();
        ui->simButton->setText("Stop Sim");
    }
    else
    {
        if(p!=0) p->StopSimulation();
        ui->simButton->setText("Start Sim");
    }

}

void MainWindow::on_resetSim_clicked()
{
    CPlane* p = (CPlane*)MREG.FindObj("plane01");
    int mode = ui->simMode->currentIndex();
    if(p!=0)
    {
        p->mode = mode;
        p->InitSimulation();
    }
    if(mode == 0)
    {
        traj_gen.Reset();
        traj_gen.Init();
        traj_gen.PrintInfo();
        traj_gen.GenerateLaws();
    }

}

void MainWindow::on_setVars_clicked()
{
    string s = (char*)ui->varsEdit->toPlainText().toUtf8().constData();
    varparser.inputS = s;
    varparser.ParseInput();
}

void MainWindow::on_inspectorTable_cellClicked(int row, int column)
{
    inspector.CellClicked(row, column);
}

void MainWindow::on_inspectorTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    currentRow=0;
    currentColumn=0;
    previousRow=0;
    previousColumn=0;
}

void MainWindow::on_inspectorTable_cellChanged(int row, int column)
{
    inspector.CellChanged(row, column);
}

void MainWindow::on_objectBox_currentIndexChanged(const QString &arg1)
{
    inspector.GetObjectDetails( (char*)arg1.toUtf8().constData() );
    CObjRegistry& MREG = CObjRegistry::getInstance2();
    Object* obj = MREG.FindObj( (char*)arg1.toUtf8().constData() );
    if(obj!=0)
        ui->classLabel->setText(obj->classname.c_str());
}
/*
void TriMeshObject::Render()
{
    using namespace Wm5;
    if( mesh == 0) return;
    int n = mesh->GetNumTriangles();
    for(int i=0; i<n; i++)
    {
        APoint tri[3];
        mesh->GetModelTriangle(i, tri);
        glBegin(GL_LINES);
        glVertex3f(tri[0].X(), tri[0].Y(), tri[0].Z() );
        glVertex3f(tri[1].X(), tri[1].Y(), tri[1].Z() );
        glVertex3f(tri[2].X(), tri[2].Y(), tri[2].Z() );
        glEnd();
    }

}
*/

void MainWindow::on_pushButton_2_clicked()
{
/*    using namespace Wm5;
    VertexFormat* vformat = VertexFormat::Create(3,
           VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
           VertexFormat::AU_NORMAL, VertexFormat::AT_FLOAT3, 0,
           VertexFormat::AU_COLOR, VertexFormat::AT_FLOAT3, 0);

    TriMeshObject* obj = new TriMeshObject();
    obj->name = "sphere01";
    //obj->mesh = StandardMesh(vformat).Sphere(64, 64, 10.125f);
    obj->mesh = StandardMesh(vformat).Cylinder (64, 64, 10, 30, 0);
    MREG.AddObj(obj);
*/


}

extern WPPolet	wppolet;

void MainWindow::cameraAnim()
{
 /*   t = (Timer::gtimer.fsec - startTime) / animTime;
    Wm5::Quaternion<double> h;
    h = h1.Slerp(t,h1,h2);
    Wm5::Matrix3<double> rot;
    h.ToRotationMatrix(rot);
    Wm5::Vector3<double> lookAt, pos;
    lookAt = Wm5::Vector3<double>(cam->lookAt.x,cam->lookAt.y,cam->lookAt.z);
    pos = Wm5::Vector3<double>(cam->ei.coord.x,cam->ei.coord.y,cam->ei.coord.z);
    lookAt = Wm5::Vector3<double>(1,0,0);
    lookAt = lookAt * rot;
    Pnt angles = wppolet.getTrajectoryAngles(t);
    lookAt = Wm5::Vector3<double>(angles.x*GR,angles.y*GR,angles.z*GR);
    double theta = acos( lookAt.Y()/lookAt.Length() ) / GR;
    double phi = atan( lookAt.Z()/lookAt.X() ) / GR;
    //cam->orient.B.z = theta + 90;
    //cam->orient.B.x = phi;
*/
    Camera* cam = (Camera*)MREG.FindObj("cam01");
    cam->ei.coord = wppolet.getTrajectoryPoint(Timer::gtimer.fsec - startTime);
    cam->UpdateLookAt();
    // prn("anim %f %f %f", t, theta, phi);

}


void MainWindow::on_pushButton_3_clicked()
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(cameraAnim()));
    startTime = Timer::gtimer.fsec;
    animTime = 100; //sec
    timer->start(10);

}


void MainWindow::on_objectBox_activated(const QString &arg1)
{
    arg1.count();
}

void MainWindow::on_moveButton_clicked()
{
    CMainInterface& mi = CMainInterface::getInstance();
    mi.SwitchMode(CMainInterface::MODE_MOVE);

}

void MainWindow::on_pickButton_clicked()
{
    CMainInterface& mi = CMainInterface::getInstance();
    mi.SwitchMode(CMainInterface::MODE_PICK);
}

void MainWindow::on_selButton_clicked()
{
    CMainInterface& mi = CMainInterface::getInstance();
    mi.SwitchMode(CMainInterface::MODE_SELECTION);
}

void MainWindow::on_gameButton_clicked()
{
    CMainInterface& mi = CMainInterface::getInstance();
    mi.SwitchMode(CMainInterface::MODE_GAME);
}

void MainWindow::on_xB_clicked()
{
    CMainInterface& mi = CMainInterface::getInstance();
    mi.axis = CMainInterface::X_AXIS;
}

void MainWindow::on_yB_clicked()
{
    CMainInterface& mi = CMainInterface::getInstance();
    mi.axis = CMainInterface::Y_AXIS;
}

void MainWindow::on_zB_clicked()
{
    CMainInterface& mi = CMainInterface::getInstance();
    mi.axis = CMainInterface::Z_AXIS;
}


void MainWindow::on_addAttributes_clicked()
{
    inspector.ShowHiddenAttrs(ui->addAttributes->isChecked());
    inspector.GetObjectDetails((char*)inspector.obj->name.c_str());

}

void MainWindow::on_savelButton_clicked()
{
    MREG.SaveScene(false);
}

void MainWindow::UpdateObjectList()
{

    vector<Object*>::iterator obj;
    ui->objectList->clear();
    for(obj = MREG.allObj.begin(); obj != MREG.allObj.end(); obj++ )
        (*obj)->isHidden = 0;

    flags.bChangedRenderMode = true;

    for(obj = MREG.allObj.begin(); obj != MREG.allObj.end(); obj++ )
    {
        bool bCancel = 1;

        //отфильтровываем точки Pnt_132424242
        if(objlistflags.bHelp == false)
        {
           // if((*obj)->ei.groupname == "Helpers" )
           // //if( sscanf((*obj)->name.c_str(),"Pnt%d_%d",&i1,&i1) == 2 )
          //      continue;
        }
        else
        {
            //if( sscanf((*obj)->name.c_str(),"Pnt%d_%d",&i1,&i1) == 2 )
            //    (*obj)->isHidden = 0;

        }

        if((*obj)->ei.groupname == "System Object")
                   bCancel = 0;

               if((objlistflags.bGeom==true) && ((*obj)->ei.groupname == "Geometry"))
                   bCancel = 0;


               if((objlistflags.bPrim==true) && ((*obj)->ei.groupname == "Primitives"))
                   bCancel = 0;

               if((objlistflags.bCurves==true) && ((*obj)->ei.groupname == "Curves"))
                   bCancel = 0;

               if((objlistflags.bHelp==true) && ((*obj)->ei.groupname == "Helpers"))
                   bCancel = 0;

               if((objlistflags.bGroups==true) && ((*obj)->ei.groupname == "Groups"))
                   bCancel = 0;

               if((objlistflags.bVeh==true) && ((*obj)->ei.groupname == "Vehicles"))
                   bCancel = 0;

               if((objlistflags.bUngroop==true) && ((*obj)->ei.groupname != "Geometry")
                                                &&	((*obj)->ei.groupname != "Primitives")
                                                && ((*obj)->ei.groupname != "Helpers")
                                                && ((*obj)->ei.groupname != "Curves")
                                                && ((*obj)->ei.groupname != "Groups")
                                                && ((*obj)->ei.groupname != "Vehicles"))
                   bCancel = 0;

               if((objlistflags.bHidden==false) && ( (*obj)->isHidden==2))
                   bCancel = 1;

               if((objlistflags.bHidden==true) && ( (*obj)->isHidden==2))
                   bCancel = 0;

               QString qname = (*obj)->name.c_str();
               //фильтраци€ по имени
               QString qnameB = qname.toLower();
               if(ui->objFilter->text().contains(" "))
               {
                  QStringList filter_list = ui->objFilter->text().split(" ");
                  foreach (QString item, filter_list) {
                      if( qnameB.contains(item) )
                         bCancel = 0;
                  }
               }else
                   if( qnameB.contains(ui->objFilter->text()) )
                      bCancel = 0;
               if(bCancel == 1) continue;

               ui->objectList->addItem((*obj)->name.c_str());



//        if( ui->bHide )
//            (*obj)->isHidden = 0;
//        else
//            (*obj)->isHidden = 2;


    }
}

void MainWindow::on_objFilter_textChanged(const QString &arg1)
{
    UpdateObjectList();
}

void MainWindow::on_filter_clicked()
{
    if(!ui->filter->isChecked())
        ui->filter->setText("");
}

void MainWindow::on_camBox_activated(const QString &arg1)
{
    static CMainInterface& main_if = CMainInterface::getInstance();

    //стара€ тер€ет контроль
    Camera* old_cam = (Camera*)MREG.FindObj(main_if.activeCamera);
    if(old_cam)
      old_cam->isReceiveInput = false;

    main_if.activeCamera = arg1.toStdString();

    // фокус окна
    /*QWidget* wgl = WglWidget::GetWidget("app6");
    if(wgl)
      wgl->setFocus();
      */
    glw->setCamName(arg1);
    glw->setFocus();

    // нова€ получает контроль
    Camera* new_cam = (Camera*)MREG.FindObj(main_if.activeCamera);
    if(new_cam)
      new_cam->isReceiveInput = true;


}

void MainWindow::on_objectList_itemClicked(QListWidgetItem *item)
{
    inspector.GetObjectDetails((char*)item->text().toStdString().c_str());
}

void MainWindow::on_pushButton_clicked()
{
    varparser.inputS = ui->varsEdit->toPlainText().toStdString();
    varparser.ParseInput();
    // классы traj_gen берет данные из таблицы vt
    traj_gen.Reset();
    traj_gen.Init();
    traj_gen.PrintInfo();
    traj_gen.GenerateLaws();
    UpdateObjectList();
}

void MainWindow::on_simMode_activated(const QString &arg1)
{
    arg1.count();
}

void MainWindow::on_simMode_activated(int index)
{
    vt.seti("polet_mode",index);
}

void MainWindow::closeEvent(QCloseEvent *)
{
}


void MainWindow::on_checkBox_19_clicked()
{

}

void MainWindow::on_objFilter_editingFinished()
{

}

void MainWindow::on_bHide_clicked()
{
    flags.bChangedRenderMode = true;
}


void MainWindow::onObjectListContext(QPoint p)
{
    QMenu menu;

     menu.addAction("Hide",this,SLOT(hideObject()));
     menu.addAction("Unhide",this,SLOT(unhideObject()));
     //menu.addAction(codec->toUnicode("”далить все точки"),this,SLOT(delAllPhotoPoints()));

/*     QAction* action = menu.addAction(codec->toUnicode("”глова€ опора"),this,SLOT(isUglOpore()));
     action->setCheckable(true);
     action->setChecked(graphicsView->curOpora.isUgl);
*/

      menu.exec(ui->objectList->mapToGlobal(p));
}

void MainWindow::hideObject()
{
    Object* o = MREG.FindObj(ui->objectList->currentItem()->text().toStdString());
    if(o!=0)
      o->isHidden = 2;
}

void MainWindow::unhideObject()
{
    Object* o = MREG.FindObj(ui->objectList->currentItem()->text().toStdString());
    if(o!=0)
      o->isHidden = 0;

}

void MainWindow::on_objectList_clicked(const QModelIndex &index)
{
    index.column();
}

void MainWindow::on_objectList_activated(const QModelIndex &index)
{
    index.column();
}

void MainWindow::on_bdisable_timer2_clicked()
{

}


std::string GetOpt(std::string opt)
{
    return varparser.vt.Get(opt);
}

void MainWindow::on_bGroups_clicked()
{

}

void MainWindow::on_inspectorTable_doubleClicked(const QModelIndex &index)
{
    inspector.CellDblClicked(index.row(),index.column());
}

void MainWindow::on_bUngroop_clicked()
{

}

void MainWindow::on_inspectorTable_clicked(const QModelIndex &index)
{

}

void MainWindow::on_reloadShadersBut_clicked()
{
    glw->ReloadShaders();
}

void MainWindow::on_gridCB_clicked()
{
    flags.bShowGrid = ui->gridCB->isChecked();
}
