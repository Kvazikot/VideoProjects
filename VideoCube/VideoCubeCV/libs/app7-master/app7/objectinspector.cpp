#include <QStyledItemDelegate>
#include <QTextCodec>
#include <QStyleOptionButton>
#include <QHeaderView>
#include <QComboBox>
#include <QPushButton>
#include <QApplication>
#include "str_utils.h"
#include "print.h"
#include "flags.h"
#include "selection.h"
#include "cobjregistry.h"
#include "objectinspector.h"


ObjectInspector ObjectInspector::instance;
ObjectInspector ObjectInspector::instance2;

ObjectInspector& ObjectInspector::getInstance()
{
    return instance;
}

ObjectInspector& ObjectInspector::getInstance2()
{
    return instance2;
}


ObjectInspector::ObjectInspector(QObject *parent)
{
    obj = 0;
    bShowHidden = 0;
    parent=0;

}

void ObjectInspector::SetTableWidget(QTableWidget* w)
{
    tablew = w;
    WidgetDelegate *mDeleg = new WidgetDelegate;
    mDeleg->tablew = tablew;
    tablew->setItemDelegateForColumn(2,mDeleg);
    //QTableWidgetItem* hi = tablew->horizontalHeaderItem(0);
    tablew->horizontalHeader()->resizeSection(1,200);
    tablew->horizontalHeader()->resizeSection(2,200);
}

void ObjectInspector::UpdateAtr(t_atr a)
{

   // if( ( !bShowHidden ) && ( (a.isHidden() ) || (a.isStd())) )
   //    return;

    if(filter!="")
    {
        if( a.name.c_str()!= filter) return;
    }


    //tablew->connect(this,SIGNAL(cellClicked(int, int));
    // name
    QTableWidgetItem* nameItem  = new QTableWidgetItem(a.name.c_str());
    tablew->setItem(a.idx, 0, nameItem);

    // value
    QTableWidgetItem* valItem  = new QTableWidgetItem(a.toString().c_str());
    tablew->setItem(a.idx, 1, valItem);

    //role
    QTableWidgetItem* typeItem  = new QTableWidgetItem(a.type.c_str());
    tablew->setItem(a.idx, 2, typeItem);

    if(a.type=="selector")
    {
        QComboBox* combo = new QComboBox();
        bDisableChange = true;
        combo->setObjectName(a.name.c_str());
        selIdx = a.idx;
        tablew->setCellWidget(a.idx, 1, combo);

        QObject::connect(combo, SIGNAL(currentIndexChanged(int)),this, SLOT(Selector(int)));
        string s = *(string*)(a.addr);
        vector<string> slist = StrSplitE(s," ",false);        
        for(int i=0; i<(int)slist.size(); i++)
        {
            QString str;
            str.fromStdString(slist[i]);
            QTextCodec *codec = QTextCodec::codecForName("cp1251");
            QString string = codec->toUnicode(slist[i].c_str());
            //QTextCodec* codec = QTextCodec::codecForName("UTF-8");
            combo->addItem(string);
        }
        //QObject::connect(combo, SIGNAL(currentIndexChanged(int)),this, SLOT(SelChanged(int)));
        combo->setCurrentIndex(a.data);
    }
    if(a.type=="state")
    {
        QComboBox* combo = new QComboBox();
        bDisableChange = true;
        combo->setObjectName(a.name.c_str());
        selIdx = a.idx;
        tablew->setCellWidget(a.idx, 1, combo);

        QObject::connect(combo, SIGNAL(currentIndexChanged(int)),this, SLOT(Selector(int)));
        vector<string> slist = StrSplitE(a.data2," ",false);
        for(int i=0; i<(int)slist.size(); i++)
        {
            QString str;
            str.fromStdString(slist[i]);
            QTextCodec *codec = QTextCodec::codecForName("cp1251");
            QString string = codec->toUnicode(slist[i].c_str());
            //QTextCodec* codec = QTextCodec::codecForName("UTF-8");
            combo->addItem(string);
        }
        //QObject::connect(combo, SIGNAL(currentIndexChanged(int)),this, SLOT(SelChanged(int)));
        combo->setCurrentIndex(*(int*)(a.addr));
    }
    if(a.type=="func")
    {
        QPushButton* but = new QPushButton();
        but->setText(a.name.c_str());
        tablew->setCellWidget(a.idx, 2, but);
        connect(but, SIGNAL(clicked()),this, SLOT(FunRun()));

    }
    if(a.type=="bool")
    {
        QPushButton* but = new QPushButton();
        bool b = *(bool*)a.addr;
        //but->setCheckable(true);
        if(b)
            but->setText("True");
        else
            but->setText("False");

        tablew->setCellWidget(a.idx, 1, but);
        but->setObjectName(a.name.c_str());
        connect(but, SIGNAL(clicked()),this, SLOT(FlagClick()));

    }
//    if(a.type!="bool" && a.type!="func" && a.type!="selector")
//    {
//        QPushButton* but = new QPushButton();
//        but->setText(a.type.c_str());
//        tablew->setCellWidget(a.idx, 2, but);
//        //curRow = a.idx;
//        QObject::connect(but, SIGNAL(clicked()),this, SLOT(FunRun()));

//    }
}

