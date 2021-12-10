#ifndef TERRAINDLG_H
#define TERRAINDLG_H

#include <QDialog>
#include "geometry.h"
#include "group.h"
#include "mazegen.h"

namespace Ui {
class TerrainDlg;
}

class TerrainDlg : public QDialog
{
    Q_OBJECT

public:
    MazeGen* mazegen;
    explicit TerrainDlg(QWidget *parent = 0);
    Group* createCoins(int Ncoins, QImage& image, MazeGen* maze);
    void     create_n_coins(string nominal, int N, Group* group, Pnt bounds, QImage& image, float pixels_per_unit );
    void     create_n_coins2(string nominal, int N, Group* group, MazeGen* maze);
    bool     fit_coin(Pnt bounds, float X, float Z, QImage& image, float pixels_per_unit);
    ~TerrainDlg();

public slots:
    void OnObjsLoad();

private slots:

    void on_createButton_clicked();

    void on_generateButton_clicked();

    void on_generateMaze_clicked();

    void on_pushButton_clicked();

    void on_genCoins_clicked();

    void on_clear_clicked();

private:
    Ui::TerrainDlg *ui;
};

#endif // TERRAINDLG_H
