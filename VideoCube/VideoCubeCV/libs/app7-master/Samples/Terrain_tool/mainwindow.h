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
    void open();
    bool saveAs();
    void brushColor();
    void brushWidth();
    void changeBrush();
    void insertShape();
    void applyFilter();
    void about();
    void aboutPlugins();
    void loadApp6();

private:
    void createActions();
    void createMenus();
    void timerEvent(QTimerEvent* e);

    PaintArea *paintArea;
    QScrollArea *scrollArea;

    QMenu *fileMenu;
    QMenu *brushMenu;
    QMenu *shapesMenu;
    QMenu *filterMenu;
    QMenu *helpMenu;
    QActionGroup *brushActionGroup;
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
