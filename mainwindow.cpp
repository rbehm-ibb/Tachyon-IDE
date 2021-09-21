// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 1/24/2017 by behm
// ******************************************************

#include "mainwindow.h"
#include "config.h"
#include "toolbarspacer.h"
#include "interfaceconfigdialog.h"
#include "finddialog.h"
#include "wordsmodel.h"
#include "wordsview.h"
#include "prop.h"
#include "console.h"
#include "helpview.h"
#include "helpmodel.h"

MainWindow::MainWindow(const QString device, uint baud, QWidget *parent)
	: QMainWindow(parent)
	, m_changed(false)
	, m_wordsModel(new WordsModel(this))
	, m_prop(new Prop(device, baud, this))
	, m_console(new Console)
{
	setWindowTitle(qApp->applicationName() + " " + qApp->applicationVersion());
//	setAttribute(Qt::WA_DeleteOnClose);
	auto fileMenu = menuBar()->addMenu(tr("&File"));
	auto editMenu = menuBar()->addMenu(tr("&Edit"));
	auto propMenu = menuBar()->addMenu(tr("&Prop"));
	auto infoMenu = menuBar()->addMenu(tr("&Info"));
	QToolBar *toolbar = addToolBar("Toolbar");
	QAction *actQuit = toolbar->addAction(QIcon(":/exit"), tr("Quit"));
	connect(actQuit, &QAction::triggered, this, &MainWindow::quit);
	actQuit->setShortcut(QKeySequence::Quit);
//	m_console->addAction(actQuit);
	toolbar->addSeparator();
	QAction *act;
	act = toolbar->addAction(QIcon("://filenew"), tr("New"));
	fileMenu->addAction(act);
	connect(act, &QAction::triggered, this, &MainWindow::newDoc);
	act = toolbar->addAction(QIcon(":/fileopen"), tr("Open"));
	act->setShortcut(QKeySequence::Open);
	fileMenu->addAction(act);
	connect(act, &QAction::triggered, this, &MainWindow::open);
	act = toolbar->addAction(QIcon(":/doc-save"), tr("Save"));
	act->setShortcut(QKeySequence::Save);
	fileMenu->addAction(act);
	connect(act, &QAction::triggered, this, &MainWindow::save);
	act = toolbar->addAction(QIcon("://doc-save-as"), tr("Save-As"));
	act->setShortcut(QKeySequence::SaveAs);
	fileMenu->addAction(act);
	connect(act, &QAction::triggered, this, &MainWindow::saveAs);
	prevMenu = fileMenu->addMenu("Previous");
	readPrev();
	fileMenu->addSeparator();
	fileMenu->addAction(actQuit);
	toolbar->addSeparator();
	act = toolbar->addAction(QIcon("://rotate-ccw"), tr("ReScan"));
//	act->setShortcut(QKeySequence("Ctrl+R"));
	editMenu->addAction(act);
	connect(act, &QAction::triggered, this, &MainWindow::rescan);
	act = toolbar->addAction(QIcon(":/icons/pics/date.png"), tr("Insert Date"));
	connect(act, &QAction::triggered, this, &MainWindow::enterDate);
	editMenu->addAction(act);
	act = toolbar->addAction(QIcon(":/icons/pics/text_uppercase.png"), tr("Upper case"));
	act->setShortcut(QKeySequence("Alt+U"));
	connect(act, &QAction::triggered, this, &MainWindow::toUpper);
	editMenu->addAction(act);
	act = toolbar->addAction(QIcon(":/icons/pics/find.png"), tr("Find"));
	act->setShortcut(QKeySequence::Find);
	editMenu->addAction(act);
	connect(act, &QAction::triggered, this, &MainWindow::find);
	toolbar->addSeparator();
	act = editMenu->addAction(tr("Save_Words"));
//	editMenu->addAction(act);
	connect(act, &QAction::triggered, this, &MainWindow::saveWords);
	toolbar->addSeparator();
	act = toolbar->addAction(QIcon(":/config"), tr("Config"));
	connect(act, &QAction::triggered, this, &MainWindow::config);
	act = toolbar->addAction(QIcon(":/icons/pics/project-burn.png"), tr("Upload"));
	act->setShortcut(QKeySequence("Ctrl+U"));
	propMenu->addAction(act);
	connect(act, &QAction::triggered, this, &MainWindow::upload);
	act = toolbar->addAction(QIcon(":/icons/pics/launch.svgz"), tr("Reset"));
	act->setShortcut(QKeySequence("Ctrl+R"));
	propMenu->addAction(act);
	connect(act, &QAction::triggered, m_prop, &Prop::resetProp);
	act = toolbar->addAction(QIcon("://info"), tr("Show Docs"));
	infoMenu->addAction(act);
	connect(act, &QAction::triggered, this, &MainWindow::showDocs);

	ToolBarSpacer::addSpacer(toolbar);
	act = toolbar->addAction(QIcon(":/logo/pics/useforthlogo-s.png"), tr("About"));
	infoMenu->addAction(act);
	connect(act, &QAction::triggered, this, &MainWindow::about);
	statusBar()->addPermanentWidget(m_lFile = new QLabel);

	m_editor = new QPlainTextEdit;
	setCentralWidget(m_editor);

	m_editor->setMinimumSize(640, 480);
	m_editor->setFont(QFont("Monospace", 10/*, QFont::Bold*/));
	m_editor->setLineWrapMode(QPlainTextEdit::NoWrap);
	m_editor->setCenterOnScroll(true);


	statusBar()->addPermanentWidget(m_lPort = new QLabel);
	QLabel *logolabel = new QLabel;
	QPixmap logo2(":/logo/ibb-logo");
	logolabel->setPixmap(logo2.scaled(32, 15));
	statusBar()->addPermanentWidget(logolabel);
	m_console->show();
	connect(m_prop, &Prop::byteRxd, m_console.data(), &Console::charRxd);
	connect(m_console.data(), &Console::sendSerial, m_prop, &Prop::send);

	m_wordsDock = new QDockWidget(tr("Words"), this);
	m_wordsDock->setFeatures(QDockWidget::DockWidgetMovable);
	m_wordsDock->setAllowedAreas(Qt::LeftDockWidgetArea);
	addDockWidget(Qt::LeftDockWidgetArea, m_wordsDock);
	m_wordsDock->setWidget(m_wordsView = new WordsView(m_wordsModel));
	m_wordsView->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_wordsView->setSelectionMode(QAbstractItemView::SingleSelection);
	connect(m_wordsView, &WordsView::currentMoved, this, &MainWindow::currentMoved);

	m_helpDock = new QDockWidget(tr("Help"), this);
	m_helpDock->setFeatures(QDockWidget::DockWidgetMovable);
	m_helpDock->setAllowedAreas(Qt::LeftDockWidgetArea);
	addDockWidget(Qt::LeftDockWidgetArea, m_helpDock);
	HelpModel *help = new HelpModel(this);
	help->load();
	m_helpDock->setWidget(m_helpView = new HelpView(help));


	m_findDock = new QDockWidget(tr("Find"), this);
	m_findDock->setFeatures(QDockWidget::DockWidgetMovable);
	m_findDock->setAllowedAreas(Qt::BottomDockWidgetArea);
	addDockWidget(Qt::BottomDockWidgetArea, m_findDock);
	m_findDock->setWidget(m_findDialog = new FindDialog);
	connect(m_findDialog, &FindDialog::find, this, &MainWindow::findInSource);

	connect(m_editor, &QPlainTextEdit::textChanged, this, &MainWindow::textChanged);
	connect(m_prop, &Prop::deviceChanged, this, &MainWindow::deviceChanged);
	deviceChanged(m_prop->device());
}


