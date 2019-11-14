#ifndef DIALOGJUMPTOADDRESS_H
#define DIALOGJUMPTOADDRESS_H

#include <QDialog>
#include <vector>

#include "./src/core/ParseProcess.h"

namespace Ui {
	class DialogJumpToAddress;
}

class DialogJumpToAddress : public QDialog {
	Q_OBJECT

public:
	explicit DialogJumpToAddress(ParseProcess* parseProcess , QWidget *parent = nullptr);
	~DialogJumpToAddress();

	void get_address();

	void accept() final;
	void reject() final;

	void get_data(int Pid , void* callback_viewdata);

	void free_meminfo(std::vector<ParseProcess::PMEMPAGEINFO>& vecMemPageInfo);

	void free_disinfo(std::vector<ParseProcess::PDISINFO>& vecDisInfo);
public:
	bool m_IsUseCpu = false;
	bool m_IsUseStack = false;
	bool m_IsUseMemory = false;
private:
	Ui::DialogJumpToAddress *ui;
	int m_Pid = 0;
	void* m_callback_viewdata = nullptr;
	ParseProcess* m_parseProcess;
};

#endif // DIALOGJUMPTOADDRESS_H
