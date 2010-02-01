#include "VisualizationWidget.h"

VisualizationWidget::VisualizationWidget( QWidget* parent)
  : QGLWidget( parent )
{

}

VisualizationWidget::~VisualizationWidget(void)
{
}

void VisualizationWidget::initializeGL(){

	volumeRenderer = new VolumeRenderer();
}

void VisualizationWidget::paintGL()
{
	volumeRenderer->render();
}

void VisualizationWidget::resizeGL(int w, int h)
{
	volumeRenderer->resize(w, h);
}

void VisualizationWidget::loadFile(QString fileName){

	//Load file
	volumeRenderer->loadFile(fileName.toLatin1().data());


}
