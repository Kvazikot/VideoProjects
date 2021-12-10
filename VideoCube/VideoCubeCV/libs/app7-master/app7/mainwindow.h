#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "../include/glwidget2.h"
#include "../include/renderobject.h"
#include "../include/objectinspector.h"

namespace Ui {
class MainWindow;
}


/*class TriMeshObject : public RenderObject
{
public:
    Wm5::TriMesh* mesh;
    TriMeshObject(){ mesh = 0; isCanRender = true; isEntety = true; }
    void Render();
};
*/

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    GlWidget2* glw;
    double startTime, animTime, t;
    //Wm5::Quaternion<double>  h1;
    //Wm5::Quaternion<double>  h2;

    int  getResX();
    int  getResY();
    void UpdateObjectList();
    void UpdatePscrVars();

    void update();
    explicit MainWindow(QWidget *parent = 0, std::string scenefile="", bool embed_opt=false);
    void timerEvent(QTimerEvent* event);
    ~MainWindow();
    void closeEvent(QCloseEvent *);

private:
    Ui::MainWindow *ui;

public slots:
    void print(const QString& str);
    void OnObjsLoad();
    void changeFlag();
    void returnFlag();

signals:
    void sigObjsLoaded();

private slots:
    void hideObject();
    void unhideObject();
    void onObjectListContext(QPoint);
    void LoadTables();
    void updateObj();
    void on_simButton_clicked();
    void on_resetSim_clicked();
    void on_setVars_clicked();
    void on_inspectorTable_cellClicked(int row, int column);
    void on_inspectorTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_inspectorTable_cellChanged(int row, int column);
    void on_objectBox_currentIndexChanged(const QString &arg1);
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void cameraAnim();
    void on_objectBox_activated(const QString &arg1);
    void on_moveButton_clicked();
    void on_pickButton_clicked();
    void on_selButton_clicked();
    void on_gameButton_clicked();
    void on_xB_clicked();
    void on_yB_clicked();
    void on_zB_clicked();
    void on_addAttributes_clicked();
    void on_savelButton_clicked();
    void on_objFilter_textChanged(const QString &arg1);
    void on_filter_clicked();
    void on_camBox_activated(const QString &arg1);
    void on_objectList_itemClicked(QListWidgetItem *item);
    void on_pushButton_clicked();
    void on_simMode_activated(const QString &arg1);
    void on_simMode_activated(int index);
    void on_checkBox_19_clicked();
    void on_objFilter_editingFinished();
    void on_bHide_clicked();
    void on_objectList_clicked(const QModelIndex &index);
    void on_objectList_activated(const QModelIndex &index);
    void on_bdisable_timer2_clicked();
    void on_bGroups_clicked();
    void on_inspectorTable_doubleClicked(const QModelIndex &index);
    void on_bUngroop_clicked();
    void on_inspectorTable_clicked(const QModelIndex &index);
    void on_reloadShadersBut_clicked();
    void on_gridCB_clicked();
};

#endif // MAINWINDOW_H
