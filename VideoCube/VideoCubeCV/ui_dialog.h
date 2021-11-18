/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QPushButton *loadFromFileButton;
    QLabel *statusBar;
    QPlainTextEdit *plainTextEdit;
    QPushButton *loadFromSharedMemoryButton;
    QCheckBox *checkBox;
    QPushButton *readSrcButton;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(664, 594);
        gridLayout = new QGridLayout(Dialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMaximumSize(QSize(640, 480));
        label->setScaledContents(true);
        label->setAlignment(Qt::AlignCenter);
        label->setWordWrap(true);

        gridLayout->addWidget(label, 1, 1, 1, 1);

        loadFromFileButton = new QPushButton(Dialog);
        loadFromFileButton->setObjectName(QString::fromUtf8("loadFromFileButton"));

        gridLayout->addWidget(loadFromFileButton, 0, 1, 1, 1);

        statusBar = new QLabel(Dialog);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));

        gridLayout->addWidget(statusBar, 4, 1, 1, 1);

        plainTextEdit = new QPlainTextEdit(Dialog);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setMaximumSize(QSize(16777215, 200));

        gridLayout->addWidget(plainTextEdit, 2, 1, 1, 1);

        loadFromSharedMemoryButton = new QPushButton(Dialog);
        loadFromSharedMemoryButton->setObjectName(QString::fromUtf8("loadFromSharedMemoryButton"));

        gridLayout->addWidget(loadFromSharedMemoryButton, 6, 1, 1, 1);

        checkBox = new QCheckBox(Dialog);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));

        gridLayout->addWidget(checkBox, 3, 1, 1, 1);

        readSrcButton = new QPushButton(Dialog);
        readSrcButton->setObjectName(QString::fromUtf8("readSrcButton"));

        gridLayout->addWidget(readSrcButton, 5, 1, 1, 1);


        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", nullptr));
        label->setText(QApplication::translate("Dialog", "Launch two of these dialogs.  In the first, press the top button and load an image from a file.  In the second, press the bottom button and display the loaded image from shared memory.", nullptr));
        loadFromFileButton->setText(QApplication::translate("Dialog", "Load Image From File...", nullptr));
        statusBar->setText(QApplication::translate("Dialog", "Status bar", nullptr));
        loadFromSharedMemoryButton->setText(QApplication::translate("Dialog", "Display Image From Shared Memory", nullptr));
        checkBox->setText(QApplication::translate("Dialog", "CheckBox", nullptr));
        readSrcButton->setText(QApplication::translate("Dialog", "Read sources test", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
