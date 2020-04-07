#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtGuiApplication1.h"
#include<QFileDialog>
#include<iostream>
#include <QMessageBox>
#include"win.h"
using namespace std;
extern QString SerName;
extern QString path;
class QtGuiApplication1 : public QMainWindow
{
	Q_OBJECT

public:
	QtGuiApplication1(QWidget* parent = Q_NULLPTR);

private:
	Ui::QtGuiApplication1Class ui;

private slots:
	void mySlot1();
	void mySlot2();
	void mySlot3();
};
