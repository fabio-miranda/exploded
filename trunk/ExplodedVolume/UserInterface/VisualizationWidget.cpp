#include "VisualizationWidget.h"

VisualizationWidget::VisualizationWidget( QWidget* parent)
  : QGLWidget( parent )
{
	
	volumeRenderer = new VolumeRenderer();

}

VisualizationWidget::~VisualizationWidget(void)
{
}

void VisualizationWidget::paintGL()
{
	volumeRenderer->render();
	//glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable( GL_DEPTH_TEST ); 
	//glLoadIdentity();
}
