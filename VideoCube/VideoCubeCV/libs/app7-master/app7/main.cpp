#include <GL/freeglut.h>
#include <QApplication>

#include "mainwindow.h"
#include "trajectorydialog.h"
#include "../include/str_utils.h"

std::string scene_opt;
bool embed_opt=false;


int main(int argc, char *argv[])
{
    qApp->addLibraryPath("./plugins");
    QApplication app(argc, argv);
    app.setApplicationName("cube");
    app.setApplicationVersion("0.1");
    glutInit(&argc, argv);

    vector<string> parts = StrSplitE(argv[0]," ",false);
    for(int i=0; i < argc; i++)
    {
        if(string(argv[i]) == "--scene")
           scene_opt = string(argv[i+1]);
        if(string(argv[i]) == "--embed")
            embed_opt = true;
    }


    MainWindow mw(0,scene_opt,embed_opt);
    mw.showMaximized();

    //TrajectoryDialog td;
    //td.show();

    return app.exec();
}



