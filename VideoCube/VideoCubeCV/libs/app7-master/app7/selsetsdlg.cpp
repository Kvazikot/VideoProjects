#include <QDomDocument>
#include <QDomElement>
#include <QListWidgetItem>
#include <QInputDialog>
#include <QFile>
#include "selsetsdlg.h"
#include "ui_selsetsdlg.h"
#include "../include/str_utils.h"
#include "../include/cobjregistry.h"
#include "../include/flags.h"
#include "../include/gemobject.h"
#include "../include/objectinspector.h"
#include "../include/selection.h"
#include "../include/group.h"
#include "../include/print.h"

static CObjRegistry& MREG = CObjRegistry::getInstance2();
static ObjectInspector& Inspector =  ObjectInspector::getInstance();
static CSelection& Selection = CSelection::getInstance();


SelSetsDlg::SelSetsDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelSetsDlg)
{
    ui->setupUi(this);
}

SelSetsDlg::~SelSetsDlg()
{
    delete ui;
}

void SelSetsDlg::UpdateObjList()
{
    vector<Object*>::iterator obj;
    vector<Object*>::iterator obj2;
    ui->objList->clear();
    for(obj = MREG.allObj.begin(); obj != MREG.allObj.end(); obj++ )
    {
        if((*obj)->classname == "Group")
        {
            Group* g = (Group*)(*obj);
            for(obj2 = g->objs.begin(); obj2 != g->objs.end(); obj2++ )
            {
                ui->objList->addItem((*obj2)->name.c_str());
            }
        }
        //ui->objList->addItem((*obj)->name.c_str());

    }
}

void SelSetsDlg::OnObjsLoad()
{
    //LoadXml("xml_shemes/scene.xml");
    UpdateObjList();
}

void SelSetsDlg::on_newSet_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Name of Set"),
                                         tr("Set name:"), QLineEdit::Normal,
                                           "Set", &ok);
    if (ok && !text.isEmpty())
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->setsTree);
        item->setText(0, text);
        item->setText(1, text);
        item->setData(0,Qt::UserRole,QVariant(QStringList("")));
    }

}

void SelSetsDlg::on_newSubSet_clicked()
{

    bool ok;
    QString text = QInputDialog::getText(this, tr("Name of SubItem"),
                                         tr("SubItem name:"), QLineEdit::Normal,
                                         "SubItem", &ok);
    if (ok && !text.isEmpty())
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->setsTree->currentItem());
        item->setText(0, text);
        item->setText(1, text);
        item->setData(0,Qt::UserRole,QVariant(QStringList("")));
    }
}

void SelSetsDlg::on_Add_clicked()
{
    QTreeWidgetItem *item = ui->setsTree->currentItem();
    QVariant v = item->data(0,Qt::UserRole);
    QStringList names_list = v.toStringList();
    foreach (QListWidgetItem* item, ui->objList->selectedItems()) {
        if( !names_list.contains(item->text()) )
            names_list.append(item->text());
    }

    item->setData(0,Qt::UserRole,QVariant(names_list));
    item->setText(0, item->text(1) + " (" + QString::number(names_list.size()) + ")");
}

void SelSetsDlg::on_Remove_clicked()
{

}

void SelSetsDlg::on_deleteSet_clicked()
{
    delete ui->setsTree->currentItem();

}

void SelSetsDlg::on_objList_itemClicked(QListWidgetItem *item)
{

    Object* obj = MREG.DeepFindObj((char*)item->text().toUtf8().constData() );
    if(obj!=0)
    {
        if(obj->atrs.size()==0)
            obj->Declare();
        Inspector.GetObjectDetails(obj);
    }
    Selection.ResetSelection();
    Selection.SelectByName( (char*)item->text().toUtf8().constData() );

}

void SelSetsDlg::on_saveXMLBut_clicked()
{
    //выходной файл
    QFile file_out("xml_shemes/scene1.xml");
    if (!file_out.open(QFile::WriteOnly | QFile::Text)) {

        prn("cannot open write scene.xml file ");
        return ;
    }
    QDomDocument domDocument;
    QDomElement  xml;
    QDomElement  root = domDocument.createElement("root");

    for(int i=0; i < (int)ui->objList->count(); i++)
    {
        QString s = ui->objList->item(i)->text() + ",";
        file_out.write(s.toUtf8().constData(),s.size());
    }
    file_out.close();
    return;

    //traverse code
//    QTreeWidgetItemIterator it(ui->setsTree);
//    QTreeWidgetItem* rootW = (*it)->parent();
//    while (*it)
//    {
//        prn((*it)->text(0).toUtf8().constData());
//        if( (*it)->parent() == rootW )
//            prn("root");
//        //(*it)->indexOfChild()
//        ++it;
//    }

}

QTreeWidgetItem* find_child(QTreeWidgetItem* parent, QString name)
{
    for(int i=0; i < (int)parent->childCount(); i++)
        if( parent->child(i)->text(0) == name)
            return parent->child(i);
    return 0;
}

