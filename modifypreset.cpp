#include "modifypreset.h"

ModifyPreset::ModifyPreset()
{
    setWindowTitle("选择预设");
    JsonHandler jsonHandler;
    QString fileData = jsonHandler.GetJsonFileAsQString(jsonFilePath);
    QJsonDocument jsonDoc = jsonHandler.GetJsonDocFromQString(fileData);
    QJsonObject rootObj = jsonDoc.object();
    WriteJsonToQList(rootObj);

    QHBoxLayout* layout = new QHBoxLayout(this);
    QWidget* contentWidget = new QWidget(this);
    setLayout(layout);
    layout->addWidget(contentWidget);

    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    setAttribute(Qt::WA_DeleteOnClose);

    // 根据presets列表创建按钮，并添加到弹出对话框中的布局中
    for (const auto& pair : presets) {
        QString name = pair.first;

        QPushButton* presetButton = new QPushButton(name);
        presetButton->setObjectName("presetButton");
        contentLayout->addWidget(presetButton);

        const QList<OperationItem> &operations = pair.second;

        // 连接按钮的clicked信号到槽函数，以处理按钮点击事件
        QObject::connect(presetButton, &QPushButton::clicked, [&, name]() {
            this->hide();
            QDialog *modifyDialog = new QDialog();
            QHBoxLayout* mainLayout = new QHBoxLayout(this);
            modifyDialog->setLayout(mainLayout);
            // 创建左侧表格
            tableWidget_ = new QTableWidget(this);
            tableWidget_->setColumnCount(3);
            tableWidget_->setHorizontalHeaderLabels(QStringList() << "操作" << "路径/截图" << "时间间隔");
            tableWidget_->horizontalHeader()->setStretchLastSection(true);
            tableWidget_->setSelectionBehavior(QAbstractItemView::SelectRows);
            tableWidget_->setSelectionMode(QAbstractItemView::SingleSelection);

            // 创建右侧按钮布局
            QVBoxLayout* buttonLayout = new QVBoxLayout();
            QPushButton* modifyButton = new QPushButton("修改操作", this);
            QPushButton* addButton = new QPushButton("添加操作", this);
            QPushButton* deleteButton = new QPushButton("删除操作", this);
            QPushButton* finishButton = new QPushButton("完成修改", this);
            buttonLayout->addWidget(modifyButton);
            buttonLayout->addWidget(addButton);
            buttonLayout->addWidget(deleteButton);
            buttonLayout->addWidget(finishButton);

            // 添加表格和按钮布局到主布局
            modifyDialog->setAttribute(Qt::WA_DeleteOnClose);

            for(const auto& operation : operations){
                int rowCount = tableWidget_->rowCount();
                tableWidget_->insertRow(rowCount);
                QString actionText = "点击软件内按钮";
                if(operation.actionType == OperationItem::ActionType::LaunchSoftware){
                    actionText = "打开软件";
                }


                QTableWidgetItem* actionItem = new QTableWidgetItem(actionText);
                QTableWidgetItem* fileItem = new QTableWidgetItem(operation.filePath);
                QString time = QString::number(operation.timeInterval);
                QTableWidgetItem* timeItem = new QTableWidgetItem(time);

                tableWidget_->setItem(rowCount, 0, actionItem);
                tableWidget_->setItem(rowCount, 1, fileItem);
                tableWidget_->setItem(rowCount, 2, timeItem);
            }
            mainLayout->addWidget(tableWidget_);
            mainLayout->addLayout(buttonLayout);
            modifyDialog->show();
            modifyDialog->resize(793, 552);
            modifyDialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
            //设置窗体最大化和最小化
            Qt::WindowFlags windowFlag  = Qt::Dialog;
            windowFlag                  |= Qt::WindowMinimizeButtonHint;
            windowFlag                  |= Qt::WindowMaximizeButtonHint;
            windowFlag                  |= Qt::WindowCloseButtonHint;
            modifyDialog->setWindowFlags(windowFlag);
            modifyDialog->show();


            // 连接槽函数以响应按钮点击事件
            connect(modifyButton, &QPushButton::clicked, this, &ModifyPreset::ModifyOperation);
            connect(addButton, &QPushButton::clicked, this, &ModifyPreset::AddOperation);
            connect(deleteButton, &QPushButton::clicked, this, &ModifyPreset::DeleteOperation);
            connect(finishButton, &QPushButton::clicked, [=](){
                FinishModifying(name);
                modifyDialog->close();
            });
        });
    }
}


