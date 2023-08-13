#include "loadpreset.h"
#include "ui_loadpreset.h"

LoadPreset::LoadPreset(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoadPreset)
{
    ui->setupUi(this);
}

LoadPreset::~LoadPreset()
{
    delete ui;
}
