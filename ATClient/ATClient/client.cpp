#include "client.h"
#include "include/packer.h"

#include <QDataStream>
#include <QTimer>
#include <QImage>



#define ANSWER_BAD 99999
#define ANSWER_GOOD 88888

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------


Client::Client() : socket_(new QTcpSocket)
{
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

Client::~Client()
{
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

void Client::setParameters(QString img_path, QString text, qint32 reconnect_time)
{
	file_info_.setFile(img_path);
	text_ = text;
	reconnect_time_ = reconnect_time;
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

void Client::connectToHost(const QHostAddress &address, quint16 port)
{
	address_ = address;
	port_ = port;

	connect(socket_.data(), &QTcpSocket::connected, this, &Client::sendImage);
	connect(socket_.data(), &QTcpSocket::readyRead, this, &Client::receivedData);
	connect(this, &Client::readDone, this, &Client::dataReceivedAll);
	addLog("Connecting to host " + address.toString() + ":" + QString::number(port));

	socket_->connectToHost(address_, port_);

	if (!socket_->waitForConnected())
	{
		addLog("Server not found.");
		socket_->disconnect();
	}

}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

void Client::reconnect()
{
	addLog("Trying to reconnect..");
	socket_->connectToHost(address_, port_);

}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
void Client::dataReceivedAll()
{
	addLog("Data received successfully");
	socket_->close();

	QImage img;

	Packer::unpack(buffer_, img);

	QString new_file_name = folder_name_ + "/" + file_info_.fileName();

	if (img.save(new_file_name))
		addLog("Image saved succesfully to "  + new_file_name);
	else
		addLog("Cannot save image");

	addLog("Disconnected from host");
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
void Client::addLog(QString log_text)
{
	QTextStream out(stdout);
	out << log_text << endl;
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
void Client::receivedData()
{
	qint32 prefix;
	while (socket_->bytesAvailable())
	{
		if (buffer_.size() == 0)
		{
			QDataStream in(socket_.data());
			in >> prefix;

			if (prefix == 99999)
			{
				addLog("Server busy. Reconnecting in " + QString::number(reconnect_time_/1000) + " seconds");
				QTimer::singleShot(reconnect_time_, this, &Client::reconnect);
				return;
			}
			else if (prefix == 88888)
			{
				addLog("Receiving data..");
				in >> bytes_left;
			}
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

void Client::sendImage()
{
	if (socket_->waitForReadyRead(2000)) 
	{
		return;
	}

	addLog("Connected succesfully");
	addLog("Sending task...");
	QString filename1 = file_info_.absoluteFilePath();
	QImage img(file_info_.absoluteFilePath());
	QByteArray data = Packer::pack(img, text_);

	socket_->write(data);

}
