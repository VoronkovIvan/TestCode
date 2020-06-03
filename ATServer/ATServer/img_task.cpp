#include "img_task.h"
#include "include/packer.h"
#include <QPainter>
#include <QFontDatabase>
#include <QHostAddress>


ImgTask::ImgTask(qint32 socket_descriptor, qint32 task_id) : socket_descriptor_(socket_descriptor), task_id_(task_id)
{
}


//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

ImgTask::~ImgTask()
{
}

void ImgTask::run()
{
	socket_.reset(new QTcpSocket());
	socket_->setSocketDescriptor(socket_descriptor_);
	emit gotIp(socket_->peerAddress().toString());
	connect(socket_.data(), &QTcpSocket::readyRead, this, &ImgTask::readSocket, Qt::DirectConnection);
	connect(this, &ImgTask::readDone, this, &ImgTask::putTextOnImage, Qt::DirectConnection);

}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

void ImgTask::readSocket()
{
	while (socket_->bytesAvailable())
	{
		if (buffer_.size() == 0)
		{
			QDataStream in(socket_.data());
			in >> bytes_left;
		}

		bytes_left -= socket_->bytesAvailable();
		buffer_.append(socket_->readAll());
	}

	if (bytes_left <= 0)
	{
		emit readDone();
	}

}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

void ImgTask::putTextOnImage(){

	Packer::unpack(buffer_, image_, text_);
	QPainter painter(&image_);

	painter.setPen(QPen(Qt::red));
	painter.setFont(QFont("Times", 24, QFont::Bold));

	painter.drawText(image_.rect(), Qt::AlignCenter, text_);
	painter.end();

	sendData();

	socket_->waitForBytesWritten();
	emit finished(task_id_);
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

void ImgTask::sendData()
{
	QByteArray buf;
	QDataStream out(&buf, QIODevice::WriteOnly);
	out << (qint32)ANSWER_GOOD;

	socket_->write(buf + Packer::pack(image_));
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
