/*
+ - - - + - + - -
+ - + - + copyright by Vladimir Baranov (Kvazikot)  <br>
+ - + - + email: vsbaranov83@gmail.com  <br>
+ - + - + github: http://github.com/Kvazikot/VideoProjects <br>
                          )            (
                         /(   (\___/)  )\
                        ( #)  \ ('')| ( #
                         ||___c\  > '__||
                         ||**** ),_/ **'|
                   .__   |'* ___| |___*'|
                    \_\  |' (    ~   ,)'|
                     ((  |' /(.  '  .)\ |
                      \\_|_/ <_ _____> \______________
                       /   '-, \   / ,-'      ______  \
              b'ger   /      (//   \\)     __/     /   \
                                          './_____/

*/

#include <QApplication>
#include <QTextStream>
#include "dialog.h"
#include "mainwindow.h"
#include "cobjregistry.h"
#include "kirpich.h"
#include "glwidget2.h"

const QString logFilePath = "VideoCube.log";
bool logToFile = true;
bool logToStderr = true;

void customMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QHash<QtMsgType, QString> msgLevelHash({{QtDebugMsg, "Debug"}, {QtInfoMsg, "Info"}, {QtWarningMsg, "Warning"}, {QtCriticalMsg, "Critical"}, {QtFatalMsg, "Fatal"}});
    QByteArray localMsg = msg.toLocal8Bit();
    QTime time = QTime::currentTime();
    QString formattedTime = time.toString("hh:mm:ss.zzz");
    QByteArray formattedTimeMsg = formattedTime.toLocal8Bit();
    QString logLevelName = msgLevelHash[type];
    QByteArray logLevelMsg = logLevelName.toLocal8Bit();

    if (logToFile) {
        QString txt = QString("%1 %2: %3 ").arg(formattedTime, logLevelName, msg);
        QFile outFile(logFilePath);
        outFile.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream ts(&outFile);
        ts << endl;
        ts << txt << endl;
        outFile.close();
    }
    if( logToStderr)
    {
        fprintf(stderr, "\n%s %s: %s \n", formattedTimeMsg.constData(), logLevelMsg.constData(), localMsg.constData());
        fflush(stderr);
    }



    if (type == QtFatalMsg)
        abort();
}

int main(int argc, char *argv[])
{
    //qApp->addLibraryPath("./platforms");
    QByteArray envVar = qgetenv("QTDIR");       //  check if the app is ran in Qt Creator
    if (envVar.isEmpty())
       logToFile = true;

    qInstallMessageHandler(customMessageOutput); // custom message handler for debugging

    QApplication application(argc, argv);
    //Dialog dialog;
    //dialog.show();
    MainWindow wnd;
    wnd.show();

/*
    GlWidget2* glw = new GlWidget2( 0 );
    if(glw)
    {
        glw->setGeometry(QRect(0,0,640,480));
        const QGLContext *context = glw->context();
        qDebug() << "Really used OpenGl: " << context->format().majorVersion() << "." << context->format().minorVersion();
        if(context->format().majorVersion() < 4)
            return 1;
        //QGridLayout *gl = new QGridLayout;
        //gl->setObjectName("Agl_widget");
        //gl->addWidget(glw,0,0);
        //ui->gl_widget->setLayout(gl);
        //QObject::connect(glw,SIGNAL(ObjectsIsLoaded()),this,SLOT(OnObjsLoad()));
        //QObject::connect(glw,SIGNAL(ObjectsIsLoaded()),ss_dialog,SLOT(OnObjsLoad()));
        //QObject::connect(glw,SIGNAL(ObjectsIsLoaded()),anim_dialog,SLOT(OnObjsLoad()));
        //QObject::connect(glw,SIGNAL(ObjectsIsLoaded()),ter_dialog,SLOT(OnObjsLoad()));
    }
*/
    Kirpich* kirpich = new Kirpich("cube",20,20,20);

    CObjRegistry* reg = new CObjRegistry();
    reg->AddObj(kirpich);
    prn("Test");

    return application.exec();
}

