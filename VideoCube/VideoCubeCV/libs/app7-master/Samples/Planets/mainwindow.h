#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDir>
#include <QMainWindow>
#include <QStringList>

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QMenu;
class QScrollArea;
QT_END_NAMESPACE
class PaintArea;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private slots:
    void loadApp6();

private:
    void createActions();
    void createMenus();
    void timerEvent(QTimerEvent* e);

    QMenu *fileMenu;
    QAction *openAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *brushWidthAct;
    QAction *brushColorAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *aboutPluginsAct;
};

#endif