int SelSetsDlg::LoadXml(QString fileName)
{
    QDomDocument domDocument;
    QVector<QDomElement> set_list;

    //проврка синтаксиса xml
    QString errorStr;
    int errorLine;
    int errorColumn;

    QFile file_in(fileName);
    if (!file_in.open(QFile::ReadOnly | QFile::Text)) {
        print("cannot open " + fileName);
        return 0;
    }
    QTextStream in_stream(&file_in);
    QString text = in_stream.readAll();
    if ( !domDocument.setContent(text, true, &errorStr, &errorLine,
                                &errorColumn)) {
        QMessageBox::information(window(), tr("DOM Bookmarks"),
                                 tr("Parse error at line %1, column %2:\n%3")
                                 .arg(errorLine)
                                 .arg(errorColumn)
                                 .arg(errorStr) );
        return 0;
    }

    QDomElement xml = domDocument.documentElement();
    QDomElement root = xml;
    QDomElement child = root;

    //find all selection sets
    while( 1==1 )
    {
        print(child.nodeName());
        if( child.nodeName() == "" ) break;
        if( child.nodeName() == "selectionSets" )
        {
            break;
        }
        child = child.nextSiblingElement();
    }

   QDomElement selSetsNode = child;
   child = child.firstChildElement();
   while( 1==1 )
   {
       if( child.nodeName() == "" ) break;
       if( child.nodeName() == "set" )
           set_list.push_back(child);
       child = child.nextSiblingElement();
   }

   //walk thru sets
   foreach (QDomElement set, set_list )
   {
       child = set.firstChildElement();
       QString set_name = set.attribute("name");

       QTreeWidgetItem *item = new QTreeWidgetItem(ui->setsTree);
       item->setText(0, set_name);
       name2TreeNode[set_name] = item;

       while( 1==1 )
       {
           if( child.nodeName() == "" ) break;
           if( child.nodeName() == "parent_obj" )
           {
               QString name = child.attribute("name");
               QString obj_string = name + "{" + child.text() + "}";
               //print(obj_string);

               QTreeWidgetItem *sub_item = new QTreeWidgetItem(item);
               sub_item->setText(0, "parent_obj");
               sub_item->setText(1, name);
               sub_item->setData(0, Qt::UserRole, QVariant(obj_string));
               sub_item->setFlags(Qt::ItemIsEditable);
           }

           if( child.nodeName() == "children" )
           {
               QString createReference = child.attribute("createReference");
               QString srcSet = child.attribute("srcSet");
               QTreeWidgetItem *sub_item = new QTreeWidgetItem(item);
               sub_item->setText(0, "children");
               sub_item->setText(1, child.text());
               sub_item->setFlags(Qt::ItemIsEditable);
               sub_item->setData(0,Qt::UserRole,QVariant(child.text()));
               if(createReference == "true")
               {
                   sub_item->setText(1, "ref from " + srcSet);
                   sub_item->setData(0,Qt::UserRole,QString("srcSet="+srcSet));
               }

           }

           child = child.nextSiblingElement();
       }

   }

   // исполнение команд и скриптов дерева
   QMap<QString,QTreeWidgetItem*>::iterator i;
   for(i=name2TreeNode.begin();i!=name2TreeNode.end(); i++)
   {
       string set_name =i.key().toStdString();
       Group* TTT = (Group*)MREG.FindObj("TTT");
       if(TTT == 0){ print("Cannot find group TTT!"); return 0; }
       t_flags& flags = GetGFlags();
       flags.bNeedUpdObjectList = true;
       for(int j=0; j < (int)i.value()->childCount(); j++)
       {
           QTreeWidgetItem* child = i.value()->child(j);
           if( child->text(0) == "children" )
           {
                QString children_list = child->data(0, Qt::UserRole).toString();
                if( children_list.contains("srcSet=") )
                {
                    QStringList parts = children_list.split("=");
                    QTreeWidgetItem* parent = name2TreeNode[parts[1]];
                    QTreeWidgetItem* src_child = find_child(parent, "children");

                    QStringList  children_list2 = src_child->data(0, Qt::UserRole).toString().split(",");
                    foreach (QString s, children_list2)
                    {
                        GemObject* o = (GemObject*)MREG.DeepFindObj(s.toUtf8().constData());
                        if (o==0) continue;
                        GemObject* o_copy = new GemObject(*o);
                        o_copy->name = StrRep(o->name, parts[1].toStdString(), set_name);
                        o_copy->isCanRender = true;
                        //group->AddObj(o_copy);
                        TTT->AddObj(o_copy);
                        prn(o_copy->name.c_str());
                    }
                }
           }
       }
   }
   for(i=name2TreeNode.begin();i!=name2TreeNode.end(); i++)
   {
       for(int j=0; j < (int)i.value()->childCount(); j++)
       {
           QTreeWidgetItem* child = i.value()->child(j);
           if( child->text(0) == "parent_obj" )
           {
                QString obj_string = child->data(0, Qt::UserRole).toString();
                MREG.CreateObj(obj_string.toUtf8().constData(),true);
           }
       }
   }

    //OnObjsLoad();
   return 0;
}

void SelSetsDlg::on_loadXMLBut_clicked()
{
    LoadXml("xml_shemes/scene.xml");
}

void SelSetsDlg::on_updateBut_clicked()
{
    UpdateObjList();
}
