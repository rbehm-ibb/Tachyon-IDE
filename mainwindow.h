// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 1/24/2017 by behm
// ******************************************************

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class Prop;
class Console;
class FindDialog;
class WordsModel;
class WordsView;
class HelpView;
class InterfaceConfigDialog;

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit MainWindow(const QString device, uint baud, QWidget *parent = 0);
	~MainWindow();

signals:

public slots:
	void loadFile(const QString fileName);
private slots:
	void portName(const QString s);
	void about();
	void quit();
	void open();
	void save();
	void saveAs();
	void newDoc();
	void saveFile(const QString fileName);
	void enterDate();
	void toUpper();
	void showDocs();
	void saveWords();
	void rescan();
	void config();
	void upload();
	void find();
	void findNext();
	void findInSource(const QString exp, QTextDocument::FindFlags options);
	void goLine(int lineno);
	void currentMoved(const QModelIndex &current);
	void showDoc(const QString fn);
	void textChanged();
	void deviceChanged(const QString deviceBaud) const;
	void loadPrev();
protected:
	virtual void closeEvent(QCloseEvent *);
private:
	void addPrev(const QString fn);
	const QStringList readPrev() const;
	QLabel *m_lFile;
	QLabel *m_lPort;
	bool m_changed;
	QPlainTextEdit *m_editor;
	FindDialog *m_findDialog;
	QDockWidget *m_findDock;
	WordsModel *m_wordsModel;
	WordsView *m_wordsView;
	QDockWidget *m_wordsDock;
	HelpView *m_helpView;
	QDockWidget *m_helpDock;
	Prop *m_prop;
	QPointer<Console> m_console;
	QPointer<InterfaceConfigDialog> m_config;
	QMenu *prevMenu;
};

#endif // MAINWINDOW_H
