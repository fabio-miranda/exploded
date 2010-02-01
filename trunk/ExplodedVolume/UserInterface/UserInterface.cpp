#include "UserInterface.h"

UserInterface::UserInterface(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	
	visualizationWidget = new VisualizationWidget(parent);
	ui.glWidget->addWidget(visualizationWidget);
}

UserInterface::~UserInterface()
{
}

void UserInterface::on_actionOpen_activated(){
	
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Raw Volume"), "../Resources", tr("Raw Files (*.raw)"));

	
	//Pass it to the renderer
	visualizationWidget->loadFile(fileName);


}
