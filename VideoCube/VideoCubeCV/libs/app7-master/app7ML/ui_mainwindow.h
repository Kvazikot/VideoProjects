/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QWidget>
#include "mingamewidget.h"
#include "nnwidget.h"
#include "qwt_plot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionSave_values;
    QAction *actionGenerate_values;
    QAction *actionShowIDs;
    QAction *actionSave_NNet_weights;
    QAction *actionLoad_NNet_weights;
    QAction *actionClear_NNet;
    QAction *actionGenerate_NNet;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QPlainTextEdit *console;
    QwtPlot *qwtPlot;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_2;
    MinGameWidget *gamewidget;
    QWidget *tab_2;
    QGridLayout *gridLayout_3;
    NNWidget *nnwidget;
    QWidget *tab_3;
    QGridLayout *gridLayout_4;
    QTableWidget *wtableWidget;
    QToolBox *toolBox;
    QWidget *page;
    QGridLayout *gridLayout_5;
    QPushButton *showQ;
    QPushButton *showOptmalButton;
    QPushButton *learnedButton;
    QPushButton *lernButton;
    QWidget *page_2;
    QGridLayout *gridLayout_6;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_7;
    QHBoxLayout *horizontalLayout;
    QPushButton *test_nn1;
    QPushButton *pushButton_2;
    QSpinBox *maxEpochSB;
    QPushButton *pushButton;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *resetW;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QSpacerItem *verticalSpacer;
    QMenuBar *menuBar;
    QMenu *menuSave_values;
    QMenu *menuView;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1045, 836);
        actionSave_values = new QAction(MainWindow);
        actionSave_values->setObjectName(QStringLiteral("actionSave_values"));
        actionGenerate_values = new QAction(MainWindow);
        actionGenerate_values->setObjectName(QStringLiteral("actionGenerate_values"));
        actionShowIDs = new QAction(MainWindow);
        actionShowIDs->setObjectName(QStringLiteral("actionShowIDs"));
        actionSave_NNet_weights = new QAction(MainWindow);
        actionSave_NNet_weights->setObjectName(QStringLiteral("actionSave_NNet_weights"));
        actionLoad_NNet_weights = new QAction(MainWindow);
        actionLoad_NNet_weights->setObjectName(QStringLiteral("actionLoad_NNet_weights"));
        actionClear_NNet = new QAction(MainWindow);
        actionClear_NNet->setObjectName(QStringLiteral("actionClear_NNet"));
        actionGenerate_NNet = new QAction(MainWindow);
        actionGenerate_NNet->setObjectName(QStringLiteral("actionGenerate_NNet"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        console = new QPlainTextEdit(centralWidget);
        console->setObjectName(QStringLiteral("console"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(console->sizePolicy().hasHeightForWidth());
        console->setSizePolicy(sizePolicy);

        gridLayout->addWidget(console, 0, 2, 1, 1);

        qwtPlot = new QwtPlot(centralWidget);
        qwtPlot->setObjectName(QStringLiteral("qwtPlot"));

        gridLayout->addWidget(qwtPlot, 2, 1, 1, 2);

        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        gridLayout_2 = new QGridLayout(tab);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gamewidget = new MinGameWidget(tab);
        gamewidget->setObjectName(QStringLiteral("gamewidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(gamewidget->sizePolicy().hasHeightForWidth());
        gamewidget->setSizePolicy(sizePolicy1);
        gamewidget->setMinimumSize(QSize(350, 350));

        gridLayout_2->addWidget(gamewidget, 0, 0, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        gridLayout_3 = new QGridLayout(tab_2);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        nnwidget = new NNWidget(tab_2);
        nnwidget->setObjectName(QStringLiteral("nnwidget"));

        gridLayout_3->addWidget(nnwidget, 0, 0, 1, 1);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        gridLayout_4 = new QGridLayout(tab_3);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        wtableWidget = new QTableWidget(tab_3);
        if (wtableWidget->columnCount() < 3)
            wtableWidget->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        wtableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        wtableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        wtableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        wtableWidget->setObjectName(QStringLiteral("wtableWidget"));

        gridLayout_4->addWidget(wtableWidget, 0, 0, 1, 1);

        tabWidget->addTab(tab_3, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);

        toolBox = new QToolBox(centralWidget);
        toolBox->setObjectName(QStringLiteral("toolBox"));
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        page->setGeometry(QRect(0, 0, 508, 268));
        gridLayout_5 = new QGridLayout(page);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        showQ = new QPushButton(page);
        showQ->setObjectName(QStringLiteral("showQ"));

        gridLayout_5->addWidget(showQ, 0, 0, 1, 1);

        showOptmalButton = new QPushButton(page);
        showOptmalButton->setObjectName(QStringLiteral("showOptmalButton"));

        gridLayout_5->addWidget(showOptmalButton, 1, 0, 1, 1);

        learnedButton = new QPushButton(page);
        learnedButton->setObjectName(QStringLiteral("learnedButton"));

        gridLayout_5->addWidget(learnedButton, 2, 0, 1, 1);

        lernButton = new QPushButton(page);
        lernButton->setObjectName(QStringLiteral("lernButton"));

        gridLayout_5->addWidget(lernButton, 3, 0, 1, 1);

        toolBox->addItem(page, QStringLiteral("Q learning"));
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        page_2->setGeometry(QRect(0, 0, 508, 268));
        gridLayout_6 = new QGridLayout(page_2);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        groupBox = new QGroupBox(page_2);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        gridLayout_7 = new QGridLayout(groupBox);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        test_nn1 = new QPushButton(groupBox);
        test_nn1->setObjectName(QStringLiteral("test_nn1"));

        horizontalLayout->addWidget(test_nn1);

        pushButton_2 = new QPushButton(groupBox);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout->addWidget(pushButton_2);

        maxEpochSB = new QSpinBox(groupBox);
        maxEpochSB->setObjectName(QStringLiteral("maxEpochSB"));
        maxEpochSB->setMaximum(100000000);
        maxEpochSB->setValue(100000);

        horizontalLayout->addWidget(maxEpochSB);


        gridLayout_7->addLayout(horizontalLayout, 0, 0, 1, 1);

        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout_7->addWidget(pushButton, 1, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        resetW = new QPushButton(groupBox);
        resetW->setObjectName(QStringLiteral("resetW"));

        horizontalLayout_2->addWidget(resetW);

        pushButton_3 = new QPushButton(groupBox);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        horizontalLayout_2->addWidget(pushButton_3);

        pushButton_4 = new QPushButton(groupBox);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));

        horizontalLayout_2->addWidget(pushButton_4);


        gridLayout_7->addLayout(horizontalLayout_2, 2, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_7->addItem(verticalSpacer, 3, 0, 1, 1);


        gridLayout_6->addWidget(groupBox, 0, 0, 1, 1);

        toolBox->addItem(page_2, QStringLiteral("Page 2"));

        gridLayout->addWidget(toolBox, 1, 0, 2, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1045, 29));
        menuSave_values = new QMenu(menuBar);
        menuSave_values->setObjectName(QStringLiteral("menuSave_values"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QStringLiteral("menuView"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuSave_values->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuSave_values->addAction(actionSave_values);
        menuSave_values->addAction(actionGenerate_values);
        menuSave_values->addAction(actionSave_NNet_weights);
        menuSave_values->addAction(actionLoad_NNet_weights);
        menuSave_values->addAction(actionClear_NNet);
        menuSave_values->addAction(actionGenerate_NNet);
        menuView->addSeparator();
        menuView->addAction(actionShowIDs);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);
        toolBox->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionSave_values->setText(QApplication::translate("MainWindow", "Save values", 0));
        actionGenerate_values->setText(QApplication::translate("MainWindow", "Generate values", 0));
        actionShowIDs->setText(QApplication::translate("MainWindow", "ShowIDs", 0));
        actionSave_NNet_weights->setText(QApplication::translate("MainWindow", "Save NNet", 0));
        actionLoad_NNet_weights->setText(QApplication::translate("MainWindow", "Load NNet", 0));
        actionClear_NNet->setText(QApplication::translate("MainWindow", "Clear NNet", 0));
        actionGenerate_NNet->setText(QApplication::translate("MainWindow", "Generate NNet", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Environment", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Neural Network", 0));
        QTableWidgetItem *___qtablewidgetitem = wtableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MainWindow", "in", 0));
        QTableWidgetItem *___qtablewidgetitem1 = wtableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("MainWindow", "out", 0));
        QTableWidgetItem *___qtablewidgetitem2 = wtableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("MainWindow", "value", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MainWindow", "Weights", 0));
        showQ->setText(QApplication::translate("MainWindow", "Show Q", 0));
        showOptmalButton->setText(QApplication::translate("MainWindow", "Show Optimal Trajectory", 0));
        learnedButton->setText(QApplication::translate("MainWindow", "Show Agent Trajectory", 0));
        lernButton->setText(QApplication::translate("MainWindow", "QLearning", 0));
        toolBox->setItemText(toolBox->indexOf(page), QApplication::translate("MainWindow", "Q learning", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", " Neural Network controls", 0));
        test_nn1->setText(QApplication::translate("MainWindow", "test_nn1 (learn)", 0));
        pushButton_2->setText(QApplication::translate("MainWindow", "Stop Learn", 0));
        maxEpochSB->setPrefix(QApplication::translate("MainWindow", "max_epoch =", 0));
        pushButton->setText(QApplication::translate("MainWindow", "test_nn2 (evalute sin(x))", 0));
        resetW->setText(QApplication::translate("MainWindow", "Reset Weights", 0));
        pushButton_3->setText(QApplication::translate("MainWindow", "Draw Errors", 0));
        pushButton_4->setText(QApplication::translate("MainWindow", "Draw W Stat", 0));
        toolBox->setItemText(toolBox->indexOf(page_2), QApplication::translate("MainWindow", "Page 2", 0));
        menuSave_values->setTitle(QApplication::translate("MainWindow", "File", 0));
        menuView->setTitle(QApplication::translate("MainWindow", "View", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
