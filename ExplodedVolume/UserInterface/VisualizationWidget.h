#ifndef VISUALIZATIONWIDGET_H
#define VISUALIZATIONWIDGET_H

#include "qgl.h"
#include "VolumeRenderer.h"

class VisualizationWidget : public QGLWidget
{
  Q_OBJECT
public:
  VisualizationWidget( QWidget* parent);
  ~VisualizationWidget();

protected:
    void paintGL();

	VolumeRenderer* volumeRenderer;
};

#endif // VISUALIZATIONWIDGET_H

