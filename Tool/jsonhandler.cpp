#include "jsonhandler.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include "defineConst.h"


JsonHandler::JsonHandler(QObject* parent)
    : QObject(parent)
{

}


JsonHandler::~JsonHandler() = default;

QString JsonHandler::GetJsonFileAsQString(QString filePath) const
{
    QFile file(filePath);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        throw std::runtime_error("JSON DECODING ERROR");
    }
    // 读取文件的全部内容
    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);		// 设置读取编码是UTF8
    QString fileData = stream.readAll();
    file.close();

    return fileData;
}

QJsonDocument JsonHandler::GetJsonDocFromQString(QString jsonFileData) const{
    // QJsonParseError类用于在JSON解析期间报告错误。
    QJsonParseError jsonError;
    // 将json解析为UTF-8编码的json文档，并从中创建一个QJsonDocument。
    // 如果解析成功，返回QJsonDocument对象，否则返回null
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonFileData.toUtf8(), &jsonError);
    // 判断是否解析失败
    if (jsonError.error != QJsonParseError::NoError && !jsonDoc.isNull()) {
        throw std::runtime_error("JSON 解析错误");
    }
    return jsonDoc;
}

QJsonDocument JsonHandler::SetJsonDocFromOperationList(QJsonDocument jsonDoc, QList<OperationItem>& operationList, QString jsonDocName){
    // 获取根对象
    QJsonObject rootObject = jsonDoc.object();
    QJsonArray jsonArray;
    for (const OperationItem& operation : operationList) {
        QJsonObject jsonObj;
        if (operation.actionType == OperationItem::ActionType::LaunchSoftware) {
            jsonObj.insert("type", "LaunchSoftware");
        }
        else if (operation.actionType == OperationItem::ActionType::ClickButton) {
            jsonObj.insert("type", "ClickButton");
        }
        jsonObj.insert("file", operation.filePath);
        jsonObj.insert("time", operation.timeInterval);
        jsonArray.append(jsonObj);
    }
    rootObject.insert(jsonDocName, jsonArray);
    jsonDoc.setObject(rootObject);
    return jsonDoc;
}

bool JsonHandler::WriteJsonDocToFilePath(const QJsonDocument& jsonDoc,const QString& filePath) const {
	// 创建文件并写入JSON数据
	QFile fileNew(filePath);
    if (!fileNew.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        return false;
	}

	QTextStream streamNew(&fileNew);
	streamNew.setEncoding(QStringConverter::Utf8);
	streamNew << jsonDoc.toJson();
	fileNew.close();
	return true;
}

