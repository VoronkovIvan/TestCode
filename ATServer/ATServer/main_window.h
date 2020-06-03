#include <QMainwindow>
#include <QPushButton>
#include <QLabel>
#include <QAction>
#include <QTableView>
#include <QStandardItemModel>
#include <QSpinBox>
#include <QHash>
#include <QLineEdit>
#include "server.h"

class MainWindow : public QMainWindow
{
public:
	MainWindow();
	~MainWindow();

private:

	Server *server_ = nullptr;;

	//! Spin box with maximum available tasks
	QSpinBox *box_thread_count_ = nullptr;;
	
	//! Label "Set task count"
	QLabel *label_thread_count_ = nullptr;;

	//! Spin box with maximum available tasks
	QLineEdit *edit_port_ = nullptr;;

	//! Label "Set task count"
	QLabel *label_port_ = nullptr;;

	//! Button start server
	QPushButton *button_start_ = nullptr;;

	//! Action, connected to start button
	QAction *action_start_ = nullptr;;

	//! Main table with tasks in progress
	QTableView *table_tasks_ = nullptr;;

	//! Table item model
	QStandardItemModel *model_ = nullptr;;

	//! Task id from server_cpp and its row in table
	QHash<qint32, qint32>  tasks_and_rows_;

	//------ Methods --------

	//! Init widgets, actions and so on
	void init();

private slots:

	//! Called when we got all server params
	void startServer(qint32 port_id, qint32 max_threads);

	//! Button is pressed obviously
	void buttonPressed();

	//! Called when server says that task is done
	void taskDone(qint32 task_id);

	//! Called when new task received by server
	void newTask(qint32 task_id, QString socket_ip);
};

