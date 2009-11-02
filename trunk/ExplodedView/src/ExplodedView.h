#ifndef ExplodedView_H
#define ExplodedView_H

#include "FindPartVisitor.h"
#include "Part.h"

#include "VCollide.h"


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
	void buildPartsGraph(char* modelName);
	void loop();

private:

	

	osgViewer::Viewer* m_viewer;
	osg::Group* m_activeGraphRoot;

	VCollide* m_vcollide;


};

#endif