#include "QtGuiApplication1.h"
QString path;
QString SerName;
QtGuiApplication1::QtGuiApplication1(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(mySlot1()));
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(mySlot2()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(mySlot3()));
}
void QtGuiApplication1::mySlot1() {
    path = QFileDialog::getOpenFileName(this, "select driver", "/the path", "sys(*.sys)");
	ui.textBrowser->setText(path);
	
	int i, j;
	j = path.length() - 4;
	QChar c('/');
	for (i = j - 1; i >= 0; --i) {
		if (path.at(i) == c) {
			break;
		}
		else {
			SerName.insert(0,path.at(i));
		}
	}
	QMessageBox::critical(NULL, "title", SerName, QMessageBox::Yes, QMessageBox::Yes);
}
void QtGuiApplication1::mySlot2() {
	std::wstring wlpstr1 = SerName.toStdWString();
	LPCWSTR lpszDriverName = wlpstr1.c_str();
	std::wstring wlpstr2 = path.toStdWString();
	LPCWSTR lpszDriverPath = wlpstr2.c_str();
	bool B=LoadNTDriver(lpszDriverName, lpszDriverPath);
	if (B) {
		QMessageBox::information(NULL, "result", "success install", QMessageBox::Yes , QMessageBox::Yes);
	}
	else {
		QMessageBox::information(NULL, "result", "fail   install", QMessageBox::Yes, QMessageBox::Yes);
	}
}
void QtGuiApplication1::mySlot3() {
	std::wstring wlpstr1 = SerName.toStdWString();
	LPCWSTR lpszDriverName = wlpstr1.c_str();
	std::wstring wlpstr2 = path.toStdWString();
	LPCWSTR lpszDriverPath = wlpstr2.c_str();
	bool B = UnloadNTDriver(lpszDriverName);
	if (B) {
		QMessageBox::information(NULL, "result", "success uninstall", QMessageBox::Yes, QMessageBox::Yes);
	}
	else {
		QMessageBox::information(NULL, "result", "fail   uninstall", QMessageBox::Yes, QMessageBox::Yes);
	}
}