MainWindow::~MainWindow()
{
	delete m_console;
}

void MainWindow::quit()
{
	close();
}

void MainWindow::about()
{
//	QIcon save = windowIcon();
	char year[] = "2017-2021";
	QString text("<h1>%1</h1>"
		     "<p>Version %2"
		     "<p>&copy; %3, <img src=\":/logo/ibb-logo\">"
		     "<p>Web: <a href=\"https://github.com/rbehm-ibb/Tachyon-IDE\">https://github.com/rbehm-ibb/Tachyon-IDE</a>"
		     "<p>Mail: <a href=\"mailto:rbehm@hushmail.com\">rbehm@hushmail.com</a>"
		     "<p>Using  <img src=\":/stdicons/qt-logo-about.png\"> %5"
		     "<p> Tachyon and Tachyon logo (c) <a href=\"https://sourceforge.net/u/pbjtech/profile/\">Peter Jakicki</a>"
		     "<p>Connected at %6"
		     );
	text = text
		.arg(qApp->applicationName(), qApp->applicationVersion())
		.arg(year, qVersion(), m_prop->device())
		;
	QMessageBox::about(this, qApp->applicationName(), text);
}

void MainWindow::open()
{
	if (m_changed)
	{
		int rc = QMessageBox::warning(this, "Not saved",
					      QString("The file\n%1\nhas not been saved\n\nLoad anyway?")
					      .arg(m_lFile->text()),
					      QMessageBox::Yes | QMessageBox::No);
		switch (rc)
		{
		case QMessageBox::No:
			return;
		default:
			break;
		}
	}
	QString fn = QFileDialog::getOpenFileName(this, "Load File",
						  Config::stringValue("file/name", qApp->applicationDirPath()), "*.fth");
	if (! fn.isEmpty())
	{
		loadFile(fn);
	}
}

