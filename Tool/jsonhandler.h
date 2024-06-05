#pragma once

#include <QObject>
#include "operationitem.h"

class JsonHandler  : public QObject
{
	Q_OBJECT

public:
	JsonHandler(QObject *parent = nullptr);
	~JsonHandler();

	QString GetJsonFileAsQString(QString filePath) const;
	QJsonDocument GetJsonDocFromQString(QString jsonFileData) const;
	QJsonDocument SetJsonDocFromOperationList(QJsonDocument jsonDoc, QList<OperationItem>& operationList, QString jsonDocName);
	bool WriteJsonDocToFilePath(const QJsonDocument& jsonDoc, const QString& filePath) const;
};
