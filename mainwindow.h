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



	// 重载官方函数
private:
	// 窗口大小变化事件
	void resizeEvent(QResizeEvent * event);

	// 自定义函数
private:
	// 初始化调整窗口
	void InitWindnow();
	// 按比例调整窗口各个控件大小
	void ScaleControlSize();
	// 自适应均分tab宽度
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
