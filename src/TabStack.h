#pragma once
#ifndef _TAB_STACK_H_
#define _TAB_STACK_H_


#include <QTableWidget>
#include <windows.h>






class TabStack {
public:
	explicit TabStack(QTableWidget *Table);
	~TabStack();


	static void CALLBACK stack_view_data(size_t viewAddr , const unsigned char* memBuffer);
private:
	void new_item();
	void free_item();
};




#endif // !_TAB_STACK_H_












