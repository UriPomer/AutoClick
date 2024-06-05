#ifndef NEWPRESET_H
#define NEWPRESET_H

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
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QTextStream>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

namespace Ui {
class NewPreset;
}

class NewPreset : public QDialog
{
    Q_OBJECT

public:
    NewPreset(QWidget* parent = nullptr)
        : QDialog(parent)
    {
        setWindowTitle("选择预设");
        this->setAttribute(Qt::WA_DeleteOnClose);
        // 创建左右布局
        QHBoxLayout* mainLayout = new QHBoxLayout(this);

        // 创建左侧表格
        tableWidget_ = new QTableWidget(this);
        tableWidget_->setColumnCount(3);
        tableWidget_->setHorizontalHeaderLabels(QStringList() << "操作" << "路径/截图" << "时间间隔");
        tableWidget_->horizontalHeader()->setStretchLastSection(true);
        tableWidget_->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget_->setSelectionMode(QAbstractItemView::SingleSelection);

        // 创建右侧按钮布局
        QVBoxLayout* buttonLayout = new QVBoxLayout();
        QPushButton* addButton = new QPushButton("添加操作", this);
        QPushButton* deleteButton = new QPushButton("删除操作", this);
        QPushButton* createButton = new QPushButton("完成创建", this);
        QPushButton* cancelButton = new QPushButton("取消创建", this);
        buttonLayout->addWidget(addButton);
        buttonLayout->addWidget(deleteButton);
        buttonLayout->addWidget(createButton);
        buttonLayout->addWidget(cancelButton);

        // 连接槽函数以响应按钮点击事件
        connect(addButton, &QPushButton::clicked, this, &NewPreset::addOperation);
        connect(deleteButton, &QPushButton::clicked, this, &NewPreset::deleteOperation);
        connect(createButton, &QPushButton::clicked, this, &NewPreset::createPreset);

        connect(cancelButton, &QPushButton::clicked,[this](){
            this->close();
        });

        // 添加表格和按钮布局到主布局
        mainLayout->addWidget(tableWidget_);
        mainLayout->addLayout(buttonLayout);
        setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
        
        // 去掉问号，只保留关闭
    }


private slots:
    void addOperation()
    {
        // 创建添加操作的对话框
        QDialog *addOperationDialog = new QDialog(this);
        addOperationDialog->setWindowTitle("添加操作");

        // 创建目的选择布局
        QVBoxLayout* addActionLayout = new QVBoxLayout();
        QLabel* purposeLabel = new QLabel("选择操作目的：", addOperationDialog);
        QPushButton* launchSoftwareButton = new QPushButton("打开软件", addOperationDialog);
        QPushButton* clickButtonButton = new QPushButton("点击软件内按钮", addOperationDialog);
        QLabel* setTimeLabel = new QLabel("设置你预计电脑启动软件所需要的时间(ms)(若你选择点击软件内按钮，这个时间不需要设置)：", addOperationDialog);
        QLineEdit* setTimeEdit = new QLineEdit;
        QIntValidator* timeIntValidator = new QIntValidator;
        timeIntValidator->setRange(0, 10000);
        setTimeEdit->setValidator(timeIntValidator);
        addActionLayout->addWidget(purposeLabel);
        addActionLayout->addWidget(launchSoftwareButton);
        addActionLayout->addWidget(clickButtonButton);
        addActionLayout->addWidget(setTimeLabel);
        addActionLayout->addWidget(setTimeEdit);

        launchSoftwareButton->setEnabled(false);

        connect(setTimeEdit ,&QLineEdit::textChanged, this, [=](){
            launchSoftwareButton->setEnabled(true);
        });

        // 连接槽函数以响应按钮点击事件
        connect(launchSoftwareButton, &QPushButton::clicked, [=]() {
            QString timeInterval = setTimeEdit->text();
            QString softwarePath = QFileDialog::getOpenFileName(this, "选择要启动的软件", "", "应用程序 (*.exe)");
            if (!softwarePath.isEmpty()) {
                addOperationRow("打开软件", softwarePath, timeInterval);
                addOperationDialog->close();
            }
        });

        connect(clickButtonButton, &QPushButton::clicked, [=]() {
            QString timeInterval = "0";
            QString buttonImagePath = QFileDialog::getOpenFileName(this, "上传按钮截图", "", "Images (*.png *.jpg *.jpeg)");
            if (!buttonImagePath.isEmpty()) {
                addOperationRow("点击软件内按钮", buttonImagePath, timeInterval);
                addOperationDialog->close();
            }
        });

        // 创建对话框布局
        QVBoxLayout* dialogLayout = new QVBoxLayout(addOperationDialog);
        dialogLayout->addLayout(addActionLayout);


        // 去掉问号，只保留关闭
        addOperationDialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
        //设置窗体最大化和最小化
        Qt::WindowFlags windowFlag  = Qt::Dialog;
        windowFlag                  |= Qt::WindowMinimizeButtonHint;
        windowFlag                  |= Qt::WindowMaximizeButtonHint;
        windowFlag                  |= Qt::WindowCloseButtonHint;
        addOperationDialog->setWindowFlags(windowFlag);
        addOperationDialog->setAttribute(Qt::WA_DeleteOnClose);
        addOperationDialog->show();
    }

