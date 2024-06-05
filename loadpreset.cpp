#include "loadpreset.h"

LoadPreset::LoadPreset(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle("选择预设");
    ParseJson();

    QHBoxLayout* layout = new QHBoxLayout(this);
    QWidget* contentWidget = new QWidget(this);
    setLayout(layout);
    layout->addWidget(contentWidget);

    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);

    // 根据presets列表创建按钮，并添加到弹出对话框中的布局中
    for (const auto& pair : presets) {
        QString name = pair.first;

        QPushButton* presetButton = new QPushButton(name);
        presetButton->setObjectName("presetButton");
        contentLayout->addWidget(presetButton);

        const QList<OperationItem> &operations = pair.second;

        // 连接按钮的clicked信号到槽函数，以处理按钮点击事件
        QObject::connect(presetButton, &QPushButton::clicked, [&]() {
            this->hide();
            OperationItem lastOperationItem = operations.first();

            for(const auto& operation : operations){
                if(operation.actionType == OperationItem::ActionType::LaunchSoftware){
                    lastOperationItem = operation;
                }

                if(operation.actionType == OperationItem::ActionType::LaunchSoftware){
                    QProcess *process = new QProcess();
                    QRegularExpression regex("^(.+)/(.+)$");
                    QRegularExpressionMatch match = regex.match(operation.filePath);

                    if (match.hasMatch()) {
                        QString firstPart = match.captured(1);
                        QString lastPart = match.captured(2);
                        qDebug() << "First Part:" << firstPart;
                        lastPart.remove("\"");
                        qDebug() << "Last Part:" << lastPart;
                        process->startDetached(runexeFilePath, QStringList() << firstPart << lastPart);
                    }else{
                        process->startDetached(operation.filePath);
                    }

                    QEventLoop loop;
                    QTimer::singleShot(operation.timeInterval, &loop, &QEventLoop::quit);
                    loop.exec();
                }
                else{
                    QProcess *process = new QProcess();
                    if(lastOperationItem.actionType == OperationItem::ActionType::LaunchSoftware){
                        process->start(clickButtonExePath, QStringList() << operation.filePath << lastOperationItem.filePath);
                    }else{
                        process->start(clickButtonExePath, QStringList() << operation.filePath );
                    }
                    process->waitForFinished(-1);
                }
            }
        }); 
    }
    QFrame* hframe = new QFrame(this);
    hframe->setFrameShape(QFrame::HLine);
    contentLayout->addWidget(hframe);

    ParseJsonAuto();

    for (const auto& pair : presetsAuto) {
        QString name = pair.first;

        QPushButton* presetButton = new QPushButton(name);
        presetButton->setObjectName("presetButton");
        contentLayout->addWidget(presetButton);

        const QList<OperationItemAuto> &operationsAuto = pair.second;

        // 连接按钮的clicked信号到槽函数，以处理按钮点击事件
        QObject::connect(presetButton, &QPushButton::clicked, [&]() {
            this->hide();
            for(const auto& operationAuto : operationsAuto){
                QProcess *process = new QProcess();
                process->startDetached(runexeAutoFilePath, QStringList() << QString::number(operationAuto.posX) << QString::number(operationAuto.posY) << QString::number(operationAuto.width) << QString::number(operationAuto.height) << operationAuto.filePath);
            }
        });
    }
    setAttribute(Qt::WA_DeleteOnClose);
    // 去掉问号，只保留关闭
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);

}


void LoadPreset::ParseJson(){
    QFile file(jsonFilePath);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << file.error();
        return;
    }

    // 读取文件的全部内容
    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);		// 设置读取编码是UTF8
    QString fileData = stream.readAll();
    file.close();


    // QJsonParseError类用于在JSON解析期间报告错误。
    QJsonParseError jsonError;
    // 将json解析为UTF-8编码的json文档，并从中创建一个QJsonDocument。
    // 如果解析成功，返回QJsonDocument对象，否则返回null
    QJsonDocument jsonDoc = QJsonDocument::fromJson(fileData.toUtf8(), &jsonError);
    // 判断是否解析失败
    if (jsonError.error != QJsonParseError::NoError && !jsonDoc.isNull()) {
        qDebug() << "Json格式错误！" << jsonError.error;
        return;
    }

    // 获取根对象
    QJsonObject rootObject = jsonDoc.object();

    // 遍历根对象的键
    for (const QString& key : rootObject.keys()) {
        qDebug() << "键：" << key;
        QList<OperationItem> preset;

        // 获取对应键的数组
        QJsonArray jsonArray = rootObject.value(key).toArray();

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

void LoadPreset::ParseJsonAuto(){
    QFile file(jsonAutoFilePath);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << file.error();
        return;
    }

    // 读取文件的全部内容
    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);		// 设置读取编码是UTF8
    QString fileData = stream.readAll();
    file.close();


    // QJsonParseError类用于在JSON解析期间报告错误。
    QJsonParseError jsonError;
    // 将json解析为UTF-8编码的json文档，并从中创建一个QJsonDocument。
    // 如果解析成功，返回QJsonDocument对象，否则返回null
    QJsonDocument jsonDoc = QJsonDocument::fromJson(fileData.toUtf8(), &jsonError);
    // 判断是否解析失败
    if (jsonError.error != QJsonParseError::NoError && !jsonDoc.isNull()) {
        qDebug() << "Json格式错误！" << jsonError.error;
        return;
    }

    // 获取根对象
    QJsonObject rootObject = jsonDoc.object();

    // 遍历根对象的键
    for (const QString& key : rootObject.keys()) {
        qDebug() << "键：" << key;
        QList<OperationItemAuto> presetAuto;

        // 获取对应键的数组
        QJsonArray jsonArray = rootObject.value(key).toArray();

        // 遍历数组中的对象
        for (const QJsonValue& value : jsonArray) {
            // 获取对象的值
            QJsonObject jsonObject = value.toObject();
            QString file = jsonObject.value("filePath").toString();
            int posX = jsonObject.value("PosX").toInt();
            int posY = jsonObject.value("PosY").toInt();
            int width = jsonObject.value("Width").toInt();
            int height = jsonObject.value("Height").toInt();

            OperationItemAuto operationAuto(posX, posY, width, height, file);
            presetAuto.append(operationAuto);
        }
        QPair<QString, QList<OperationItemAuto>> pair;
        pair.first = key;
        pair.second = presetAuto;
        presetsAuto.append(pair);
    }
    return;
}