void MainWindow::save()
{
	QString fn = m_lFile->text();
	if (fn.isEmpty())
	{
		saveAs();
	}
	else
	{
		saveFile(fn);
	}
}

void MainWindow::saveAs()
{
	QString fn = QFileDialog::getSaveFileName(this, "Save File",
						  Config::stringValue("file/name", qApp->applicationDirPath()),
						  "*.fth");
	if (! fn.isEmpty())
	{
		saveFile(fn);
	}
}

void MainWindow::newDoc()
{
	if (m_changed)
	{
		int rc = QMessageBox::warning(this, "Not saved",
					      QString("The file\n%1\nhas not been saved\n\nClear anyway?").arg(m_lFile->text()),
					      QMessageBox::Yes | QMessageBox::No);
		switch (rc)
		{
		case QMessageBox::No:
			return;
		default:
			break;
		}
	}
	m_editor->clear();
	m_lFile->setText("");
	m_changed = false;
}

void MainWindow::loadFile(const QString fileName)
{
	QFile f(fileName);
	if (! f.open(QIODevice::ReadOnly))
	{
		qWarning() << Q_FUNC_INFO << f.fileName() << f.errorString();
		QMessageBox::critical(this, "Problem loading File",
					      QString("The file\n\n%1\n\ncannot be opened.\n\n%2")
				      .arg(f.fileName())
				      .arg(f.errorString()),
				      QMessageBox::Yes);
		return;
	}
	addPrev(f.fileName());
	QTextStream s(&f);
	QString text = s.readAll();
	m_editor->setPlainText(text);
	m_lFile->setText(f.fileName());
	Config::setValue("file/name", f.fileName());
	m_wordsModel->analyse(text);
	m_changed = false;
}

void MainWindow::saveFile(const QString fileName)
{
//	qWarning() << Q_FUNC_INFO << fileName;
	QFile f(fileName);
	if (! f.open(QIODevice::WriteOnly | QIODevice::Truncate))
	{
		qWarning() << Q_FUNC_INFO << f.fileName() << f.errorString();
		QMessageBox::critical(this, "Problem saving File",
					      QString("The file\n\n%1\n\ncannot be created.\n\n%2")
				      .arg(f.fileName())
				      .arg(f.errorString()),
				      QMessageBox::Yes);
		return;
	}
	QTextStream s(&f);
	s << m_editor->toPlainText();
	if (s.status() != QTextStream::Ok)
	{
		QMessageBox::critical(this, "Problem writing to File",
					      QString("The file\n\n%1\n\ncannot be saved.\n\n%2\n%3")
				      .arg(f.fileName())
				      .arg(f.errorString())
				      .arg(s.status()),
				      QMessageBox::Yes);
		return;
	}
	m_lFile->setText(f.fileName());
	Config::setValue("file/name", f.fileName());
	addPrev(f.fileName());
	m_changed = false;
}

void MainWindow::enterDate()
{
	QString dt = QDate::currentDate().toString(Qt::ISODate);
	m_editor->insertPlainText(dt);
}

void MainWindow::toUpper()
{
	QString sel = m_editor->textCursor().selectedText();
	m_editor->textCursor().removeSelectedText();
	m_editor->textCursor().insertText(sel.toUpper());
}

void MainWindow::showDocs()
{
	QString fn = QFileDialog::getOpenFileName(this, "Open File", qApp->applicationDirPath() + "/docs", "*.pdf");
	if (! fn.isEmpty())
	{
		showDoc(fn);
	}
}

void MainWindow::showDoc(const QString fn)
{
	QDesktopServices::openUrl(QUrl::fromLocalFile(fn));
}

void MainWindow::textChanged()
{
//	qDebug() << Q_FUNC_INFO << m_changed;
	m_changed = true;
}