    void deleteOperation()
    {
        int selectedRow = tableWidget_->currentRow();
        if (selectedRow >= 0) {
            tableWidget_->removeRow(selectedRow);
        }
    }

    void createPreset()
    {
        // 执行完成创建的操作

        if(tableWidget_->rowCount() == 0){
            QMessageBox::information(nullptr, "警告", "列表不能为空");
            return;
        }
        //设置预设名
        QDialog *nameDialog = new QDialog(this);
        nameDialog->setWindowTitle("设置预设名");
        nameDialog->setAttribute(Qt::WA_DeleteOnClose);

        // 去掉问号，只保留关闭
        nameDialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
        //设置窗体最大化和最小化
        Qt::WindowFlags windowFlag  = Qt::Dialog;
        windowFlag                  |= Qt::WindowMinimizeButtonHint;
        windowFlag                  |= Qt::WindowMaximizeButtonHint;
        windowFlag                  |= Qt::WindowCloseButtonHint;
        nameDialog->setWindowFlags(windowFlag);


        QVBoxLayout* nameLayout = new QVBoxLayout();
        QLabel* nameLabel = new QLabel("在下方输入预设名称：", nameDialog);
        QLineEdit *nameLineEdit = new QLineEdit(nameDialog);
        QPushButton *namePushButton = new QPushButton("完成", nameDialog);

        //QRegularExpressionValidator* validator = new QRegularExpressionValidator(this);
        //QRegularExpression regExp("[A-Za-z_\\-\\u4E00-\\u9FFF0-9]+");  // 包含英文、下划线、短横线、数字和中文的正则表达式
        //// 应用验证器到LineEdit
        //validator->setRegularExpression(regExp);
        //nameLineEdit->setValidator(validator);


        nameLayout->addWidget(nameLabel);
        nameLayout->addWidget(nameLineEdit);
        nameLayout->addWidget(namePushButton);

        // 创建对话框布局
        QVBoxLayout* dialogLayout = new QVBoxLayout(nameDialog);
        dialogLayout->addLayout(nameLayout);
        nameDialog->setAttribute(Qt::WA_DeleteOnClose);

        connect(namePushButton, &QPushButton::clicked, this, [=](){
            nameDialog->close();
            nameSettingByUser = nameLineEdit->text();
            QList<OperationItem> operationList;
            int rowCount = tableWidget_->rowCount();
            for (int i = 0; i < rowCount; ++i) {
                QString actionText = tableWidget_->item(i, 0)->text();
                QString filePath = tableWidget_->item(i, 1)->text();
                int timeInterval = tableWidget_->item(i, 2)->text().toInt();

                OperationItem::ActionType actionType = OperationItem::ActionType::LaunchSoftware;
                if (actionText == "点击软件内按钮") {
                    actionType = OperationItem::ActionType::ClickButton;
                }

                OperationItem operation(actionType, filePath, timeInterval);
                operationList.append(operation);
            }


            JsonHandler jsonHandler;
            QString fileData = jsonHandler.GetJsonFileAsQString(jsonFilePath);
            QJsonDocument jsonDoc = jsonHandler.GetJsonDocFromQString(fileData);
            QJsonDocument jsonFinalDoc = jsonHandler.SetJsonDocFromOperationList(jsonDoc, operationList, nameSettingByUser);
            bool isSuccess = jsonHandler.WriteJsonDocToFilePath(jsonFinalDoc, jsonFilePath);
            if (isSuccess) {
                QMessageBox::information(nullptr, "提示", "创建成功");
            }
			else {
				QMessageBox::information(nullptr, "提示", "创建失败");
			}

            this->close();
        });

        nameDialog->resize(390,200);
        nameDialog->show();
    }

private:
    Ui::NewPreset *ui;
    void addOperationRow(const QString& actionText, const QString& filePath, const QString& timeInterval)
    {
        int rowCount = tableWidget_->rowCount();
        tableWidget_->insertRow(rowCount);

        QTableWidgetItem* actionItem = new QTableWidgetItem(actionText);
        QTableWidgetItem* fileItem = new QTableWidgetItem(filePath);
        QTableWidgetItem* timeItem = new QTableWidgetItem(timeInterval);

        tableWidget_->setItem(rowCount, 0, actionItem);
        tableWidget_->setItem(rowCount, 1, fileItem);
        tableWidget_->setItem(rowCount, 2, timeItem);
    }

    QTableWidget* tableWidget_;
    QString nameSettingByUser;
};


#endif // NEWPRESET_H
