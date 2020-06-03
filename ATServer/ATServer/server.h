#include <QtCore/QCoreApplication>
#include <QTcpServer>
#include <QScopedPointer>
#include <QTcpSocket>



class Server : public QTcpServer
{
	Q_OBJECT
public:


	Server();

	~Server();


	//! Start server
	//! Return true if started, false if not
	bool start();

	//! Stop server listening
	void stop();

	//! Check if listening or not
	inline bool isStarted() { return started_; }

	//! Set aprameters
	inline void setParams(qint16 port_n, qint32 max_threads) { port_ = port_n;	max_processing_ = max_threads; }
protected:

	//! Method, indicating that we have new connection atm and we decide
	//! If we rdy to work with new task or not
	virtual void incomingConnection(qintptr socket_descriptor);

private:

	//! Currently processing tasks
	qint16 processing_ = 0;

	//! Maximum allowed tasks
	qint16 max_processing_ = 1;

	//! Server port
	qint16 port_ = 1234;

	//! Counter to give id's to tasks
	qint32 task_counter_ = 0;

	//! Is server started flag
	bool started_ = false;
private slots:

	//! Called when ImgTask gives us client IP
	void receiveIpFromSocket(QString);

signals:
	//! Emitted when we start new task
	void taskStarted(qint32 task_id, QString ip);

	//! Emitted when task is done
	void taskClosed(qint32 task_id);


};