void MainWindow::deviceChanged(const QString deviceBaud) const
{
	m_lPort->setText(deviceBaud);
}

void MainWindow::loadPrev()
{
	QAction *act = qobject_cast<QAction*>(sender());
	qDebug() << Q_FUNC_INFO << sender() << act->text();
	QFile f(act->text());
	if (! f.open(QIODevice::ReadOnly))
	{
		qWarning() << Q_FUNC_INFO << f.fileName() << f.errorString();
		return;
	}
	QTextStream s(&f);
	QString text = s.readAll();
	m_editor->setPlainText(text);
	m_lFile->setText(f.fileName());
	Config::setValue("file/name", f.fileName());
	m_wordsModel->analyse(text);
	m_changed = false;

}

void MainWindow::saveWords()
{
	m_wordsModel->save();
}

void MainWindow::rescan()
{
	m_wordsModel->analyse(m_editor->toPlainText());
}

void MainWindow::config()
{
	if (! m_config)
	{
		m_config = new InterfaceConfigDialog(this);
	}
	m_config->setProp(m_prop);
	m_config->exec();
}

void MainWindow::upload()
{
	const QByteArray d = m_editor->toPlainText().toLocal8Bit();
	m_prop->upload(d);
}

void MainWindow::find()
{
	m_findDock->show();
	m_findDialog->focusIn();
}

void MainWindow::findNext()
{
	m_findDialog->findSlot();
}

void MainWindow::findInSource(const QString exp, QTextDocument::FindFlags options)
{
	m_editor->find(exp, options);
}

void MainWindow::goLine(int lineno)
{
//	qDebug() << Q_FUNC_INFO << lineno;
	m_editor->moveCursor(QTextCursor::Start);
	for (int i = 0; i < lineno; ++i)
		m_editor->moveCursor(QTextCursor::NextBlock);
	m_editor->ensureCursorVisible();
	m_editor->setFocus();
}

void MainWindow::currentMoved(const QModelIndex &current)
{
	int lineno = current.sibling(current.row(), WordsModel::Lineno).data().toInt();
//	qDebug() << Q_FUNC_INFO << current << lineno;
	goLine(lineno);
	QString word = current.sibling(current.row(), WordsModel::Word).data().toString();
	m_findDialog->setFindString(word);
	m_findDialog->findSlot();
//	editor->find(word, 0);
}

void MainWindow::portName(const QString s)
{
	m_lPort->setText(s);
}


void MainWindow::closeEvent(QCloseEvent *event)
{
//	qDebug() << Q_FUNC_INFO << m_changed;
	if (m_changed)
	{
		int rc = QMessageBox::warning(this, "Not saved", QString("The file\n%1\nhas not been saved\n\nQuit anyway?").arg(m_lFile->text()),
					      QMessageBox::Yes | QMessageBox::No);
		switch (rc)
		{
		case QMessageBox::Yes:
		case QMessageBox::Ok:
			event->accept();
			break;
		default:
			event->ignore();
		}
	}
	else
	{
		event->accept();
	}
	if (event->isAccepted())
	{
		if (m_console)
		{
			m_console->closeIt();
		}
		qApp->closeAllWindows();
	}
}

void MainWindow::addPrev(const QString fn)
{
	QStringList prev = readPrev();
	if (! prev.contains(fn))
	{
		prev.prepend(fn);
	}
	while (prev.count() > 10)
	{
		prev.removeLast();
	}
	Config::conf()->beginWriteArray("file/previous", prev.count());
	for (int i = 0; i < prev.count(); ++i)
	{
		Config::conf()->setArrayIndex(i);
		Config::setValue("prev", prev.at(i));
	}
	Config::conf()->endArray();
	readPrev();	// update menu
}

const QStringList MainWindow::readPrev() const
{
	QStringList prev;
	int n = Config::conf()->beginReadArray("file/previous");
	for (int i = 0; i < n; ++i)
	{
		Config::conf()->setArrayIndex(i);
		const QString fn = Config::stringValue("prev");
		if (! fn.isEmpty())
		{
			prev << fn;
		}
	}
	Config::conf()->endArray();
	prevMenu->clear();
	foreach (const QString &s, prev)
	{
		QAction *act = prevMenu->addAction(QIcon(":/fileopen"), s);
		connect(act, &QAction::triggered, this, &MainWindow::loadPrev);
	}
	return prev;
}
