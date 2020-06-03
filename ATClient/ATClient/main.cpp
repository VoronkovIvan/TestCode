#include <QtCore/QCoreApplication>
#include "client.h"
#include <QHostAddress>
#include <QSettings>
#include <QTimer>

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	QSettings settings("params.ini", QSettings::IniFormat);

	Client *client = new Client();

	if (QCoreApplication::arguments().count() < 3)
	{
		qInfo() << "Not enough arguments";
	}
	else {
		client->setParameters(QCoreApplication::arguments().at(1), QCoreApplication::arguments().at(2), settings.value("timeout").toInt());
		client->setOutputFolder(settings.value("output folder").toString());

		client->connectToHost(QHostAddress::QHostAddress(settings.value("ip").toString()), settings.value("port").toInt());
	}



	return a.exec();
}
