// ******************************************************
// * copyright (C) 2016 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 8/12/2016 by behm
// ******************************************************

#include "filenamehandler.h"

FilenameHandler::FilenameHandler(QString ext, QObject *parent)
	: QObject(parent)
{
	setExt(ext);
}

FilenameHandler::FilenameHandler(QString fn, QString ext, QObject *parent)
	: QObject(parent)
{
	setExt(ext);
	setFilename(fn);
}

QString FilenameHandler::fullname() const
{
	return m_fi.absoluteFilePath();
}

QString FilenameHandler::ext() const
{
	return m_ext;
}

void FilenameHandler::setExt(const QString &ext)
{
	m_ext = ext;
	if (! m_ext.startsWith('.'))
		m_ext.prepend('.');
}

QFileInfo FilenameHandler::fi() const
{
	return m_fi;
}

void FilenameHandler::setFi(const QFileInfo &fi)
{
	m_fi = fi;
}

void FilenameHandler::setFilename(QString fn)
{
	const QString oldFn = m_fi.absoluteFilePath();
	m_fi.setFile(fn);
	m_fi.setFile(m_fi.absoluteDir().absoluteFilePath(m_fi.baseName() + m_ext));
	if (oldFn != m_fi.absoluteFilePath())
		emit changed(fullname(), m_fi.baseName());
}



