/********************************************************************************
** Form generated from reading UI file 'newpreset.ui'
**
** Created by: Qt User Interface Compiler version 6.4.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWPRESET_H
#define UI_NEWPRESET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NewPreset
{
public:

    void setupUi(QWidget *NewPreset)
    {
        if (NewPreset->objectName().isEmpty())
            NewPreset->setObjectName("NewPreset");
        NewPreset->resize(694, 413);

        retranslateUi(NewPreset);

        QMetaObject::connectSlotsByName(NewPreset);
    } // setupUi

    void retranslateUi(QWidget *NewPreset)
    {
        NewPreset->setWindowTitle(QCoreApplication::translate("NewPreset", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class NewPreset: public Ui_NewPreset {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWPRESET_H
