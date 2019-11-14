/********************************************************************************
** Form generated from reading UI file 'dialogprocesslist.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGPROCESSLIST_H
#define UI_DIALOGPROCESSLIST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QListWidget>

QT_BEGIN_NAMESPACE

class Ui_DialogProcessList
{
public:
    QListWidget *listWidget_Process;

    void setupUi(QDialog *DialogProcessList)
    {
        if (DialogProcessList->objectName().isEmpty())
            DialogProcessList->setObjectName(QStringLiteral("DialogProcessList"));
        DialogProcessList->resize(421, 583);
        listWidget_Process = new QListWidget(DialogProcessList);
        listWidget_Process->setObjectName(QStringLiteral("listWidget_Process"));
        listWidget_Process->setGeometry(QRect(10, 10, 401, 531));

        retranslateUi(DialogProcessList);

        QMetaObject::connectSlotsByName(DialogProcessList);
    } // setupUi

    void retranslateUi(QDialog *DialogProcessList)
    {
        DialogProcessList->setWindowTitle(QApplication::translate("DialogProcessList", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogProcessList: public Ui_DialogProcessList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGPROCESSLIST_H
