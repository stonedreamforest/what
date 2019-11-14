#include "TabMemory.h"
#include <QHeaderView>
#include <thread>
#include <QTextCodec>
#include <vector>
#include <utility>
#include "./core/ParseProcess.h"
constexpr auto MEM_ITEM_ROW_COUNT = 100;
constexpr auto MEM_BYTE_ITEM_COUNT = MEM_ITEM_ROW_COUNT * 16;

static std::array<QTableWidgetItem* , MEM_BYTE_ITEM_COUNT> arry_ItemMemByte;
static std::array<QTableWidgetItem* , MEM_ITEM_ROW_COUNT> arry_ItemVirAddress;
static std::array<QTableWidgetItem* , MEM_ITEM_ROW_COUNT> arry_ItemUtf8;
static std::array<QTableWidgetItem* , MEM_ITEM_ROW_COUNT> arry_ItemUtf16;
static std::array<QTableWidgetItem* , MEM_ITEM_ROW_COUNT> arry_ItemGBK18030;


static QTableWidget *m_memTable;

TabMemory::TabMemory(QTableWidget* Table) {

	m_memTable = Table;
	//初始化表格
	m_memTable->setWindowTitle("user list");  //表名
	// 设置是否显示表格线
	m_memTable->setShowGrid(true);
	//整行选中的方式
	m_memTable->setSelectionBehavior(QAbstractItemView::SelectRows);



	std::thread tempThread(&TabMemory::new_item , this);
	tempThread.detach();


   //设置行列数
	auto colCount = 20;
	m_memTable->setColumnCount(colCount);
	m_memTable->setRowCount(MEM_ITEM_ROW_COUNT);
   //设置每列宽
	for (auto Table = 0; Table < MEM_ITEM_ROW_COUNT; Table++) {
		//设置每行宽、高

		auto Width = 20;
		auto Height = 20;
		if (Table == 0) {
		#ifdef _WIN64
			Width = 150;

		#else
			Width = 100;
		#endif // _WIN64

		}
		if ((colCount - 1) == Table ||
			(colCount - 2) == Table ||
			(colCount - 3) == Table) {
			Width = 233;
		}
		m_memTable->setColumnWidth(Table , Width);
		m_memTable->setRowHeight(Table , Height);
	}





	QStringList header;
	header.append(QStringLiteral("内存地址"));
	for (size_t i = 0; i < 16; i++) {
		header.append(QString("%1").arg(i + 1));
	}
	header.append(QStringLiteral("UTF-8"));
	header.append(QStringLiteral("UTF-16"));
	header.append(QStringLiteral("GBK18030"));

	m_memTable->setHorizontalHeaderLabels(header);





	//自动调整最后一列使之与表格对齐
	m_memTable->horizontalHeader()->setStretchLastSection(true);
}

TabMemory::~TabMemory() {
	free_item();
}

void CALLBACK TabMemory::memory_view_data(size_t viewAddr , const unsigned char* memBuffer) {

#ifdef _WIN64
	constexpr int numView = 16;
#else
	constexpr int numView = 8;
#endif // _WIN64
	//设置表格的内容
	QString iTemText;

	// 防止实际反汇编函数大于申请行数
	QTextCodec *Utf16 = QTextCodec::codecForName("UTF-16");
	QTextCodec *Gbk18030 = QTextCodec::codecForName("GB18030");

	for (auto Row = 0 , byteCount = 0; Row < MEM_ITEM_ROW_COUNT; Row++ , viewAddr += 16) {
		iTemText = QString("%1 => ").arg(viewAddr , numView , 16 , QChar('0'));
		arry_ItemVirAddress[Row]->setText(iTemText.toUpper());
		m_memTable->setItem(Row , 0 , arry_ItemVirAddress[Row]);

		QString ansiItemText;
		char encodeBuffer[0x20]={0};
		for (auto Col = 0; Col < 16; Col++) {
			iTemText = QString("%1").arg(memBuffer[byteCount] , 2 , 16 , QChar('0'));
			arry_ItemMemByte[byteCount]->setText(iTemText.toUpper());
			m_memTable->setItem(Row , Col + 1 , arry_ItemMemByte[byteCount]);

			ansiItemText += memBuffer[byteCount];
			encodeBuffer[Col] = memBuffer[byteCount];
			byteCount++;
		}
		arry_ItemUtf8[Row]->setText(ansiItemText);
		m_memTable->setItem(Row , 17 , arry_ItemUtf8[Row]);

		arry_ItemUtf16[Row]->setText(Utf16->toUnicode(encodeBuffer));
		m_memTable->setItem(Row , 18 , arry_ItemUtf16[Row]);

		arry_ItemGBK18030[Row]->setText(Gbk18030->toUnicode(encodeBuffer));
		m_memTable->setItem(Row , 19 , arry_ItemGBK18030[Row]);
	}
	m_memTable->scrollToItem(m_memTable->item(0 , 0) , QAbstractItemView::PositionAtTop);

}

void TabMemory::new_item() {
	for (auto i = 0; i < MEM_BYTE_ITEM_COUNT; i++) {
		arry_ItemMemByte[i] = new QTableWidgetItem;
	}
	for (auto i = 0; i < MEM_ITEM_ROW_COUNT; i++) {
		arry_ItemVirAddress[i] = new QTableWidgetItem;
		arry_ItemUtf8[i] = new QTableWidgetItem;
		arry_ItemUtf16[i] = new QTableWidgetItem;
		arry_ItemGBK18030[i] = new QTableWidgetItem;
	}
}

void TabMemory::free_item() {
	for (auto i = 0; i < MEM_BYTE_ITEM_COUNT; i++) {
		delete arry_ItemMemByte[i];
	}
	for (auto i = 0; i < MEM_ITEM_ROW_COUNT; i++) {
		delete arry_ItemVirAddress[i];
		delete arry_ItemUtf8[i];
		delete arry_ItemUtf16[i];
		delete arry_ItemGBK18030[i];
		
	}
}
