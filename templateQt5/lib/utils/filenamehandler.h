// ******************************************************
// * copyright (C) 2016 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 8/12/2016 by behm
// ******************************************************

#ifndef FILENAMEHANDLER_H
#define FILENAMEHANDLER_H

class FilenameHandler : public QObject
{
	Q_OBJECT
public:
	explicit FilenameHandler(QString ext, QObject *parent = 0);
	explicit FilenameHandler(QString fn, QString ext, QObject *parent = 0);
	QString fullname() const;

	QString ext() const;
	void setExt(const QString &ext);

	QFileInfo fi() const;
	void setFi(const QFileInfo &fi);

signals:
	void changed(QString fullname, QString name);
public slots:
	void setFilename(QString fn);
private slots:
protected:
private:
	QString m_ext;
	QFileInfo m_fi;
};

#endif // FILENAMEHANDLER_H
