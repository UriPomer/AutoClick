/********************************************************************************
** Form generated from reading UI file 'loadpreset.ui'
**
** Created by: Qt User Interface Compiler version 6.4.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOADPRESET_H
#define UI_LOADPRESET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoadPreset
{
public:

    void setupUi(QWidget *LoadPreset)
    {
        if (LoadPreset->objectName().isEmpty())
            LoadPreset->setObjectName("LoadPreset");
        LoadPreset->resize(400, 300);

        retranslateUi(LoadPreset);

        QMetaObject::connectSlotsByName(LoadPreset);
    } // setupUi

    void retranslateUi(QWidget *LoadPreset)
    {
        LoadPreset->setWindowTitle(QCoreApplication::translate("LoadPreset", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoadPreset: public Ui_LoadPreset {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOADPRESET_H
