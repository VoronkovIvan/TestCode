#pragma once
#include <QtCore/QCoreApplication>
#include <QTcpSocket>
#include <QScopedPointer>
#include <QFileInfo>
#include <QHostAddress>

typedef QScopedPointer <QTcpSocket> socket_ptr;

class Client : public QObject
{
	Q_OBJECT
public:

	Client();

	~Client();

	//! Sets local parameters
	void setParameters(QString img_path, QString text, qint32 reconnect_time = 5000);

	//! Sets output folder to save file
	inline void setOutputFolder(QString folder_name) { folder_name_ = folder_name; }

	//! Connect to server
	void connectToHost(const QHostAddress &address, quint16 port);

	//! Check if connected to server
	inline bool isConnected() { return (socket_->state() == QTcpSocket::ConnectedState); }


private:

	//! QScopedPointer to socket object
	socket_ptr socket_;	

	//! Output folder name
	QString folder_name_;

	//! Input file information
	QFileInfo file_info_;	

	//! Text to be written on image
	QString text_;

	//! Server ip address
	QHostAddress address_;

	//! Server port
	quint16 port_;

	//! Socket message buffer
	QByteArray buffer_;

	//! Bytes left to write to buffer
	qint64 bytes_left = -1;

	//! Time to reconnect to server again
	qint32 reconnect_time_ = 5000;

	//! Send log to console
	void addLog(QString log_text);

private slots:
	
	//! Called when data received to socket
	void receivedData();

	//! Called when socket connected
	void sendImage();

	//! Called when we need to try and reconnect ot server
	void reconnect();

	//! Called when all data received from server
	void dataReceivedAll();

signals:
	//! Emitted when all data received
	void readDone();
};

