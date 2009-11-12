#ifndef ExplodedView_H
#define ExplodedView_H

#include "FindPartVisitor.h"
#include "Part.h"

//#include "PQP.h"


#include <iostream>
#include <limits>
#include <queue>


//osg
#include <osgGA/TrackballManipulator>
#include <osgGA/StateSetManipulator>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>
#include <osg/Node>
#include <osg/ClipNode>
#include <osgSim/MultiSwitch>
#include <osg/ShapeDrawable>


class ExplodedView{

public:
	ExplodedView();

	void setUp(char* modelName);
	void buildPartsGraph();
	void run();

	void findBlockedDirections();
	void calculateDistancesOutBB();
	Part* findSmallestDistanceOutBoundingBox();
	void insertOnPartsGraph(Part* part);
	void printGraph();
	void explode();
	void bfs(Part* v);
	void updateExplodingParts();
	void verifyExplodingParts();
	void findSmallestDistances();
	void countBlockedDirections();
	void findContainers();
	void buildBox();



	//VCollide* m_vCollide;
	osgViewer::Viewer* m_viewer;
	osg::Group* m_sceneGraphRoot;
	std::vector< Part* > m_partsGraph;

private:
	
	std::vector< std::vector<Part*> >* m_explodingLevels;

	

	//TODO:
	//std::vector< osg::ref_ptr< Part > > m_partsGraph;


};

#endif