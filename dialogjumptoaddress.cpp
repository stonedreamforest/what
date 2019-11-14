#include "dialogjumptoaddress.h"
#include "ui_dialogjumptoaddress.h"
#include <QMessageBox>



DialogJumpToAddress::DialogJumpToAddress(ParseProcess* parseProcess , QWidget *parent) :
	QDialog(parent) ,
	ui(new Ui::DialogJumpToAddress) {
	ui->setupUi(this);

	m_parseProcess = parseProcess;

	connect(ui->textEdit_Address , &QTextEdit::textChanged , this , &DialogJumpToAddress::get_address);
}

DialogJumpToAddress::~DialogJumpToAddress() {
	delete ui;
}

void DialogJumpToAddress::get_address() {
	// 先不做实时处理
	//QMessageBox::information(NULL , "Title" , ui->textEdit_Address->toPlainText() , QMessageBox::Yes | QMessageBox::No , QMessageBox::Yes);
}

void DialogJumpToAddress::accept() {
	bool ok;
	auto targetAddr = ui->textEdit_Address->toPlainText().toULongLong(&ok , 16);
	if (ok == false) {
		QMessageBox::warning(NULL , QStringLiteral("警告") , QStringLiteral("解析输入地址出错") , QMessageBox::Yes , QMessageBox::Yes);
		return;
	}

	std::vector<ParseProcess::PMEMPAGEINFO> vecMemPageInfo;
	if (m_parseProcess->read_memory_by_pid(this->m_Pid , vecMemPageInfo , targetAddr) == false) {
		QMessageBox::warning(NULL , QStringLiteral("警告") , QStringLiteral("打开进程出错") , QMessageBox::Yes , QMessageBox::Yes);
		return;
	}
	if (m_IsUseCpu == true) {
		std::vector<ParseProcess::PDISINFO> vecDisInfo;
		m_parseProcess->zydis_data(targetAddr , vecMemPageInfo[0] , vecDisInfo);

		typedef void(CALLBACK *pViewData)(const size_t viewAddr , const std::vector<ParseProcess::PDISINFO>& vecDisInfo);
		pViewData cpu_view_data = reinterpret_cast<pViewData>(m_callback_viewdata);
		cpu_view_data(targetAddr , vecDisInfo);

		if (vecDisInfo.size() != 0) {
			this->free_disinfo(vecDisInfo);
		}
	} else if (m_IsUseMemory == true) {
		typedef void(CALLBACK *pViewData)(size_t viewAddr , const unsigned char* memBuffer);
		pViewData memory_view_data = reinterpret_cast<pViewData>(m_callback_viewdata);
		auto startOffset = targetAddr - (size_t)vecMemPageInfo[0]->memBasicInfo.BaseAddress;
		memory_view_data(targetAddr , &vecMemPageInfo[0]->Buffer[startOffset]);
	} else if (m_IsUseStack == true) {
		typedef void(CALLBACK *pViewData)(size_t viewAddr , const unsigned char* memBuffer);
		pViewData stack_view_data = reinterpret_cast<pViewData>(m_callback_viewdata);
		auto startOffset = targetAddr - (size_t)vecMemPageInfo[0]->memBasicInfo.BaseAddress;
		stack_view_data(targetAddr , &vecMemPageInfo[0]->Buffer[startOffset]);
	}

	if (vecMemPageInfo.size() != 0) {
		this->free_meminfo(vecMemPageInfo);
	}

	this->hide();
}

void DialogJumpToAddress::reject() {
	this->hide();
}

void DialogJumpToAddress::get_data(int Pid , void* callback_viewdata) {
	m_callback_viewdata = callback_viewdata;
	this->m_Pid = Pid;
}


void DialogJumpToAddress::free_meminfo(std::vector<ParseProcess::PMEMPAGEINFO>& vecMemPageInfo) {
	for (auto memInfo : vecMemPageInfo) {
		free(memInfo->Buffer);
		free(memInfo);
	}
	vecMemPageInfo.clear();
}



void DialogJumpToAddress::free_disinfo(std::vector<ParseProcess::PDISINFO>& vecDisInfo) {
	for (auto disInfo : vecDisInfo) {
		free(disInfo);
	}
	vecDisInfo.clear();
}