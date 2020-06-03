#include <QString>
#include <QImage>
#include <QBuffer>
#include <QDataStream>

//Data looks like  (qint64)datasize + 55 + IMGDATA + 66 + TEXT
//                 

class Packer
{
public:
	Packer() {};
	~Packer() {};

	static QByteArray pack(const QImage &img) {


		QByteArray data;
		QDataStream out(&data, QIODevice::WriteOnly);
		out << (qint64)0 << (qint8)55 << img;

		qint64 data_len = data.size() - sizeof(qint64);

		out.device()->seek(0);
		out << data_len;

		return data;
	}

	static QByteArray pack(const QImage &img, QString text)
	{
		QByteArray data;
		QDataStream out(&data, QIODevice::WriteOnly);
		out << (qint64)0 << (qint8)55 << img << (qint8)66 << text;;

		qint64 data_len = data.size() - sizeof(qint64);

		out.device()->seek(0);
		out << data_len;

		return data;
	}

	static void unpack(const QByteArray &data, QImage &img, QString &text)
	{
		QDataStream in(data);

		qint8 prefix;
		qint64 data_len;

		in >> prefix;

		if (prefix == 55)
		{
			in >> img;

			if (!in.atEnd())
			{
				in >> prefix;
				if (prefix == 66)
				{
					in >> text;
				}
			}
		}
		else return;
	}

	static void unpack(const QByteArray &data, QImage &img)
	{
		QDataStream in(data);

		qint8 prefix;
		qint64 data_len;

		in >> prefix;

		if (prefix == 55)
		{
			in >> img;
		}
		else return;
	}


};

