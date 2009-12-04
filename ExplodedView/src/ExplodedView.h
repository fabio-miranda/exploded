#ifndef ExplodedView_H
#define ExplodedView_H

#include "FindPartVisitor.h"
#include "Part.h"
#include "keyboardEventHandler.h"

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
	void switchExplodeInplode(int index);
	void explode(int index);
	void inplode(int index);
	void bfs(Part* v, std::vector< std::vector<Part*> >** levels);
	void updateExplodingParts();
	void updateInplodingParts();
	void verifyExplodingParts();
	void findSmallestDistances();
	void countBlockedDirections();
	void findContainers();
	void buildBox();



	//VCollide* m_vCollide;
	osgViewer::Viewer* m_viewer;
	osg::Group* m_sceneGraphRoot;
	Part* m_ptrCurrentPart; //the current part being inploded or exploded
	std::vector< Part* > m_partsGraph;
	osg::Node* m_loadedModel;

private:
	
	std::vector< std::vector<Part*> >* m_explodingLevels;
	std::vector< std::vector<Part*> >* m_inplodingLevels;

	

	//TODO: use osg::ref_ptr
	//std::vector< osg::ref_ptr< Part > > m_partsGraph;


};

#endif