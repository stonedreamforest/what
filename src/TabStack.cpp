#include "TabStack.h"
#include <QHeaderView>
#include <array>
#include <thread>

constexpr auto STACK_ITEM_ROW_COUNT = 0x100;
static std::array<QTableWidgetItem* , STACK_ITEM_ROW_COUNT> arry_ItemStackAddress;
static std::array<QTableWidgetItem* , STACK_ITEM_ROW_COUNT> arry_ItemStackValue;

static QTableWidget *m_stackTable;

TabStack::TabStack(QTableWidget * Table) {
	m_stackTable = Table;
	//初始化表格
	m_stackTable->setWindowTitle("user list");  //表名
	// 设置是否显示表格线
	m_stackTable->setShowGrid(true);
	//整行选中的方式
	m_stackTable->setSelectionBehavior(QAbstractItemView::SelectRows);


	std::thread tempThread(&TabStack::new_item , this);
	tempThread.detach();



   //设置行列数
	m_stackTable->setColumnCount(2);
	m_stackTable->setRowCount(STACK_ITEM_ROW_COUNT);
   //设置每列宽
	for (auto Table = 0; Table < STACK_ITEM_ROW_COUNT; Table++) {
		m_stackTable->setColumnWidth(Table , 150);
		m_stackTable->setRowHeight(Table , 20);
	}





	QStringList header;
	header.append(QStringLiteral("栈"));
	header.append(QStringLiteral("值"));
	m_stackTable->setHorizontalHeaderLabels(header);




	//自动调整最后一列使之与表格对齐
	m_stackTable->horizontalHeader()->setStretchLastSection(true);
}

TabStack::~TabStack() {
	free_item();
}

void CALLBACK TabStack::stack_view_data(size_t viewAddr , const unsigned char* memBuffer) {
	QString iTemText;

#ifdef _WIN64
	constexpr int numView = 16;
#else
	constexpr int numView = 8;
#endif // _WIN64



	size_t* Buffer = (size_t*)memBuffer;

	for (auto Row = 0; Row < STACK_ITEM_ROW_COUNT; Row++ , viewAddr += sizeof(void*)) {
		iTemText = QString("%1 => ").arg(viewAddr , numView , 16 , QChar('0'));
		arry_ItemStackAddress[Row]->setText(iTemText.toUpper());
		m_stackTable->setItem(Row , 0 , arry_ItemStackAddress[Row]);


		iTemText = QString("%1").arg(Buffer[Row] , numView , 16 , QChar('0'));
		arry_ItemStackValue[Row]->setText(iTemText.toUpper());
		m_stackTable->setItem(Row , 1 , arry_ItemStackValue[Row]);
	}
	m_stackTable->scrollToItem(m_stackTable->item(0 , 0) , QAbstractItemView::PositionAtTop);
}

void TabStack::new_item() {
	for (auto Row = 0; Row < STACK_ITEM_ROW_COUNT; Row++) {
		arry_ItemStackAddress[Row] = new QTableWidgetItem;
		arry_ItemStackValue[Row] = new QTableWidgetItem;
	}
}

void TabStack::free_item() {
	for (auto Row = 0; Row < STACK_ITEM_ROW_COUNT; Row++) {
		delete arry_ItemStackAddress[Row];
		delete arry_ItemStackValue[Row];
	}
}
