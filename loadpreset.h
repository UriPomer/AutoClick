#ifndef LOADPRESET_H
#define LOADPRESET_H

#include <operationitem.h>
#include <Tool/defineConst.h>

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

namespace Ui {
class LoadPreset;
}

class LoadPreset : public QDialog
{
    Q_OBJECT

public:
    explicit LoadPreset(QWidget *parent = nullptr);

public slots:
    void ParseJson();
    void ParseJsonAuto();

private:
    Ui::LoadPreset *ui;
    QList<QPair<QString, QList<OperationItem>>> presets;
    QList<QPair<QString, QList<OperationItemAuto>>> presetsAuto;
    const QString clickButtonExePath = FindButtonAndClickExePath;
};

#endif // LOADPRESET_H
