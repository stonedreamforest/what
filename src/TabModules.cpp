#include "TabModules.h"
#include <QMessageBox>

#include "./core/ImpPeparse.h"

TabModules::TabModules(QTreeView* TreeView) {
	m_modTreeView = TreeView;
	m_treeModel = new QStandardItemModel(m_modTreeView);
	m_treeModel->setHorizontalHeaderLabels(QStringList() << QStringLiteral("ģ��") << QStringLiteral("��ϸ����") << QStringLiteral("����c++����"));

	//2����QTreeViewӦ��model
	m_modTreeView->setModel(m_treeModel);

}

TabModules::~TabModules() {
	delete m_treeModel;
}



bool TabModules::ListProcessModules(int dwPID , std::vector<PMODULEENTRY32>& vecModInfo) {
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 me32;

	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE , dwPID);
	if (hModuleSnap == INVALID_HANDLE_VALUE) {
		QMessageBox::warning(NULL , QStringLiteral("����") , QStringLiteral("��ȡģ�����") , QMessageBox::Yes , QMessageBox::Yes);
		return false;

	}

	me32.dwSize = sizeof(MODULEENTRY32);


	if (!Module32First(hModuleSnap , &me32)) {
		QMessageBox::warning(NULL , QStringLiteral("����") , QStringLiteral("��ȡģ�����2") , QMessageBox::Yes , QMessageBox::Yes);
		CloseHandle(hModuleSnap);
		return false;
	}


	do {
		auto Mod = (PMODULEENTRY32)malloc(sizeof(MODULEENTRY32));
		memcpy(Mod , &me32 , sizeof(MODULEENTRY32));
		vecModInfo.push_back(Mod);
	} while (Module32Next(hModuleSnap , &me32));

	CloseHandle(hModuleSnap);
	return true;
}



void TabModules::display_modules(int Pid) {
	if (m_isRefresh == false) {
		return;
	}
	m_isRefresh = false;

	std::vector<PMODULEENTRY32> vecModInfo;
	if (ListProcessModules(Pid , vecModInfo) == false) {
		return;
	}


	for (const auto& Mod : vecModInfo) {
		//һ���ڵ㣬����mModel
		QList<QStandardItem*> treeRoot1;
		auto szExePath = QString::fromWCharArray(Mod->szExePath);
		QStandardItem* root1Item1 = new QStandardItem(QString::fromWCharArray(Mod->szModule));
		QStandardItem* root1item2 = new QStandardItem(szExePath);


		treeRoot1.append(root1Item1);
		treeRoot1.append(root1item2);
		m_treeModel->appendRow(treeRoot1);
		ImpPeparse PE(szExePath.toLocal8Bit().data());


		for (int j = 0; j < 2; j++) {
			//�����ڵ�,�����1��һ���ڵ�
			QList<QStandardItem*> items2;

			QStandardItem* item3;

			if (j == 0) {
				item3 = new QStandardItem(QStringLiteral("�����"));
			} else if (j == 1) {
				item3 = new QStandardItem(QStringLiteral("������"));
			}
			items2.append(item3);
			root1Item1->appendRow(items2);


			QStandardItem* item5;
			QStandardItem* item6;
			QStandardItem* item7;
			if (j == 0) {
				auto impCount = PE.m_vecImpFunAddr.size();
				for (auto k = 0u; k < impCount; k++) {
				//�����ڵ�,�����1�������ڵ�
					QList<QStandardItem*> items3;


					item5 = new QStandardItem(QString::number(PE.m_vecImpFunAddr[k] , 16));
					item6 = new QStandardItem(QString(PE.m_vecImpString[k].c_str()));
					item7 = new QStandardItem(QString(PE.m_vecDecodingImpString[k].c_str()));

					items3.append(item5);
					items3.append(item6);
					items3.append(item7);
					item3->appendRow(items3);
				}
			} else if (j == 1) {
				if (PE.m_IsHasExports) {
					auto expCount = PE.m_vecExpFunAddr.size();
					for (auto k = 0u; k < expCount; k++) {
						QList<QStandardItem*> items3;

						item5 = new QStandardItem(QString::number(PE.m_vecExpFunAddr[k] + reinterpret_cast<size_t>(Mod->modBaseAddr) , 16));
						item6 = new QStandardItem(QString(PE.m_vecExpString[k].c_str()));
						item7 = new QStandardItem(QString(PE.m_vecDecodingExpString[k].c_str()));

						items3.append(item5);
						items3.append(item6);
						items3.append(item7);

						item3->appendRow(items3);
					}
				}
			}
		}
	}

	for (auto Mod : vecModInfo) {
		delete Mod;
	}
	vecModInfo.clear();
}

void TabModules::refresh_modules(int Pid) {
	m_isRefresh = true;
	m_treeModel->clear();
	m_treeModel->setHorizontalHeaderLabels(QStringList() << QStringLiteral("ģ��") << QStringLiteral("��ϸ����") << QStringLiteral("����1"));
	display_modules(Pid);
}
