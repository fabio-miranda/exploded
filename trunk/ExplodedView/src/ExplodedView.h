#ifndef ExplodedView_H
#define ExplodedView_H


#include "ModelLoader.h"
#include "Part.h"

#include <vector>
#include <iostream>

#include <osg/Node>
#include <osg/Geometry>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgUtil/Optimizer>
#include <osgGA/TrackballManipulator>
#include <osgGA/StateSetManipulator>

class ExplodedView{
	
public:
	ExplodedView();
	void run();

	


private:
	void findParts();
	void setUpOSG();

	
	osgViewer::Viewer* mViewer;
	osg::Geometry* mGeometry;
	osg::Geode* mGeode;
	std::vector<Cell*>* mPartsVector;
	std::vector<Cell*>* mCellArray;
	


};



#endif