#include <QThread>
#include <QTcpSocket>
#include <QString>
#include <QImage>
#include <QRunnable>


#define ANSWER_BAD 99999
#define ANSWER_GOOD 88888

typedef QScopedPointer <QTcpSocket> socket_ptr;

class ImgTask : public QObject, public QRunnable
{
	Q_OBJECT
public:
	ImgTask(qint32 socket_descriptor, qint32 task_id);

	~ImgTask();

	//! Get tas unique id
	inline qint32 getId() { return task_id_; }

public slots:

	//! Run thread
	void run() override;

	//! Called when new data received by socket_
	void readSocket();

	//! Called when all data received and we can start processing image
	void putTextOnImage();

private:

	//! Socket id to init socket object
	qint32 socket_descriptor_;

	//! Socket pointer
	socket_ptr socket_;

	//! Text to be written on image
	QString text_;

	//! Image itself
	QImage image_;

	//! Buffer to be filled with data from socket
	QByteArray buffer_;

	//! Bytes left to read from socket
	qint64 bytes_left = -1;

	//! Unique task id
	qint32 task_id_;

	//! Send data to client
	void sendData();


signals:
	//! Emitted when all data received from socket
	void readDone();

	//! Emitted when work with picture is finished and new image sent to client
	void finished(qint32);

	//! Emitted when connected to socket and we know it's ip address
	void gotIp(QString);
};