void ObjectInspector::SelChanged(int i)
{
   i=0;
}


void ObjectInspector::FlagClick()
{
    QPushButton* but = (QPushButton*)sender();   
    t_atr* a = obj->GET_ATTR(but->objectName().toStdString());
    (*(bool*)a->addr) = !(*(bool*)a->addr);
    bool b = *(bool*)a->addr;
    if( b )
        but->setText("True");
    else
        but->setText("False");

    //obj->

}


void ObjectInspector::FunRun()
{
    QPushButton* but = (QPushButton*)sender();
    for(int i=0; i<(int)tablew->rowCount()-1; i++ )
    {
        QTableWidgetItem* nameItem = tablew->item(i,0);
        if(nameItem->text().size()!=0)
        if(nameItem->text() == but->text())
        {
            t_atr* a = obj->GET_ATTR(but->text().toStdString());
            if(a!=0)
            {
                obj->OnAttrChange(*a);
                break;
            }
        }


    }


    //int row = item->row();
    //prn("%d",row);
    return;
/*    QTableWidgetItem* itemName = tablew->item(row, 0);
    QTableWidgetItem* itemVal = tablew->item(row, 1);
    t_atr* a = obj->GET_ATTR(itemName->text().toStdString().c_str());
    a->fromString(itemVal->text().toStdString().c_str());
    obj->OnAttrChange(*a);
*/


}


void ObjectInspector::Selector(int idx)
{
    if( bDisableChange ) return;
    QComboBox* combo = qobject_cast<QComboBox*>(sender());
    if( combo != NULL )
    {
        t_atr* a = obj->GET_ATTR(combo->objectName().toStdString());
        if(a)
        {
            if(a->type == "selector")
                a->data = idx;
            else if (a->type == "state")
                *(int*)(a->addr) = idx;
            obj->OnAttrChange(*a);
        }
    }
}

void ObjectInspector::AddFunc(QString name, int idx)
{
    // кнопка
    QTableWidgetItem* valItem  = new QTableWidgetItem("");
    valItem->setData(Qt::EditRole, 1);
//    tablew->set
    tablew->setItem(idx, 2, valItem);

}

void ObjectInspector::ShowHiddenAttrs(bool flag)
{

        bShowHidden = flag;
        int ri=0;
        int ni=0;
        for( uint i=0; i < obj->atrs.size(); i++ )
        {
            t_atr& a = obj->atrs[i];
            if( !bShowHidden )
                a.idx = ri;
            else
                a.idx = i;

            if( !( a.isHidden() || a.isStd() ) )
                ri++;
            else
                ni++;


        }
        //prn("ri=%d ni=%d",ri,ni);


}

