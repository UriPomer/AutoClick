#include "newpreset.h"
#include "loadpreset.h"
#include "modifypreset.h"
#include "deletepreset.h"
#include "autonewpreset.h"
#include <QApplication>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QDebug>
#include <QThread>
#include <QSharedMemory>


#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

class autoClick : public QApplication
{
public:
	autoClick(int argc, char** argv)
		: QApplication(argc, argv)
	{
		QFile file(":/qss/src/MaterialDark.qss");
		file.open(QFile::ReadOnly);
		QString styleSheet = tr(file.readAll());
		this->setStyleSheet(styleSheet);
		
		// 创建系统托盘图标
		trayIcon_ = new QSystemTrayIcon(QIcon(":/icon/src/logo.png"), this);

		// 创建右键菜单
		QMenu* menu = new QMenu();
		QAction* actionNewPreset = new QAction("手动新建预设");
		QAction* actionAutoNewPreset = new QAction("自动新建预设");
		QAction* actionLoadPreset = new QAction("加载预设");
		QAction* actionModifyPreset = new QAction("修改预设");
		QAction* actionDeletePreset = new QAction("删除预设");
		QAction* actionExit = new QAction("退出程序");
		menu->addAction(actionNewPreset);
		menu->addAction(actionAutoNewPreset);
		menu->addAction(actionLoadPreset);
		menu->addAction(actionModifyPreset);
		menu->addAction(actionDeletePreset);
		menu->addSeparator(); // 添加分隔线
		menu->addAction(actionExit);

		// 将右键菜单与托盘图标关联
		trayIcon_->setContextMenu(menu);

		// 连接槽函数以响应菜单项的触发
		connect(actionNewPreset, &QAction::triggered, this, &autoClick::onNewPreset);
		connect(actionLoadPreset, &QAction::triggered, this, &autoClick::onLoadPreset);
		connect(actionAutoNewPreset, &QAction::triggered, this, &autoClick::OnAutoNewPreset);
		connect(actionModifyPreset, &QAction::triggered, this, &autoClick::onModifyPreset);
		connect(actionDeletePreset, &QAction::triggered, this, &autoClick::onDeletePreset);
		connect(actionExit, &QAction::triggered, this, &autoClick::onExit);

		// 显示托盘图标
		trayIcon_->show();
		setQuitOnLastWindowClosed(false);
	}

	void onNewPreset()
	{
		NewPreset* newPreset = new NewPreset();
		
		newPreset->setAttribute(Qt::WA_DeleteOnClose); // 在窗口关闭时自动删除
		newPreset->resize(500, 340);
		newPreset->show();
	}

	void OnAutoNewPreset() {
		AutoNewPreset* autoNewPreset = new AutoNewPreset();
		autoNewPreset->setAttribute(Qt::WA_DeleteOnClose);
		autoNewPreset->show();
	}

	void onLoadPreset()
	{
		LoadPreset* loadPreset = new LoadPreset();
		loadPreset->setAttribute(Qt::WA_DeleteOnClose); // 在窗口关闭时自动删除
		loadPreset->show();
	}

	void onModifyPreset() {
		ModifyPreset* modifyPreset = new ModifyPreset();
		modifyPreset->setAttribute(Qt::WA_DeleteOnClose);
		modifyPreset->show();
	}

	void onDeletePreset() {
		DeletePreset* deletePreset = new DeletePreset();
		deletePreset->setAttribute(Qt::WA_DeleteOnClose);
		deletePreset->show();
	}

	void onExit()
	{
		// 在退出程序之前，确保先删除托盘图标
		trayIcon_->hide();
		delete trayIcon_;

		exit(0);
	}

private:
	QSystemTrayIcon* trayIcon_;
};

int main(int argc, char** argv)
{

	autoClick app(argc, argv);
	static QSharedMemory* singleApp = new QSharedMemory("SingleApp");//创建“SingleApp”的共享内存块
	if (!singleApp->create(1))//创建失败，说明已经有一个程序运行，
	{
		QMessageBox::information(NULL, "提示", "程序已经运行");//弹出提示框 注意：该提示应该在 qApp->quit();之前，否则提示框将会一闪而过
		qApp->quit();//退出当前程序
		return -1;
	}
	return app.exec();
}

