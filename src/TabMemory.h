#pragma once
#ifndef _TAB_MEMORY_H_
#define _TAB_MEMORY_H_


#include <QTableWidget>
#include <windows.h>

class TabMemory {
public:
	explicit TabMemory(QTableWidget *Table);
	~TabMemory();

	static void CALLBACK memory_view_data(size_t viewAddr , const unsigned char* vecDisInfo);
private:
	void new_item();
	void free_item();

};






#endif // !_TAB_MEMORY_H_




