#ifndef OBJECTINSPECTOR_H
#define OBJECTINSPECTOR_H

#include <QTimerEvent>
#include <QTableWidget>
#include <QStyledItemDelegate>
#include <vector>
#include "object.h"

class WidgetDelegate : public QStyledItemDelegate
{
public:
    QTableWidget* tablew;

private:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

};

class ObjectInspector : public QObject
{
    Q_OBJECT
public:
    static ObjectInspector instance;
    static ObjectInspector instance2;
    static ObjectInspector& getInstance();
    static ObjectInspector& getInstance2();
    static ObjectInspector* getInstance3();
    int     selIdx;
    int     curRow;
    bool    bShowHidden;
    QTableWidget* tablew;
    Object*       obj;
    QString       filter;
    QString       objfilter;
    bool          bDisableChange;
    bool          bDisableUpdate;
    std::vector<meta_entry> meths;
    explicit ObjectInspector(QObject *parent = 0);
    bool isObjDead();
    void Clear();
    void Update();
    void ShowHiddenAttrs(bool flag);
    void SetFilter(QString _filter){filter = _filter;}
    int  GetObjectDetails(char* objname);
    int  GetObjectDetails(Object* o);
    void AddFunc(QString name, int idx);
    void ApplyAtr(int atr_idx, bool but);
    void UpdateAtr(t_atr a);
    void RunFunc(int idx);
    void SetTableWidget(QTableWidget* w);
    void CellClicked(int row, int column);
    void CellDblClicked(int row, int column);
    void CellChanged(int row, int column);
    void timerEvent(QTimerEvent* e);

signals:

public slots:
    void SelChanged(int);
    void FunRun();
    void Selector(int);
    void FlagClick();
};

#endif // OBJECTINSPECTOR_H
