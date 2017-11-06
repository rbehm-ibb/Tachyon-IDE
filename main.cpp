// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 1/24/2017 by behm
// ******************************************************

#include "mainwindow.h"
#include "config.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setApplicationName("Tachyon-4th-IDE");
	app.setApplicationVersion("V1.6d");
	app.setOrganizationName("R.Behm");
	app.setWindowIcon(QIcon(":/logo/pics/useforthlogo-s.png"));
	app.quitOnLastWindowClosed();
	QString device;
	QString filename;
	uint baud;
	// put this into a block to not have it during program lifetime.
	{
		QCommandLineParser parser;
		parser.setApplicationDescription(app.applicationName());
		parser.addHelpOption();
		parser.addVersionOption();
		parser.addPositionalArgument("filename", "initial file to load");

		QCommandLineOption devOption(QStringList() << "d" << "device", "Serial device.", "port", Config::stringValue("hw/port", "prop.board"));
		parser.addOption(devOption);
		QCommandLineOption baudOption(QStringList() << "b" << "baudrate", "Serial baudrate.", "baud", Config::stringValue("hw/baud", "115200"));
		parser.addOption(baudOption);

		parser.process(app);
		device = parser.value(devOption);
		baud = parser.value(baudOption).toUInt();
		if (! parser.positionalArguments().isEmpty())
		{
			filename = parser.positionalArguments().first();
		}

		QString dsn(":/style/styles.css");
		QFile fcss(dsn);
		if (fcss.open(QFile::ReadOnly))
		{
			QTextStream scss(&fcss);
			app.setStyleSheet(scss.readAll());
		}
		else
		{
			qWarning() << Q_FUNC_INFO << fcss.fileName() << "not found";
		}
	}
	MainWindow mw(device, baud);
	if (! filename.isEmpty())
	{
		mw.loadFile(filename);
	}
	mw.show();
	int rc = app.exec();
//	qApp->closeAllWindows();
	return rc;
}

