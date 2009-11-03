#ifndef PART_H
#define PART_H

#include "AddTrianglesCollision.h"
#include "ProxyPart.h"
#include "CollisionData.h"


#include "PQP.H"

//osg
#include <osg/Node>
#include <osg/PositionAttitudeTransform>
#include <osgViewer/Viewer>


class Part {

public:
	Part();
	void setPQPModel();
	//void setOSGNode(osg::Node* node);
	//osg::Node* getOSGNode();
	void resetRestrictedMoviments();
	void insertVertex(Part* vertexToPart);
	
	void findDistancesOutBoundingBox(osgViewer::Viewer* viewer, double stepSize, double minimumDistance, bool visualize);
	void checkCollisionsAlongAxis(osgViewer::Viewer* viewer, Part* compareTo, int x, int y, int z, double stepSize, int numIterations, double minimumDistance, bool visualize);
	double calculateDistance(Part* compareTo, PQP_REAL translation_x, PQP_REAL translation_y, PQP_REAL translation_z);

	PQP_Model* m_pqpModel;
	bool m_inserted; //nodes already inserted into the collision graph
	CollisionData m_collisions[6]; //0,1:x ; 2,3:y; 4,5:z
	int m_countRestrictedDirections;

	osg::Node* m_osgNode;
	osg::PositionAttitudeTransform* m_osgTransform;


private:
	
	ProxyPart* m_ptrFirstProxyPart;
	ProxyPart* m_ptrLastProxyPart;
	

};

#endif