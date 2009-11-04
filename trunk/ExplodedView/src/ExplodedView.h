#ifndef ExplodedView_H
#define ExplodedView_H

#include "FindPartVisitor.h"
#include "Part.h"

#include "PQP.h"


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
#include <osgSim/MultiSwitch>


class ExplodedView{

public:
	ExplodedView();

	void setUp();
	void buildPartsGraph(char* modelName);
	void loop();

	void findBlockedDirections();
	void calculateDistancesOutBB();
	Part* findSmallestDistance();
	void insertOnPartsGraph(Part* part);
	void printGraph();
	void explode();
	void bfs(Part* v);
	void updateExplodingParts();
	void verifyExplodingParts();

private:
	double ExplodedView::calculateDistance(int index1, int index2, PQP_REAL translation_x, PQP_REAL translation_y, PQP_REAL translation_z);
	

	osgViewer::Viewer* m_viewer;
	osg::Group* m_sceneGraphRoot;

	std::vector< Part* > m_partsGraph;
	std::vector< std::vector<Part*> >* m_explodingLevels;

	//TODO:
	//std::vector< osg::ref_ptr< Part > > m_partsGraph;


};

#endif