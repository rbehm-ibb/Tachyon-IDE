// ******************************************************
// * copyright (C) 2016 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 1/8/2017 by behm
// ******************************************************

#include "config.h"

void Config::load(QWidget *conf)
{
	i();
	foreach (QWidget * w, conf->findChildren<QWidget*>())
	{
		QString name = w->objectName();
		if (name.startsWith("qt_"))	// Qt helper widgets
		{
			continue;
		}
		if (w->inherits("QLineEditIconButton") || w->inherits("QGroupBox"))
		{
			continue;
		}
		name.prepend(conf->objectName() + "/");
//		qDebug() << Q_FUNC_INFO << w << name << Config::stringValue(name);
		QLineEdit *le = qobject_cast<QLineEdit*>(w);
		if (le)
		{
			le->setText(Config::stringValue(name));
			continue;
		}
		QAbstractButton *rb = qobject_cast<QAbstractButton*>(w);
		if (rb)
		{
			if (rb->isCheckable())
			{
				rb->setChecked(Config::value(name).toBool());
			}
			continue;
		}
		QComboBox *cb = qobject_cast<QComboBox*>(w);
		if (cb)
		{
			cb->setCurrentText(Config::stringValue(name));
			continue;
		}
		QSpinBox *sb = qobject_cast<QSpinBox*>(w);
		if (sb)
		{
			sb->setValue(Config::value(name).toInt());
			continue;
		}
		QDoubleSpinBox *dsb = qobject_cast<QDoubleSpinBox*>(w);
		if (dsb)
		{
			dsb->setValue(Config::value(name).toDouble());
			continue;
		}
	}
}

void Config::save(QWidget *conf)
{
	i();
	foreach (QWidget * w, conf->findChildren<QWidget*>())
	{
		QString name = w->objectName();
		if (name.startsWith("qt_"))	// Qt helper widgets
		{
			continue;
		}
		if (w->inherits("QLineEditIconButton") || w->inherits("QGroupBox"))
		{
			continue;
		}
		name.prepend(conf->objectName() + "/");
//		qDebug() << Q_FUNC_INFO << w << name;
		QLineEdit *le = qobject_cast<QLineEdit*>(w);
		if (le)
		{
			Config::setValue(name, le->text());
			continue;
		}
		QAbstractButton *rb = qobject_cast<QAbstractButton*>(w);
		if (rb)
		{
			if (rb->isCheckable())
			{
				Config::setValue(name, rb->isChecked());
			}
			continue;
		}
		QComboBox *cb = qobject_cast<QComboBox*>(w);
		if (cb)
		{
			Config::setValue(name, cb->currentText());
			continue;
		}
		QSpinBox *sb = qobject_cast<QSpinBox*>(w);
		if (sb)
		{
			Config::setValue(name, sb->value());
			continue;
		}
		QDoubleSpinBox *dsb = qobject_cast<QDoubleSpinBox*>(w);
		if (dsb)
		{
			Config::setValue(name, dsb->value());
			continue;
		}
	}
}
