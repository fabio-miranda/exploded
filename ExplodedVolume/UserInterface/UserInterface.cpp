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
	
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Raw Volume"), "../VolumeRenderer/Resources", tr("Raw Files (*.raw)"));

	//Load file
	std::ifstream stream (fileName.toLatin1(), std::ios::in|std::ios::binary|std::ios::ate);
	std::ifstream::pos_type size = stream.tellg();
	char * memblock = new char [size];
	stream.seekg (0, std::ios::beg);
    stream.read (memblock, size);
    stream.close();

	//Pass it to the renderer


}
