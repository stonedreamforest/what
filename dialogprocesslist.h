#ifndef DIALOGPROCESSLIST_H
#define DIALOGPROCESSLIST_H

#include <QDialog>
#include <vector>
#include <QListWidgetItem>
namespace Ui {
class DialogProcessList;
}

class DialogProcessList : public QDialog
{
    Q_OBJECT

public:
    explicit DialogProcessList(QWidget *parent = nullptr);
    ~DialogProcessList();
	void update_process_list();
	void get_select_pid(QListWidgetItem* item);

private:
	void get_process_list();
	void resizeEvent(QResizeEvent* event);
	void free_item();
	int get_process_bits(int Pid);
public:
	int m_Pid;
	bool m_isAttached =false;
	wchar_t m_exePath[0x260];
private:
    Ui::DialogProcessList *ui;
	std::vector<int> m_vecPid;
	std::vector<QListWidgetItem*> m_vecItem;
};

#endif // DIALOGPROCESSLIST_H
