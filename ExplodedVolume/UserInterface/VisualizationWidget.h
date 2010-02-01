#ifndef VISUALIZATIONWIDGET_H
#define VISUALIZATIONWIDGET_H


#include "VolumeRenderer.h"
#include <QGLWidget>
#include <iostream>
#include <fstream>


class VisualizationWidget : public QGLWidget
{
  Q_OBJECT
public:
  VisualizationWidget( QWidget* parent);
  ~VisualizationWidget();

  void loadFile(QString);

protected:
    void paintGL();
	void initializeGL();
	void resizeGL(int w, int h);
	

	VolumeRenderer* volumeRenderer;
};

#endif // VISUALIZATIONWIDGET_H

