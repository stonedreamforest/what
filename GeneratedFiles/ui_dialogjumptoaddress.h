/********************************************************************************
** Form generated from reading UI file 'dialogjumptoaddress.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGJUMPTOADDRESS_H
#define UI_DIALOGJUMPTOADDRESS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_DialogJumpToAddress
{
public:
    QDialogButtonBox *buttonBox;
    QTextEdit *textEdit_Address;

    void setupUi(QDialog *DialogJumpToAddress)
    {
        if (DialogJumpToAddress->objectName().isEmpty())
            DialogJumpToAddress->setObjectName(QStringLiteral("DialogJumpToAddress"));
        DialogJumpToAddress->resize(432, 96);
        buttonBox = new QDialogButtonBox(DialogJumpToAddress);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(80, 60, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        textEdit_Address = new QTextEdit(DialogJumpToAddress);
        textEdit_Address->setObjectName(QStringLiteral("textEdit_Address"));
        textEdit_Address->setGeometry(QRect(10, 20, 411, 31));

        retranslateUi(DialogJumpToAddress);
        QObject::connect(buttonBox, SIGNAL(accepted()), DialogJumpToAddress, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DialogJumpToAddress, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogJumpToAddress);
    } // setupUi

    void retranslateUi(QDialog *DialogJumpToAddress)
    {
        DialogJumpToAddress->setWindowTitle(QApplication::translate("DialogJumpToAddress", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogJumpToAddress: public Ui_DialogJumpToAddress {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGJUMPTOADDRESS_H
