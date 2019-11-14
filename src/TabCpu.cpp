#include "TabCpu.h"
#include <QHeaderView>
#include <QMessageBox>
#include <array>
#include <thread>


static QTableWidget *m_cpuTable = nullptr;
constexpr auto ITEM_CPU_ROW_COUNT = 0x1000;
std::array<QTableWidgetItem* , ITEM_CPU_ROW_COUNT> arry_ItemVirAddress;
std::array<QTableWidgetItem* , ITEM_CPU_ROW_COUNT> arry_ItemDisByte;
std::array<QTableWidgetItem* , ITEM_CPU_ROW_COUNT> arry_ItemDisCode;
std::array<QTableWidgetItem* , ITEM_CPU_ROW_COUNT> arry_ItemComment;

TabCpu::TabCpu(QTableWidget *Table) {

	m_cpuTable = Table;
	//��ʼ�����
	m_cpuTable->setWindowTitle("user list");  //����
	// �����Ƿ���ʾ�����
	m_cpuTable->setShowGrid(true);
	//����ѡ�еķ�ʽ
	m_cpuTable->setSelectionBehavior(QAbstractItemView::SelectRows);






   //����������
	m_cpuTable->setColumnCount(4);
	m_cpuTable->setRowCount(ITEM_CPU_ROW_COUNT);
   //����ÿ�п�
	for (auto Table = 0; Table < ITEM_CPU_ROW_COUNT; Table++) {
		//����ÿ�п���
		m_cpuTable->setColumnWidth(Table , 150);
		m_cpuTable->setRowHeight(Table , 20);
	}


	std::thread tempThread(&TabCpu::new_item , this);
	tempThread.detach();

	//�������б���
	//�����Ļ���һ�к͵�һ����������̶���ʾ�Ļ� Ҫ��ֹʹ��Ĭ�϶�ά���� x+1 y+1

	//m_cpuTable->horizontalHeader()->setVisible(false);
	//m_cpuTable->verticalHeader()->setVisible(false);




	QStringList header;
	header.append(QStringLiteral("�����ַ"));
	header.append(QStringLiteral("�ֽ���"));
	header.append(QStringLiteral("������"));
	header.append(QStringLiteral("ע��"));
	m_cpuTable->setHorizontalHeaderLabels(header);





	//�Զ��������һ��ʹ֮�������
	m_cpuTable->horizontalHeader()->setStretchLastSection(true);

}

TabCpu::~TabCpu() {
	free_item();
}

void CALLBACK TabCpu::cpu_view_data(const size_t viewAddr , const std::vector<ParseProcess::PDISINFO>& vecDisInfo) {

	//���ñ�������
	QString iTemText;

	// ��ֹʵ�ʷ���ຯ��������������
	auto realRow = vecDisInfo.size() > ITEM_CPU_ROW_COUNT ? ITEM_CPU_ROW_COUNT : vecDisInfo.size();

	for (auto Row = 0u; Row < realRow; Row++) {
		iTemText = QString("%1").arg(vecDisInfo[Row]->virtualAddress);
		arry_ItemVirAddress[Row]->setText(iTemText);

		iTemText = QString("%1").arg(vecDisInfo[Row]->disByte);
		arry_ItemDisByte[Row]->setText(iTemText);

		iTemText = QString("%1").arg(vecDisInfo[Row]->disCode);
		arry_ItemDisCode[Row]->setText(iTemText);

		m_cpuTable->setItem(Row , 0 , arry_ItemVirAddress[Row]);
		m_cpuTable->setItem(Row , 1 , arry_ItemDisByte[Row]);
		m_cpuTable->setItem(Row , 2 , arry_ItemDisCode[Row]);
		//m_cpuTable->setItem(iRow , 3 , arry_ItemComment[iRow]);
	}
	bool ok;
	auto rowIndex = 0;
	for (auto Row = 0u; Row < vecDisInfo.size(); Row++) {
		auto TempAddr = QString(vecDisInfo[Row]->virtualAddress).toULongLong(&ok , 16);
		if (ok == false) {
			QMessageBox::warning(NULL , QStringLiteral("����") , QStringLiteral(" ���� TempAddress ����") , QMessageBox::Yes , QMessageBox::Yes);
			return;
		}
		if (TempAddr == viewAddr) {
			rowIndex = Row;
			break;
		}
	}
	m_cpuTable->scrollToItem(m_cpuTable->item(rowIndex , 0) , QAbstractItemView::PositionAtCenter);
	m_cpuTable->selectRow(rowIndex);
}


std::pair<void* , int> TabCpu::get_address() {
	bool ok;
	std::pair<void* , int> Item(0 , 0);
	auto Row = m_cpuTable->currentRow();
	auto VirtualAddress = arry_ItemVirAddress[Row]->text().toULongLong(&ok , 16);
	if (ok == false) {
		return Item;
	}
	Item.first = reinterpret_cast<void*>(VirtualAddress);
	Item.second = Row;
	return Item;
}

void TabCpu::set_row_background(int Row , QColor rowColor) {
	m_cpuTable->item(Row , 0)->setBackground(rowColor);
	m_cpuTable->item(Row , 1)->setBackground(rowColor);
	m_cpuTable->item(Row , 2)->setBackground(rowColor);
}

void TabCpu::new_item() {
	for (auto i = 0; i < ITEM_CPU_ROW_COUNT; i++) {
		arry_ItemVirAddress[i] = new QTableWidgetItem;
		arry_ItemDisByte[i] = new QTableWidgetItem;
		arry_ItemDisCode[i] = new QTableWidgetItem;
		arry_ItemComment[i] = new QTableWidgetItem;
	}
}

void TabCpu::free_item() {
	for (auto i = 0; i < ITEM_CPU_ROW_COUNT; i++) {
		delete arry_ItemVirAddress[i];
		delete arry_ItemDisByte[i];
		delete arry_ItemDisCode[i];
		delete arry_ItemComment[i];
	}
}
