#include "autonewpreset.h"

AutoNewPreset::AutoNewPreset()
{
    //设置预设名
    setWindowTitle("设置预设名");

    QVBoxLayout* nameLayout = new QVBoxLayout();
    QLabel* nameLabel = new QLabel("在下方输入预设名称：", this);
    QLineEdit *nameLineEdit = new QLineEdit(this);
    QPushButton *namePushButton = new QPushButton("完成", this);

    //QRegularExpressionValidator* validator = new QRegularExpressionValidator(this);
    //QRegularExpression regExp("[A-Za-z_\\-\\u4E00-\\u9FA5]+"); // 包含英文、下划线、短横线和中文的正则表达式
    //// 应用验证器到LineEdit
    //validator->setRegularExpression(regExp);
    //nameLineEdit->setValidator(validator);


    nameLayout->addWidget(nameLabel);
    nameLayout->addWidget(nameLineEdit);
    nameLayout->addWidget(namePushButton);

    // 创建对话框布局
    QVBoxLayout* dialogLayout = new QVBoxLayout(this);
    dialogLayout->addLayout(nameLayout);

    connect(namePushButton, &QPushButton::clicked, this, [=](){
        QString name = nameLineEdit->text();
        QProcess *process = new QProcess();
        process->startDetached("./ahkEXE/AutoNewProfile.exe",QStringList() << name);
        close();
        delete(process);
    });
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    setAttribute(Qt::WA_DeleteOnClose);

}