void ModifyPreset::WriteJsonToQList(QJsonObject &rootObj){
    // 遍历根对象的键
    for (const QString& key : rootObj.keys()) {
        QList<OperationItem> preset;

        // 获取对应键的数组
        QJsonArray jsonArray = rootObj.value(key).toArray();

        // 遍历数组中的对象
        for (const QJsonValue& value : jsonArray) {
            // 获取对象的值
            QJsonObject jsonObject = value.toObject();
            QString file = jsonObject.value("file").toString();
            int time = jsonObject.value("time").toInt();
            QString type = jsonObject.value("type").toString();
            if(type == "LaunchSoftware"){
                OperationItem operationitem(OperationItem::ActionType::LaunchSoftware, file, time);
                preset.append(operationitem);
            }
            else{
                OperationItem operationitem(OperationItem::ActionType::ClickButton, file, time);
                preset.append(operationitem);
            }
        }
        QPair<QString, QList<OperationItem>> pair;
        pair.first = key;
        pair.second = preset;
        presets.append(pair);
    }
    return;
}

void ModifyPreset::FinishModifying(QString name){
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
    QJsonDocument jsonFinalDoc = jsonHandler.SetJsonDocFromOperationList(jsonDoc, operationList, name);
    bool isSuccess = jsonHandler.WriteJsonDocToFilePath(jsonFinalDoc, jsonFilePath);
    if(isSuccess){
		QMessageBox::information(this, "提示", "修改成功");
	}
	else{
		QMessageBox::information(this, "提示", "修改失败");
	}
}

void ModifyPreset::DeleteOperation(){
    int selectedRow = tableWidget_->currentRow();
    if (selectedRow >= 0) {
        tableWidget_->removeRow(selectedRow);
    }
}

void ModifyPreset::AddOperation() {
    int selectedRow = tableWidget_->currentRow();
    tableWidget_->insertRow(selectedRow + 1);
    ModifyOperation(selectedRow + 1);
}

void ModifyPreset::ModifyOperation(int targetRow){
    if (targetRow >= 0) {
        QDialog *changeOperationDialog = new QDialog(this);
        changeOperationDialog->setWindowTitle("添加操作");

        // 创建目的选择布局
        QVBoxLayout* addActionLayout = new QVBoxLayout();
        QLabel* purposeLabel = new QLabel("选择操作目的：", changeOperationDialog);
        QPushButton* launchSoftwareButton = new QPushButton("打开软件", changeOperationDialog);
        QPushButton* clickButtonButton = new QPushButton("点击软件内按钮", changeOperationDialog);
        QLabel* setTimeLabel = new QLabel("设置你预计电脑启动软件所需要的时间(ms)(若你选择点击软件内按钮，这个时间不需要设置)：", changeOperationDialog);
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
               ChangeOperationRow("打开软件", softwarePath, timeInterval, targetRow);
               changeOperationDialog->close();
            }
        });

        connect(clickButtonButton, &QPushButton::clicked, [=]() {
            QString timeInterval = "0";
            QString buttonImagePath = QFileDialog::getOpenFileName(this, "上传按钮截图", "", "Images (*.png *.jpg *.jpeg)");
            if (!buttonImagePath.isEmpty()) {
                ChangeOperationRow("点击软件内按钮", buttonImagePath, timeInterval, targetRow);
                changeOperationDialog->close();
            }
        });

        // 创建对话框布局
        QVBoxLayout* dialogLayout = new QVBoxLayout(changeOperationDialog);
        dialogLayout->addLayout(addActionLayout);
        changeOperationDialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
        //设置窗体最大化和最小化
        Qt::WindowFlags windowFlag  = Qt::Dialog;
        windowFlag                  |= Qt::WindowMinimizeButtonHint;
        windowFlag                  |= Qt::WindowMaximizeButtonHint;
        windowFlag                  |= Qt::WindowCloseButtonHint;
        changeOperationDialog->setWindowFlags(windowFlag);


        changeOperationDialog->setAttribute(Qt::WA_DeleteOnClose);
        changeOperationDialog->show();
    }
}

void ModifyPreset::ChangeOperationRow(const QString& actionText, const QString& filePath, const QString& timeInterval, int targetRow){
    QTableWidgetItem* actionItem = new QTableWidgetItem(actionText);
    QTableWidgetItem* fileItem = new QTableWidgetItem(filePath);
    QTableWidgetItem* timeItem = new QTableWidgetItem(timeInterval);

    tableWidget_->setItem(targetRow, 0, actionItem);
    tableWidget_->setItem(targetRow, 1, fileItem);
    tableWidget_->setItem(targetRow, 2, timeItem);
}
