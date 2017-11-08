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
/// \n save() and load() are to be used to stored and load Configuration widgets. Use the objectName() to create keys.
///
class Config : public QObject
{
	Q_OBJECT
public:
	/// _dt_, will take care of saving the settings
	~Config();
	/// \brief value get a value from the settings
	/// \param key the key where the setting is stored, can contains group/key
	/// \param defaultValue if there is no setting for key then use this value, store it also
	/// \return the value as a QVariant
	static QVariant value(const QString &key, const QVariant defaultValue = QVariant());
	/// \brief stringValue get a value converted to a QString
	/// \param key  the key where the setting is stored, can contains group/key
	/// \param defaultValue if there is no setting for key then use this value, store it also
	/// \return  the value as a QString
	static QString stringValue(const QString &key, const QString &defaultValue = QString::null);
	/// \brief stringValue get a value converted to a QString, this saves the creation of a QString for the caller
	/// \param key the key where the setting is stored, can contains group/key
	/// \param defaultValue if there is no setting for key then use this value, store it also
	/// \return  the value as a QString
	static QString stringValue(const char *key, const QString &defaultValue = QString::null);
	/// \brief intValue get a value converted to a int
	/// \param key  the key where the setting is stored, can contains group/key
	/// \param defaultValue if there is no setting for key then use this value, store it also, INT_MIN is handled as no default given
	/// \return   the value as a int
	static int intValue(const QString &key, int defaultValue = INT_MIN);
	/// \brief intValue intValue get a value converted to a int, this saves the creation of a QString for the caller
	/// \param key  the key where the setting is stored, can contains group/key.
	/// \param defaultValue if there is no setting for key then use this value, store it also, INT_MIN is handled as no default given
	/// \return   the value as a int
	static int intValue(const char *key, int defaultValue = INT_MIN);

	/// \brief setValue sets a value in the setting, wfrites only if value is changed
	/// \n The function exists only to prevent the overlayed .. int value to be used if value is convertible to int, e.g. double
	/// \param key  the key where the setting is stored, can contains group/key.
	/// \param value The value to write,
	static void setValue(const QString &key, const QVariant value);
	/// \brief setValue sets a value in the setting, writes only if value is changed
	/// \param key  the key where the setting is stored, can contains group/key.
	/// \param value The value to write,
	static void setValue(const QString &key, const QString &value);
	/// \brief setValue sets a value in the setting, writes only if value is changed
	/// \param key  the key where the setting is stored, can contains group/key.
	/// \param value The value to write,
	static void setValue(const char *key, const QString &value);
	/// \brief setValue sets a value in the setting, writes only if value is changed
	/// \param key  the key where the setting is stored, can contains group/key.
	/// \param value The value to write,
	static void setValue(const QString &key, const int value);
	/// \brief setValue sets a value in the setting, writes only if value is changed
	/// \param key  the key where the setting is stored, can contains group/key.
	/// \param value The value to write,
	static void setValue(const char *key, const int value);

	/// unrelated, retreives the system user from the evironment.
	static QString sysUser();
	/// access to the underlying QSattings.
	static QSettings *conf() { return i()->m_conf; }

	/// save all fields (subwidgets) of a QWidget
	/// objectName() is used as the key
	static void save(QWidget *conf);	///< not impl if no gui
	/// loads all fields (subwidgets) of a QWidget
	/// objectName() is used as the key
	static void load(QWidget *conf);	///< not impl if no gui
signals:
public slots:
private slots:
protected:
private:
	QSettings *m_conf;			///< the manages the settings.
	/// \brief i get (and create) and instance.
	/// It creates a QSettings with qApp as parent in INI format in the qApp->applicationDirPath(),
	/// using the qApp->applicationName()
	/// \return the created instance
	static Config *i();
	static Config *instance;		///< the singleton instance
	explicit Config(QObject *parent = 0);	///< non accessible _ct_
};

#endif // CONFIG_H
