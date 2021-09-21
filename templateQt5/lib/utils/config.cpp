// ******************************************************
// * copyright (C) 2016 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 1/8/2017 by behm
// ******************************************************

#include "config.h"

Config *Config::instance = nullptr;

Config::Config(QObject *parent)
	: QObject(parent)
{
//	m_conf = new QSettings(QSettings::UserScope, qApp->organizationName(), qApp->applicationName(), this);
	m_conf = new QSettings(qApp->applicationDirPath() + "/" + qApp->applicationName() + ".rc", QSettings::IniFormat, this);
	instance = this;
//	qDebug() << Q_FUNC_INFO << m_conf->fileName();
}

Config::~Config()
{
//	qDebug() << Q_FUNC_INFO << m_conf->fileName();
	m_conf->sync();
}

bool Config::loadDefaults(const QString &cfgName)
{
	return i()->loadDefaultsInternal(cfgName);
}

bool Config::loadDefaultsInternal(const QString &cfgName)
{
//	qDebug() << Q_FUNC_INFO << cfgName;
	QString name(cfgName);
	if (name.isEmpty())
	{
		name = "defaults.rc";
	}
	QResource rsc(name);
	if (! rsc.isValid())
	{
		qWarning() << Q_FUNC_INFO << rsc.absoluteFilePath() << "not found";
		return false;
	}
//	qDebug() << Q_FUNC_INFO << rsc.absoluteFilePath() << rsc.isValid();
	QSettings defs(rsc.absoluteFilePath(), QSettings::IniFormat);
	if (defs.status() != QSettings::NoError)
	{
		qWarning() << Q_FUNC_INFO << defs.fileName() << "error:" << defs.status();
		return false;
	}
//	qDebug() << Q_FUNC_INFO << defs.allKeys() << defs.fileName() << defs.status();
	foreach (const QString &k, defs.allKeys())
	{
		if (! m_conf->contains(k))
		{
			m_conf->setValue(k, defs.value(k));
		}
	}
	if (m_conf->value("defaults/date").isNull())
	{
		m_conf->setValue("defaults/date", QDateTime::currentDateTime().toString(Qt::RFC2822Date));
//		qDebug() << Q_FUNC_INFO << cfgName;
	}
	m_conf->setValue("defaults/name", qApp->applicationName());
	m_conf->setValue("defaults/version", qApp->applicationVersion());
//	m_conf->sync();
	return true;
}

QVariant Config::value(const QString &key, const QVariant defaultValue)
{
	if (! i()->m_conf->contains(key) && defaultValue.isValid())
	{
		instance->m_conf->setValue(key, defaultValue);
	}
	return instance->m_conf->value(key, defaultValue);
}

QString Config::stringValue(const QString &key, const QString &defaultValue)
{
	if (! i()->m_conf->contains(key) && ! defaultValue.isEmpty())
	{
		instance->m_conf->setValue(key, defaultValue);
	}
	return instance->m_conf->value(key, defaultValue).toString();
}

QString Config::stringValue(const char *k, const QString &defaultValue)
{
	return stringValue(QString::fromLatin1(k), defaultValue);
}

int Config::intValue(const QString &key, int defaultValue)
{
	if (! i()->m_conf->contains(key) && defaultValue != INT_MIN)
	{
		instance->m_conf->setValue(key, defaultValue);
	}
	return instance->m_conf->value(key, defaultValue).toString().toInt(nullptr, 0);
}

int Config::intValue(const char *key, int defaultValue)
{
	return intValue(QString::fromLatin1(key), defaultValue);
}

int Config::boolValue(const char *key)
{
	return instance->m_conf->value(key).toBool();
}

qreal Config::realValue(const QString &key, qreal defaultValue)
{
	if (! i()->m_conf->contains(key) && ! qIsNaN(defaultValue))
	{
		instance->m_conf->setValue(key, defaultValue);
	}
	return instance->m_conf->value(key, defaultValue).toDouble();
}

qreal Config::realValue(const char *key, qreal defaultValue)
{
	return realValue(QString::fromLatin1(key), defaultValue);
}

void Config::setValue(const QString &key, const QVariant value)
{
	if (! i()->conf()->contains(key) || (i()->conf()->value(key) != value))
	{
		i()->m_conf->setValue(key, value);
	}
}

void Config::setValue(const char *key, const QString &value)
{
	setValue(QString::fromLatin1(key), value);
}

void Config::setValue(const char *key, const char *value)
{
	setValue(QString::fromLatin1(key), QString::fromLatin1(value));

}

void Config::setValue(const QString &key, const QString &value)
{
	if (! i()->conf()->contains(key) || (i()->conf()->value(key).toString() != value))
	{
		i()->m_conf->setValue(key, value);
	}
}

void Config::setValue(const QString &key, const int value)
{
	if (! i()->conf()->contains(key) || (i()->conf()->value(key).toInt() != value))
	{
		i()->m_conf->setValue(key, value);
	}
}

void Config::setValue(const char *key, const int value)
{
	setValue(QString::fromLatin1(key), value);
}

void Config::setValue(const QString &key, const qreal value)
{
	if (! i()->conf()->contains(key) || (i()->conf()->value(key).toDouble() != value))
	{
		i()->m_conf->setValue(key, value);
	}
}

void Config::setValue(const char *key, const qreal value)
{
	setValue(QString::fromLatin1(key), value);
}

void Config::beginGroup(const char *grp)
{
	i()->conf()->beginGroup(QString(grp));
}

void Config::endGroup()
{
	i()->conf()->endGroup();
}

QString Config::sysUser()
{
	QByteArray user = qgetenv("USER");
	return QString::fromLocal8Bit(user);
}

Config *Config::i()
{
	if (! instance)
	{
		instance = new Config(qApp);
//		setValue("app/name", qApp->applicationName());
//		setValue("app/version", qApp->applicationVersion());
	}
	return instance;
}
