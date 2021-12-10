#ifndef TERRAINDLG_H
#define TERRAINDLG_H

#include <QDialog>

namespace Ui {
class TerrainDlg;
}

class TerrainDlg : public QDialog
{
    Q_OBJECT

public:
    explicit TerrainDlg(QWidget *parent = 0);
    ~TerrainDlg();

private slots:
    void on_createButton_clicked();

    void on_generateButton_clicked();

    void on_generateMaze_clicked();

    void on_pushButton_clicked();

private:
    Ui::TerrainDlg *ui;
};

#endif // TERRAINDLG_H
