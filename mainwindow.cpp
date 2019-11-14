#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <windows.h>
#include "dialogjumptoaddress.h"
#include "./src/TabCpu.h"
#include "./src/TabModules.h"
#include "./src/TabMemory.h"
#include "./src/TabStack.h"
#include "./src/core/Debuger.h"
#include "./src/core/ParseProcess.h"
#include "injectAllTheThings/injectAllTheThings.h"
#include "DialogProcessList.h"
#include <QMessageBox>
#include <string>
#include "Shlwapi.h"

static DialogJumpToAddress* dlgJmpAddress = nullptr;
static TabCpu* tabCpu = nullptr;
static TabModules* tabModules = nullptr;
static TabMemory* tabMemory = nullptr;
static TabStack* tabStack = nullptr;
static DialogProcessList* dlgProcessList;
static ParseProcess* parseProcess = nullptr;
static Debuger* debuger = nullptr;


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent) ,
	ui(new Ui::MainWindow) {
	ui->setupUi(this);

	InitWindnow();

	dlgProcessList = new DialogProcessList;
	debuger = new Debuger;
	parseProcess = new ParseProcess(debuger);
	tabCpu = new TabCpu(ui->tableWidget_Cpu);
	tabModules = new TabModules(ui->treeView_Modules);
	tabMemory = new TabMemory(ui->tableWidget_memView);
	tabStack = new TabStack(ui->tableWidget_Stack);
	dlgJmpAddress = new DialogJumpToAddress(parseProcess);



	connect(ui->actionAttach_Process , &QAction::triggered , this , &MainWindow::attach_process);
	connect(ui->action1 , &QAction::triggered , this , &MainWindow::inject1);
	connect(ui->action2 , &QAction::triggered , this , &MainWindow::inject2);
	connect(ui->action3 , &QAction::triggered , this , &MainWindow::inject3);
	connect(ui->action4 , &QAction::triggered , this , &MainWindow::inject4);
	connect(ui->action5 , &QAction::triggered , this , &MainWindow::inject5);
	connect(ui->action6 , &QAction::triggered , this , &MainWindow::inject6);
	connect(ui->action7 , &QAction::triggered , this , &MainWindow::inject7);
	connect(ui->tabWidget_Main , &QTabWidget::tabBarClicked , this , &MainWindow::tabSelected);
	connect(new_hotkey("ctrl+g") , &QAction::triggered , this , &MainWindow::jmp_address);
	connect(new_hotkey("f9") , &QAction::triggered , this , &MainWindow::run_process);
	connect(new_hotkey("f12") , &QAction::triggered , this , &MainWindow::stop_process);
	connect(new_hotkey("f2") , &QAction::triggered , this , &MainWindow::set_break_point);
	connect(new_hotkey("f5") , &QAction::triggered , this , &MainWindow::page_refresh);
	this->setWindowTitle(QStringLiteral("附加后请使用F5刷新缓存"));
}


MainWindow::~MainWindow() {
	free_hotkey();
	delete dlgJmpAddress;
	delete tabStack;
	delete tabMemory;
	delete tabModules;
	delete tabCpu;
	delete parseProcess;
	delete debuger;
	delete dlgProcessList;
	delete ui;
}

void MainWindow::jmp_address() {
#pragma omp critical
	{
		if (ui->tabWidget_Main->currentIndex() == 0 && dlgProcessList->m_isAttached == true) {
			QString winTitle;
			if (ui->tableWidget_Cpu->hasFocus()) {
				dlgJmpAddress->m_IsUseCpu = true;
				dlgJmpAddress->m_IsUseMemory = false;
				dlgJmpAddress->m_IsUseStack = false;
				winTitle = QStringLiteral("当前选中反汇编窗口...");

				dlgJmpAddress->get_data(dlgProcessList->m_Pid , tabCpu->cpu_view_data);
			}
			if (ui->tableWidget_memView->hasFocus()) {
				dlgJmpAddress->m_IsUseCpu = false;
				dlgJmpAddress->m_IsUseMemory = true;
				dlgJmpAddress->m_IsUseStack = false;
				winTitle = QStringLiteral("当前选中内存窗口...");
				dlgJmpAddress->get_data(dlgProcessList->m_Pid , tabMemory->memory_view_data);
			}
			if (ui->tableWidget_Stack->hasFocus()) {
				dlgJmpAddress->m_IsUseCpu = false;
				dlgJmpAddress->m_IsUseMemory = false;
				dlgJmpAddress->m_IsUseStack = true;
				winTitle = QStringLiteral("当前选中堆栈窗口...");
				dlgJmpAddress->get_data(dlgProcessList->m_Pid , tabStack->stack_view_data);
			}
			dlgJmpAddress->setWindowTitle(winTitle);
			dlgJmpAddress->setModal(true);
			dlgJmpAddress->show();
			refresh_window_title();
		}
	}
}

void MainWindow::resizeEvent(QResizeEvent * event) {
	Q_UNUSED(event);
	ScaleControlSize();
}

void MainWindow::InitWindnow() {
	// 改变tabbar朝向 只需设置一次
	ui->tabWidget_Main->setTabPosition(QTabWidget::North);
	ScaleControlSize();
}

