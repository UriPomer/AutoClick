#ifndef MODIFYPRESET_H
#define MODIFYPRESET_H

#include <Tool/operationitem.h>
#include <Tool/defineConst.h>
#include <Tool/jsonhandler.h>

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

class ModifyPreset : public QDialog
{
public:
    ModifyPreset();

private slots:
    void DeleteOperation();
    void ModifyOperation(int targetRow);
    void AddOperation();

private:
    QList<QPair<QString, QList<OperationItem>>> presets;
    QTableWidget* tableWidget_;

    void WriteJsonToQList(QJsonObject &rootObject);
    void FinishModifying(QString name);
    void ChangeOperationRow(const QString& actionText, const QString& filePath, const QString& timeInterval, int targetRow);
};

#endif // MODIFYPRESET_H
