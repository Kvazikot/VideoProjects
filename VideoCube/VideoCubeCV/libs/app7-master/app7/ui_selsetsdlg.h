/********************************************************************************
** Form generated from reading UI file 'selsetsdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELSETSDLG_H
#define UI_SELSETSDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeWidget>

QT_BEGIN_NAMESPACE

class Ui_SelSetsDlg
{
public:
    QTreeWidget *setsTree;
    QPushButton *newSet;
    QPushButton *Add;
    QPushButton *Remove;
    QPushButton *newSubSet;
    QLineEdit *objFilter;
    QListWidget *objList;
    QPushButton *deleteSet;
    QPushButton *saveXMLBut;
    QPushButton *loadXMLBut;
    QPushButton *updateBut;

    void setupUi(QDialog *SelSetsDlg)
    {
        if (SelSetsDlg->objectName().isEmpty())
            SelSetsDlg->setObjectName(QString::fromUtf8("SelSetsDlg"));
        SelSetsDlg->resize(737, 380);
        setsTree = new QTreeWidget(SelSetsDlg);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("node"));
        setsTree->setHeaderItem(__qtreewidgetitem);
        setsTree->setObjectName(QString::fromUtf8("setsTree"));
        setsTree->setGeometry(QRect(230, 20, 501, 271));
        setsTree->header()->setVisible(false);
        newSet = new QPushButton(SelSetsDlg);
        newSet->setObjectName(QString::fromUtf8("newSet"));
        newSet->setGeometry(QRect(230, 300, 75, 23));
        Add = new QPushButton(SelSetsDlg);
        Add->setObjectName(QString::fromUtf8("Add"));
        Add->setGeometry(QRect(20, 300, 75, 23));
        Remove = new QPushButton(SelSetsDlg);
        Remove->setObjectName(QString::fromUtf8("Remove"));
        Remove->setGeometry(QRect(100, 300, 75, 23));
        newSubSet = new QPushButton(SelSetsDlg);
        newSubSet->setObjectName(QString::fromUtf8("newSubSet"));
        newSubSet->setGeometry(QRect(310, 300, 75, 23));
        objFilter = new QLineEdit(SelSetsDlg);
        objFilter->setObjectName(QString::fromUtf8("objFilter"));
        objFilter->setGeometry(QRect(20, 20, 201, 20));
        objList = new QListWidget(SelSetsDlg);
        objList->setObjectName(QString::fromUtf8("objList"));
        objList->setGeometry(QRect(20, 50, 201, 241));
        objList->setSelectionMode(QAbstractItemView::ExtendedSelection);
        deleteSet = new QPushButton(SelSetsDlg);
        deleteSet->setObjectName(QString::fromUtf8("deleteSet"));
        deleteSet->setGeometry(QRect(390, 300, 75, 23));
        saveXMLBut = new QPushButton(SelSetsDlg);
        saveXMLBut->setObjectName(QString::fromUtf8("saveXMLBut"));
        saveXMLBut->setGeometry(QRect(230, 330, 75, 23));
        loadXMLBut = new QPushButton(SelSetsDlg);
        loadXMLBut->setObjectName(QString::fromUtf8("loadXMLBut"));
        loadXMLBut->setGeometry(QRect(310, 330, 75, 23));
        updateBut = new QPushButton(SelSetsDlg);
        updateBut->setObjectName(QString::fromUtf8("updateBut"));
        updateBut->setGeometry(QRect(20, 330, 75, 23));

        retranslateUi(SelSetsDlg);

        QMetaObject::connectSlotsByName(SelSetsDlg);
    } // setupUi

    void retranslateUi(QDialog *SelSetsDlg)
    {
        SelSetsDlg->setWindowTitle(QApplication::translate("SelSetsDlg", "Dialog", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = setsTree->headerItem();
        ___qtreewidgetitem->setText(1, QApplication::translate("SelSetsDlg", "data", nullptr));
        newSet->setText(QApplication::translate("SelSetsDlg", "New Set", nullptr));
        Add->setText(QApplication::translate("SelSetsDlg", "Add", nullptr));
        Remove->setText(QApplication::translate("SelSetsDlg", "Remove", nullptr));
        newSubSet->setText(QApplication::translate("SelSetsDlg", "New SubSet", nullptr));
        deleteSet->setText(QApplication::translate("SelSetsDlg", "Delete Set", nullptr));
        saveXMLBut->setText(QApplication::translate("SelSetsDlg", "Save XML", nullptr));
        loadXMLBut->setText(QApplication::translate("SelSetsDlg", "Load XML", nullptr));
        updateBut->setText(QApplication::translate("SelSetsDlg", "Update", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SelSetsDlg: public Ui_SelSetsDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELSETSDLG_H
