#ifndef AUTONEWPRESET_H
#define AUTONEWPRESET_H

#include <Tool/operationitem.h>

#include <QWidget>
#include <QApplication>
#include <QDialog>
#include <QPushButton>
#include <QFileDialog>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QRegularExpressionValidator>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QTextStream>
#include <QDebug>
#include <QProcess>

class AutoNewPreset : public QDialog
{
public:
    AutoNewPreset();
};

#endif // AUTONEWPRESET_H
