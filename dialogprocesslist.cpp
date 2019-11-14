#include "DialogProcessList.h"
#include "ui_dialogprocesslist.h"
#include <windows.h>
#include <tlhelp32.h>
#include <Psapi.h>
#include <QMessageBox>

using namespace std;
DialogProcessList::DialogProcessList(QWidget *parent) :
	QDialog(parent) ,
	ui(new Ui::DialogProcessList) {

	ui->setupUi(this);
	connect(ui->listWidget_Process , &QListWidget::itemDoubleClicked , this , &DialogProcessList::get_select_pid);
}

DialogProcessList::~DialogProcessList() {
	free_item();
	delete ui;
}


void DialogProcessList::get_process_list() {
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;

	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS , 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		return;
	}

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if (!Process32First(hProcessSnap , &pe32)) {
		CloseHandle(hProcessSnap);          // clean the snapshot object
		return;
	}

	// Now walk the snapshot of processes, and
	// display information about each process in turn
	do {
		QString	iTemText = QString("%1bit => [0x%2 | %3] => %4").arg(get_process_bits(pe32.th32ProcessID)).arg(pe32.th32ProcessID , 0 , 16).arg(pe32.th32ProcessID).arg(pe32.szExeFile);
		QListWidgetItem *item = new QListWidgetItem;
		item->setText(iTemText);
		ui->listWidget_Process->addItem(item);
		m_vecItem.push_back(item);
		m_vecPid.push_back(pe32.th32ProcessID);
	} while (Process32Next(hProcessSnap , &pe32));

	CloseHandle(hProcessSnap);
	return;
}



void DialogProcessList::get_select_pid(QListWidgetItem* item) {
	Q_UNUSED(item);
	auto selectPid = m_vecPid[ui->listWidget_Process->currentRow()];
	auto cPid = GetCurrentProcessId();
	if (cPid == selectPid) {
		QMessageBox::warning(NULL , QStringLiteral("警告") , QStringLiteral("不能附加自己") , QMessageBox::Yes , QMessageBox::Yes);
		return;
	}
	if (get_process_bits(cPid) != get_process_bits(selectPid)) {
		QMessageBox::warning(NULL , QStringLiteral("警告") , QStringLiteral("请使用对应What附加") , QMessageBox::Yes , QMessageBox::Yes);
		return;
	}
	m_Pid = m_vecPid[ui->listWidget_Process->currentRow()];
	m_isAttached = true;
	auto hProcess = OpenProcess(PROCESS_ALL_ACCESS , FALSE , m_Pid);
	GetModuleFileNameExW(hProcess , NULL , m_exePath , MAX_PATH + 1);
	this->hide();
}

void DialogProcessList::resizeEvent(QResizeEvent* event) {
	Q_UNUSED(event);
	ui->listWidget_Process->move(0 , 0);
	ui->listWidget_Process->resize(this->width() , this->height());
}

void DialogProcessList::free_item() {
	for (const auto& Item : m_vecItem) {
		delete Item;
	}
	m_vecItem.clear();
}

int DialogProcessList::get_process_bits(int Pid) {
	BOOL  Wow64Process;
	auto hProcess = OpenProcess(PROCESS_ALL_ACCESS , FALSE , Pid);
	IsWow64Process(hProcess , &Wow64Process);
	CloseHandle(hProcess);
	return Wow64Process ? 32 : 64;
}

void DialogProcessList::update_process_list() {
	free_item();
	ui->listWidget_Process->clear();
	m_vecPid.clear();
	get_process_list();
}
