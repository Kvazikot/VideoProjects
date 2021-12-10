#ifndef SELSETSDLG_H
#define SELSETSDLG_H

#include <QListWidgetItem>
#include <QDialog>
#include <QTreeWidgetItem>
#include <QMap>
namespace Ui {
class SelSetsDlg;
}

class SelSetsDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SelSetsDlg(QWidget *parent = 0);
    int LoadXml(QString fileName);
    void UpdateObjList();
    QMap<QString,QTreeWidgetItem*> name2TreeNode;
    ~SelSetsDlg();

private:
    Ui::SelSetsDlg *ui;
public slots:
    void OnObjsLoad();
private slots:
    void on_newSet_clicked();
    void on_newSubSet_clicked();
    void on_Add_clicked();
    void on_Remove_clicked();
    void on_deleteSet_clicked();
    void on_objList_itemClicked(QListWidgetItem *item);
    void on_saveXMLBut_clicked();
    void on_loadXMLBut_clicked();
    void on_updateBut_clicked();
};

#endif // SELSETSDLG_H
