#ifndef ExplodedView_H
#define ExplodedView_H

#include "findNodeVisitor.h"


#include <iostream>


//osg
#include <osgGA/TrackballManipulator>
#include <osgGA/StateSetManipulator>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>
#include <osg/Node>
#include <osgSim/MultiSwitch>


class ExplodedView{

public:
	ExplodedView();

	void setUp();
	void loadModel(char*);
	void loop();

private:
	osgViewer::Viewer* m_viewer;
	osg::Group* m_activeGraphRoot;


};

#endif