void MainWindow::ScaleControlSize() {
	ui->tabWidget_Main->move(0 , 0);
	ui->tabWidget_Main->resize(this->width() , this->height());

	int cpuHeight = ui->tabWidget_Main->height() * 0.6;
	int cpuWidth = ui->tabWidget_Main->width() * 0.73;

	ui->tableWidget_Cpu->move(0 , 0);
	ui->tableWidget_Cpu->resize(cpuWidth , cpuHeight);

	ui->treeView_Modules->move(0 , 0);
	ui->treeView_Modules->resize(ui->tabWidget_Main->width() , ui->tabWidget_Main->height() - 85);

	int memHeight = ui->tabWidget_Main->height() - cpuHeight;
	ui->tableWidget_memView->move(0 , cpuHeight);
	ui->tableWidget_memView->resize(cpuWidth , memHeight - 85);

	int stackWidth = ui->tabWidget_Main->width() - cpuWidth;
	ui->tableWidget_Stack->move(cpuWidth , 0);
	ui->tableWidget_Stack->resize(stackWidth , ui->tabWidget_Main->height() - 85);
	AverageTabSize(ui->tabWidget_Main);
}

void MainWindow::AverageTabSize(QTabWidget* Tab) {
	auto str_style = QString("QTabBar::tab { height: 30px; width: %1px;}").arg(Tab->width() / Tab->count());
	Tab->setStyleSheet(str_style);
}


void MainWindow::attach_process() {
#pragma omp critical
	{
		dlgProcessList->setModal(true);
	// 遍历进程列表
		dlgProcessList->update_process_list();
		dlgProcessList->show();
	}
}

void MainWindow::inject1() {
	inject_select(1);
}

void MainWindow::inject2() {
	inject_select(2);

}

void MainWindow::inject3() {
	inject_select(3);

}

void MainWindow::inject4() {
	inject_select(4);

}

void MainWindow::inject5() {
	inject_select(5);

}

void MainWindow::inject6() {
	inject_select(6);

}

void MainWindow::inject7() {
	inject_select(7);
}

void MainWindow::run_process() {
#pragma omp critical
	{
		if (ui->tabWidget_Main->currentIndex() == 0) {
			debuger->run();
		}
	}
}

void MainWindow::stop_process() {
#pragma omp critical
	{
		if (ui->tabWidget_Main->currentIndex() == 0) {
			debuger->stop();
		}
	}
}

void MainWindow::set_break_point() {
#pragma omp critical
	{
		if (ui->tabWidget_Main->currentIndex() == 0) {
			auto Item = tabCpu->get_address();
			if (Item.first == nullptr) {
				return;
			}
			if (debuger->unset_bp(Item.first) == true) {
				tabCpu->set_row_background(Item.second , Qt::white);
				return;
			}
			debuger->set_bp(Item.first);
			tabCpu->set_row_background(Item.second , Qt::gray);
		}
	}
}

void MainWindow::tabSelected(int tabIndex) {
#pragma omp critical
	{
		switch (tabIndex) {
			case 1:
				if (dlgProcessList->m_Pid) {
					tabModules->display_modules(dlgProcessList->m_Pid);
				}
				break;
			default:
				break;
		}
	}
}

void MainWindow::page_refresh() {
#pragma omp critical
	{
		if (dlgProcessList->m_isAttached) {
			refresh_window_title();
			auto tabIndex = ui->tabWidget_Main->currentIndex();
			switch (tabIndex) {
				case 0:
					break;
				case 1:
					tabModules->refresh_modules(dlgProcessList->m_Pid);
					break;
				default:
					break;
			}
		}
	}
}

void MainWindow::refresh_window_title() {
	char wTitle[0x300] = { 0 };
	auto szExePath = QString::fromWCharArray(dlgProcessList->m_exePath);
	sprintf_s(wTitle , sizeof(wTitle) , "附加的进程id:[0x%04X | %04d] 路径为：%s <使用F5刷新缓存>" , dlgProcessList->m_Pid , dlgProcessList->m_Pid , szExePath.toLocal8Bit().data());
	this->setWindowTitle(QString::fromLocal8Bit(wTitle));
}

void MainWindow::inject_select(int actType) {
#pragma omp critical
	{
		if (dlgProcessList->m_isAttached) {
			WCHAR crtPath[0x200] = { 0 };
			GetCurrentDirectoryW(sizeof(crtPath) , crtPath);
			auto dllPath = std::wstring(crtPath) + L"\\DbgCore.dbg";
			auto exeName = PathFindFileNameW(dlgProcessList->m_exePath);
			inject_main(actType , dlgProcessList->m_Pid , dllPath.c_str() , exeName);
		} else {
			QMessageBox::warning(NULL , QStringLiteral("警告") , QStringLiteral("请先附加进程") , QMessageBox::Yes , QMessageBox::Yes);
		}
	}
}

QAction* MainWindow::new_hotkey(const QString& qacKeyStr) {
	QAction *Action = new QAction(this);
	Action->setShortcut(qacKeyStr);
	this->addAction(Action);
	vec_HotKey.push_back(Action);
	return Action;
}

void MainWindow::free_hotkey() {
	for (const auto& hotKey : vec_HotKey) {
		delete hotKey;
	}
	vec_HotKey.clear();
}


