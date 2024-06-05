#ifndef DELETEPRESET_H
#define DELETEPRESET_H

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
#include <QSystemTrayIcon>
#include <QMenu>
#include <QProcess>
#include <QTimer>

class DeletePreset : public QDialog
{
public:
    DeletePreset();

private:
    void ParseJson();
    void ParseJsonAuto();
    QList<QString> presetNames;
    QList<QString> presetAutoNames;
};

#endif // DELETEPRESET_H
