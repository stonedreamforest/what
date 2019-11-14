#pragma once
#ifndef _CPU_H_
#define _CPU_H_
#include <QTableWidget>
#include <vector>
#include <utility>
#include "./core/ParseProcess.h"

class TabCpu {
public:
	explicit TabCpu(QTableWidget *table);
	~TabCpu();

	static void CALLBACK cpu_view_data(const size_t viewAddr , const std::vector<ParseProcess::PDISINFO>& vecDisInfo);

	std::pair<void* , int> get_address();

	void set_row_background(int Row , QColor rowColor);
private:
	void new_item();
	void free_item();
};



#endif // _)CPU_H_

