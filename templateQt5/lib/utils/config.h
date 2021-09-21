// ******************************************************
// * copyright (C) 2016 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 1/8/2017 by behm
// ******************************************************

#ifndef CONFIG_H
#define CONFIG_H

///
/// \brief The Config class is a general configuration storage using QSettings internally.
///
/// It is implemented as a singleton to be usable throughout the whole application.
/// \n All accesses work via static functions.
/// \n The setValue() only change the underlying QSettings when there is a real change.
/// \n The ...value() function add the default value to the underlying QSettings if there is currently nothing stored for this key.
/// \n save() and load() are to be used to store and load Configuration widgets. They use the objectName() to create keys.
///
class Config : public QObject
{
	Q_OBJECT
public:
	~Config();
	static bool loadDefaults(const QString &cfgName = QString());
	static QVariant value(const QString &key, const QVariant defaultValue = QVariant());
	static QString stringValue(const QString &key, const QString &defaultValue = QString());
	static QString stringValue(const char *key, const QString &defaultValue = QString());
	static int intValue(const QString &key, int defaultValue = INT_MIN);
	static int intValue(const char *key, int defaultValue = INT_MIN);
	static int boolValue(const char *key);
	static qreal realValue(const QString &key, qreal defaultValue = qQNaN());
	static qreal realValue(const char *key, qreal defaultValue = qQNaN());

	static void setValue(const QString &key, const QVariant value);
	// next is needed to prevent getting triggered with int value.
	static void setValue(const char *key, const QByteArray d) { setValue(key, QVariant(d)); }
	static void setValue(const QString &key, const QString &value);
	static void setValue(const char *key, const QString &value);
	static void setValue(const char *key, const char *value);
	static void setValue(const QString &key, const int value);
	static void setValue(const QString key, const uint value) { setValue(key, QVariant(value)); }
	static void setValue(const char *key, const uint value) { setValue(key, QVariant(value)); }
	static void setValue(const char *key, const int value);
	static void setValue(const QString &key, const qreal value);
	static void setValue(const char *key, const qreal value);

	static void beginGroup(const char *grp);
	static void endGroup();

	static QString sysUser();
	static QSettings *conf() { return i()->m_conf; }

	static void save(QWidget *conf);	///< not impl if no gui
	static void load(QWidget *conf);	///< not impl if no gui
signals:

public slots:
private slots:
protected:
private:
	QSettings *m_conf;
	QString m_user;
	static Config *i();
	static Config *instance;
	explicit Config(QObject *parent = nullptr);
	bool loadDefaultsInternal(const QString &cfgName);
};

#endif // CONFIG_H
