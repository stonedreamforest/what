#pragma once
#ifndef _TAB_MODULES_H_
#define _TAB_MODULES_H_
#include <QTreeView>
#include <QStandardItem>
#include <vector>
#include <windows.h>
#include <tlhelp32.h>








class TabModules {
public:
	TabModules(QTreeView* listView);
	~TabModules();




	bool ListProcessModules(int dwPID , std::vector<PMODULEENTRY32>& vecModInfo);

	void display_modules(int Pid);
	void refresh_modules(int Pid);

private:
	bool m_isRefresh = true;
	QTreeView* m_modTreeView;
	QStandardItemModel* m_treeModel;
};








#endif // !_TAB_MODULES_H_




