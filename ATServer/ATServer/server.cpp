#include <QThread>
#include "server.h"
#include "img_task.h"
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

Server::Server()
{

}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

Server::~Server()
{

}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

bool Server::start()
{
	started_ = true;
	return listen(QHostAddress::Any, port_);
}
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

void Server::stop()
{
	started_ = false;
	close();
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------


void Server::incomingConnection(qintptr socket_descriptor)
{
	if (processing_ < max_processing_) {

		ImgTask *task = new ImgTask(socket_descriptor, task_counter_++);
		QThread *new_thread = new QThread;
		++processing_;

		connect(new_thread, &QThread::finished, task, &ImgTask::deleteLater);
		connect(new_thread, &QThread::finished, new_thread, &QThread::deleteLater);

		connect(task, &ImgTask::finished, [&](qint32 task_id) { --processing_; emit taskClosed(task_id); });
		connect(task, &ImgTask::finished, new_thread, &QThread::quit);
		connect(task, &ImgTask::gotIp, this, &Server::receiveIpFromSocket);

		connect(new_thread, &QThread::started, task, &ImgTask::run);

		task->moveToThread(new_thread);
		new_thread->start();

	}
	else {
		QByteArray data;
		QDataStream out(&data, QIODevice::WriteOnly);
		out << (qint32)ANSWER_BAD;

		QTcpSocket *socket = new QTcpSocket();
		socket->setSocketDescriptor(socket_descriptor);
		socket->write(data);
		socket->waitForBytesWritten();
		socket->disconnect();
		socket->deleteLater();
	}

}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

void Server::receiveIpFromSocket(QString ip)
{
	ImgTask *task = qobject_cast<ImgTask *>(sender());
	emit taskStarted(task->getId(), ip);
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

