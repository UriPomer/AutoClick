#include "deletepreset.h"
#include "defineConst.h"

DeletePreset::DeletePreset()
{
    setWindowTitle("删除预设");
    ParseJson();

    QHBoxLayout* layout = new QHBoxLayout(this);
    QWidget* contentWidget = new QWidget(this);
    setLayout(layout);
    layout->addWidget(contentWidget);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    setAttribute(Qt::WA_DeleteOnClose);
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);

    // 根据presets列表创建按钮，并添加到弹出对话框中的布局中
    for (const auto& name : presetNames) {
        QPushButton* presetButton = new QPushButton(name);
        presetButton->setObjectName("presetButton");
        contentLayout->addWidget(presetButton);

        // 连接按钮的clicked信号到槽函数，以处理按钮点击事件
        QObject::connect(presetButton, &QPushButton::clicked, [&]() {
            this->hide();
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
            QJsonObject *rootObject = new QJsonObject(jsonDoc.object());
            rootObject->remove(name);
            jsonDoc.setObject(*rootObject);

            // 创建文件并写入JSON数据
            QFile fileNew(jsonFilePath);
            if (!fileNew.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
                QMessageBox::information(nullptr, "提示", "删除失败");
                qDebug() << fileNew.errorString();
                return;
            }

            QTextStream streamNew(&fileNew);
            streamNew.setEncoding(QStringConverter::Utf8);		// 设置写入编码是UTF8
            // 写入文件
            streamNew << jsonDoc.toJson();
            fileNew.close();
            QMessageBox::information(nullptr, "提示", "删除成功");
        });
    }
    QFrame* hframe = new QFrame(this);
    hframe->setFrameShape(QFrame::HLine);
    contentLayout->addWidget(hframe);

    ParseJsonAuto();
    for(const auto &name : presetAutoNames){
        QPushButton* presetButton = new QPushButton(name);
        presetButton->setObjectName("presetButton");
        contentLayout->addWidget(presetButton);

        // 连接按钮的clicked信号到槽函数，以处理按钮点击事件
        QObject::connect(presetButton, &QPushButton::clicked, [&]() {
            this->hide();
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
            QJsonObject *rootObject = new QJsonObject(jsonDoc.object());
            rootObject->remove(name);
            jsonDoc.setObject(*rootObject);

            // 创建文件并写入JSON数据
            QFile fileNew(jsonAutoFilePath);
            if (!fileNew.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
                QMessageBox::information(nullptr, "提示", "删除失败");
                qDebug() << fileNew.errorString();
                return;
            }

            QTextStream streamNew(&fileNew);
            streamNew.setEncoding(QStringConverter::Utf8);		// 设置写入编码是UTF8
            // 写入文件
            streamNew << jsonDoc.toJson();
            fileNew.close();
            QMessageBox::information(nullptr, "提示", "删除成功");
        });
    }
}


void DeletePreset::ParseJson(){
    QFile file(jsonFilePath);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
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
        return;
    }

    // 获取根对象
    QJsonObject *rootObject = new QJsonObject(jsonDoc.object());

    // 遍历根对象的键
    for (const QString& key : rootObject->keys()) {
        presetNames.append(key);
    }
    return;
}

void DeletePreset::ParseJsonAuto(){
    QFile file(jsonAutoFilePath);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
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
        return;
    }

    // 获取根对象
    QJsonObject *rootObject = new QJsonObject(jsonDoc.object());

    // 遍历根对象的键
    for (const QString& key : rootObject->keys()) {
        presetAutoNames.append(key);
    }
    return;
}
