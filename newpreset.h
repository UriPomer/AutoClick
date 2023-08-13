#ifndef NEWPRESET_H
#define NEWPRESET_H

#include <QWidget>

namespace Ui {
class NewPreset;
}

class NewPreset : public QWidget
{
    Q_OBJECT

public:
    explicit NewPreset(QWidget *parent = nullptr);
    ~NewPreset();

private:
    Ui::NewPreset *ui;
};

#endif // NEWPRESET_H
