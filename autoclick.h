#ifndef AUTOCLICK_H
#define AUTOCLICK_H

#include <QWidget>

class autoClick : public QWidget
{
    Q_OBJECT

public:
    autoClick(QWidget *parent = nullptr);
    ~autoClick();
};
#endif // AUTOCLICK_H
