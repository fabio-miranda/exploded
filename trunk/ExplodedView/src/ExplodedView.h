#ifndef ExplodedView_H
#define ExplodedView_H

#include "FindPartVisitor.h"
#include "Part.h"
#include "keyboardEventHandler.h"
#include "StringExplode.h"
#include "Cell.h"

//#include "PQP.h"


#include <iostream>
#include <limits>
#include <queue>
#include <fstream>
#include <string>
#include <hash_map>




//osg
#include <osgGA/TrackballManipulator>
#include <osgGA/StateSetManipulator>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>
#include <osgUtil/SmoothingVisitor>
#include <osgFX/Cartoon>
#include <osg/Node>
#include <osg/ClipNode>
#include <osgSim/MultiSwitch>
#include <osg/ShapeDrawable>
#include <osg/Texture1D>



class ExplodedView{

public:
	ExplodedView();

	void setUp();
	void loadModelOSG(char* modelName);
	void findPartsOSG();
	void loadModelGeresim(char* modelName);
	void findPartsGeresim(osg::Vec3Array* verticesArray, osg::Vec4Array* colorsArray);
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
	
	std::vector< Cell*>* m_cellsVector;
	std::set<float>* m_propSet;

	

	//TODO: use osg::ref_ptr
	//std::vector< osg::ref_ptr< Part > > m_partsGraph;


};

#endif