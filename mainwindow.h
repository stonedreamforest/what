#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



	// ���عٷ�����
private:
	// ���ڴ�С�仯�¼�
	void resizeEvent(QResizeEvent * event);

	// �Զ��庯��
private:
	// ��ʼ����������
	void InitWindnow();
	// �������������ڸ����ؼ���С
	void ScaleControlSize();
	// ����Ӧ����tab���
	void AverageTabSize(QTabWidget* Tab);

	void jmp_address();
	void attach_process();
	void inject1();
	void inject2();
	void inject3();
	void inject4();
	void inject5();
	void inject6();
	void inject7();
	void run_process();
	void stop_process();
	void set_break_point();
	void tabSelected(int tabIndex);
	void page_refresh();
	void refresh_window_title();

	void inject_select(int actType);

	QAction* new_hotkey(const QString& qacKeyStr);
	void free_hotkey();
private:
    Ui::MainWindow *ui;
	std::vector<QAction*> vec_HotKey;
};

#endif // MAINWINDOW_H
