/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionAttach_Process;
    QAction *action1;
    QAction *action2;
    QAction *action3;
    QAction *action4;
    QAction *action5;
    QAction *action6;
    QAction *action7;
    QWidget *centralWidget;
    QTabWidget *tabWidget_Main;
    QWidget *tab;
    QTableWidget *tableWidget_Cpu;
    QTableWidget *tableWidget_memView;
    QTableWidget *tableWidget_Stack;
    QWidget *tab_2;
    QTreeView *treeView_Modules;
    QMenuBar *menuBar;
    QMenu *menuOptions;
    QMenu *menuInject;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1371, 796);
        actionAttach_Process = new QAction(MainWindow);
        actionAttach_Process->setObjectName(QStringLiteral("actionAttach_Process"));
        action1 = new QAction(MainWindow);
        action1->setObjectName(QStringLiteral("action1"));
        action2 = new QAction(MainWindow);
        action2->setObjectName(QStringLiteral("action2"));
        action3 = new QAction(MainWindow);
        action3->setObjectName(QStringLiteral("action3"));
        action4 = new QAction(MainWindow);
        action4->setObjectName(QStringLiteral("action4"));
        action5 = new QAction(MainWindow);
        action5->setObjectName(QStringLiteral("action5"));
        action6 = new QAction(MainWindow);
        action6->setObjectName(QStringLiteral("action6"));
        action7 = new QAction(MainWindow);
        action7->setObjectName(QStringLiteral("action7"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        tabWidget_Main = new QTabWidget(centralWidget);
        tabWidget_Main->setObjectName(QStringLiteral("tabWidget_Main"));
        tabWidget_Main->setGeometry(QRect(20, 10, 1281, 711));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        tableWidget_Cpu = new QTableWidget(tab);
        tableWidget_Cpu->setObjectName(QStringLiteral("tableWidget_Cpu"));
        tableWidget_Cpu->setGeometry(QRect(10, 10, 791, 261));
        tableWidget_memView = new QTableWidget(tab);
        tableWidget_memView->setObjectName(QStringLiteral("tableWidget_memView"));
        tableWidget_memView->setGeometry(QRect(10, 290, 791, 361));
        tableWidget_Stack = new QTableWidget(tab);
        tableWidget_Stack->setObjectName(QStringLiteral("tableWidget_Stack"));
        tableWidget_Stack->setGeometry(QRect(840, 10, 421, 661));
        tabWidget_Main->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        treeView_Modules = new QTreeView(tab_2);
        treeView_Modules->setObjectName(QStringLiteral("treeView_Modules"));
        treeView_Modules->setGeometry(QRect(30, 10, 481, 261));
        tabWidget_Main->addTab(tab_2, QString());
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1371, 23));
        menuOptions = new QMenu(menuBar);
        menuOptions->setObjectName(QStringLiteral("menuOptions"));
        menuInject = new QMenu(menuOptions);
        menuInject->setObjectName(QStringLiteral("menuInject"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuOptions->menuAction());
        menuOptions->addAction(actionAttach_Process);
        menuOptions->addAction(menuInject->menuAction());
        menuInject->addSeparator();
        menuInject->addAction(action1);
        menuInject->addAction(action2);
        menuInject->addAction(action3);
        menuInject->addAction(action4);
        menuInject->addAction(action5);
        menuInject->addAction(action6);
        menuInject->addAction(action7);

        retranslateUi(MainWindow);

        tabWidget_Main->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        actionAttach_Process->setText(QApplication::translate("MainWindow", "Attach Process", nullptr));
        action1->setText(QApplication::translate("MainWindow", "1", nullptr));
        action2->setText(QApplication::translate("MainWindow", "2", nullptr));
        action3->setText(QApplication::translate("MainWindow", "3", nullptr));
        action4->setText(QApplication::translate("MainWindow", "4", nullptr));
        action5->setText(QApplication::translate("MainWindow", "5", nullptr));
        action6->setText(QApplication::translate("MainWindow", "6", nullptr));
        action7->setText(QApplication::translate("MainWindow", "7", nullptr));
        tabWidget_Main->setTabText(tabWidget_Main->indexOf(tab), QApplication::translate("MainWindow", "Cpu", nullptr));
        tabWidget_Main->setTabText(tabWidget_Main->indexOf(tab_2), QApplication::translate("MainWindow", "PE View", nullptr));
        menuOptions->setTitle(QApplication::translate("MainWindow", "Options", nullptr));
        menuInject->setTitle(QApplication::translate("MainWindow", "Inject", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
