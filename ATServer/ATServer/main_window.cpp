#include "main_window.h"
#include <QLayout>
#include <QHeaderView>

MainWindow::MainWindow()
{
	init();
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

MainWindow::~MainWindow()
{
	delete server_;
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

void MainWindow::init()
{
	server_ = new Server();
	connect(server_, &Server::taskStarted, this, &MainWindow::newTask);
	connect(server_, &Server::taskClosed, this, &MainWindow::taskDone);

	setFixedSize(350, 500);


	box_thread_count_ = new QSpinBox();
	label_thread_count_ = new QLabel();
	edit_port_ = new QLineEdit();
	label_port_ = new QLabel();
	button_start_ = new QPushButton();
	action_start_ = new QAction();
	table_tasks_ = new QTableView();
	model_ = new QStandardItemModel();

	label_thread_count_->setText("Set task count:");
	label_port_->setText("Set port #:");
	button_start_->setText("Start");
	edit_port_->setText("1234");
	edit_port_->setValidator(new QIntValidator(1, 9999, this));

	table_tasks_->setModel(model_);
	table_tasks_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	model_->setHorizontalHeaderLabels({ "Task id", "Client ip address" });

	connect(button_start_, &QPushButton::clicked, this, &MainWindow::buttonPressed);

	QGridLayout *main_layout = new QGridLayout();
	main_layout->addWidget(label_thread_count_, 0, 0);
	main_layout->addWidget(box_thread_count_, 0, 1);
	main_layout->addWidget(label_port_, 1, 0);
	main_layout->addWidget(edit_port_, 1, 1);
	main_layout->addWidget(table_tasks_, 2, 0, 1, 2);
	main_layout->addWidget(button_start_, 3, 1, 1, 1);

	QWidget *widget = new QWidget();
	widget->setLayout(main_layout);
	setCentralWidget(widget);
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

void MainWindow::startServer(qint32 port_id, qint32 max_threads)
{
	server_->setParams(port_id, max_threads);
	server_->start();
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

void MainWindow::buttonPressed()
{
	if (server_->isStarted())
	{
		button_start_->setText("Start");
		edit_port_->setEnabled(true);
		box_thread_count_->setEnabled(true);
		server_->stop();
	} else
	{
		button_start_->setText("Stop");
		edit_port_->setEnabled(false);
		box_thread_count_->setEnabled(false);
		startServer(edit_port_->text().toInt(), box_thread_count_->value());
	}

}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

void MainWindow::taskDone(qint32 task_id)
{
	for (qint32 i = 0; i < model_->rowCount(); ++i)
	{
		auto item = model_->item(i, 0);
		if (item->text().toInt() == task_id)
		{
			model_->removeRow(i);
			break;
		}
	}
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

void MainWindow::newTask(qint32 task_id, QString socket_ip)
{
	model_->appendRow({ new QStandardItem(QString::number(task_id)), new QStandardItem(socket_ip) });
	tasks_and_rows_.insert(task_id, model_->rowCount() - 1);
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
