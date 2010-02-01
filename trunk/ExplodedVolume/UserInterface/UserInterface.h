#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <QtGui/QMainWindow>
#include <QFileDialog>
#include "ui_UserInterface.h"
#include "VisualizationWidget.h"

class UserInterface : public QMainWindow
{
	Q_OBJECT

public:
	UserInterface(QWidget *parent = 0, Qt::WFlags flags = 0);
	~UserInterface();

private:
	Ui::UserInterfaceClass ui;
	VisualizationWidget* visualizationWidget;

private slots:
	void on_actionOpen_activated();
};

#endif // USERINTERFACE_H