int  ObjectInspector::GetObjectDetails(char* objname)
{
    tablew->clear();
    CObjRegistry& MREG = CObjRegistry::getInstance2();
    obj = MREG.DeepFindObj( objname );
    if(objname==0) return 0;
    if(obj==0) return 0;
    GetObjectDetails(obj);
    startTimer(80);
    return 1;
}

int  ObjectInspector::GetObjectDetails(Object* o)
{
    tablew->clear();
    obj = o;

    ShowHiddenAttrs(bShowHidden);

    bDisableChange = true;

    tablew->setRowCount(obj->atrs.size());

    //аттрибуты
    int idx = 0;
    for(int i=0; i<(int)obj->atrs.size(); i++)
    {
        UpdateAtr(obj->atrs[i]);
        idx++;

       if(obj->atrs[i].type == "func")
        {
            AddFunc(obj->atrs[i].name.c_str(), idx-1);
        }


        //mu_printf("%s flags %x", obj->atrs[i].name.c_str(), obj->atrs[i].flags);

    }

    //idx+=i;

    //методы
    meths = obj->metatable[obj->classname];
    for(vector<meta_entry>::iterator j=meths.begin(); j!=meths.end(); j++)
    {
        AddFunc((*j).methname, idx);
        idx++;
    }

    bDisableChange = false;
    //obj->OnSelect();
    CSelection s = CSelection::getInstance();
    s.ResetSelection();
    s.AddObj(obj);
    return 1;

}

void ObjectInspector::CellChanged(int row, int column)
{
    if( bDisableChange ) return;
    QTableWidgetItem* itemName = tablew->item(row, 0);
    QTableWidgetItem* itemVal = tablew->item(row, 1);
    if( column == 1 )
    {

        t_atr* a = obj->GET_ATTR(itemName->text().toStdString().c_str());
        a->fromString(itemVal->text().toStdString().c_str());
        obj->OnAttrChange(*a);
        prn("val=%s",a->toString().c_str());
    }
    bDisableUpdate = false;
}

void ObjectInspector::CellClicked(int row, int column)
{
    if( bDisableChange ) return;
    QTableWidgetItem* item = tablew->item(row, 0);
    if(item==0) return;
    if( column == 2 )
    {
        t_atr* a = obj->GET_ATTR(item->text().toUtf8().constData());
        //if(a->type == "func" )
        obj->OnAttrChange(*a);
    }
    else
    {
        t_atr* a = obj->GET_ATTR(item->text().toUtf8().constData());
        //if(a->type == "func" )
        obj->OnAttrSelect(*a);
    }
}

void ObjectInspector::CellDblClicked(int row, int column)
{
    row=column;
    bDisableUpdate = true;
}

void WidgetDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                            const QModelIndex &index) const
 {

/*    QTableWidgetItem* item = tablew->item(index.row(), 2);

    QString type = item->text();
    if( type == "func" )
    {
        QStyleOptionButton opt;
        opt.palette = QPalette(Qt::red);
        opt.state = QStyle::State_Active | QStyle::State_Enabled;
        opt.text = "Run";
        opt.rect = option.rect;
        QApplication::style()->drawControl(QStyle::CE_PushButton, &opt, painter);

     } else
         QStyledItemDelegate::paint(painter, option, index);
*/

     QStyledItemDelegate::paint(painter, option, index);
}

void ObjectInspector::timerEvent(QTimerEvent* e)
{
    e=0;
    if(bDisableUpdate) return;
    bDisableChange = true;
    for(vector<t_atr>::iterator a = obj->atrs.begin(); a!=obj->atrs.end(); a++)
    {
        // value
        QTableWidgetItem* valItem  = new QTableWidgetItem(a->toString().c_str());
        tablew->setItem(a->idx, 1, valItem);
        if(a->type=="state")
        {
            QComboBox* combo = (QComboBox*)tablew->cellWidget(a->idx, 1);
            combo->setCurrentIndex(*(int*)(a->addr));
        }

    }
    bDisableChange = false;
}
