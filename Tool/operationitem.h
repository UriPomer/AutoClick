#ifndef OPERATIONITEM_H
#define OPERATIONITEM_H

#include <QString>

class OperationItem
{
public:
    enum class ActionType {
        LaunchSoftware,
        ClickButton
    };

    OperationItem(ActionType actionType, const QString& filePath, const int timeInterval)
        : actionType(actionType), filePath(filePath), timeInterval(timeInterval)
    {
    }

    ActionType actionType;
    QString filePath;
    int timeInterval;
};

class OperationItemAuto{
public:

    OperationItemAuto(int posX, int posY, int width, int height, QString filePath)
        : posX(posX), posY(posY), width(width), height(height), filePath(filePath)
    {

    }

    int posX;
    int posY;
    int width;
    int height;
    QString filePath;
};

#endif // OPERATIONITEM_H
