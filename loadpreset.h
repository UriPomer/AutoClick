#ifndef LOADPRESET_H
#define LOADPRESET_H

#include <QWidget>

namespace Ui {
class LoadPreset;
}

class LoadPreset : public QWidget
{
    Q_OBJECT

public:
    explicit LoadPreset(QWidget *parent = nullptr);
    ~LoadPreset();

private:
    Ui::LoadPreset *ui;
};

#endif // LOADPRESET_H
