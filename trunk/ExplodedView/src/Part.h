#ifndef PART_H
#define PART_H

#include "TriangleVertexVisitor.h"
#include "TriangleIndexVisitor.h"
#include "ProxyPart.h"
#include "CollisionData.h"


#include "PQP.H"
//#include "VCollide.h"

//osg
#include <osg/Node>
#include <osg/PositionAttitudeTransform>
#include <osgViewer/Viewer>
#include <osg/TriangleIndexFunctor>
#include <osg/TriangleFunctor>

#include <limits>


class Part {

public:
	Part();
	//void setVCollide(VCollide* vCollide);
	void setPQP();
	//void setOSGNode(osg::Node* node);
	//osg::Node* getOSGNode();
	void resetRestrictedMoviments();
	//void resetPosition(VCollide* vCollide);
	void insertVertexFrom(Part* vertexToPart);
	void explode(double stepSize);
	//void setUp(VCollide* vCollide, osg::Group* sceneGraphRoot);
	void setUp(osg::Group* sceneGraphRoot);
	double calculateDistanceOutBoundingBox(Part* collidedWith, double* collisionDirection);
	double findSmallestDistanceOutBoundingBox();
	void countBlockedDirections();
	
	void calculateDistancesOutBoundingBox();
	//void calculateDistancesOutBoundingBox(osgViewer::Viewer* viewer, double stepSize, double minimumDistance, bool visualize);
	//void checkCollisionsAlongAxis(osgViewer::Viewer* viewer, VCollide* vCollide, std::vector< Part* > partsGraph, int x, int y, int z, double stepSize, int numIterations, double minimumDistance, bool visualize);
	void checkCollisionsAlongAxis(osgViewer::Viewer* viewer, std::vector< Part* > partsGraph, int x, int y, int z, double stepSize, int numIterations, double minimumDistance, bool visualize);
	//double calculateDistance(Part* compareTo, PQP_REAL translation_x, PQP_REAL translation_y, PQP_REAL translation_z);

	//int m_vcollideId; //equal to the position of the part in the m_partsGraph array
	bool m_inserted; //nodes already inserted into the collision graph
	//CollisionData m_collisions[6]; //0,1:x ; 2,3:y; 4,5:z
	CollisionData* m_smallestDistanceOutBoundingBox[6]; //the smallest distances out of the bounding box
	std::vector<CollisionData*> m_allDistanceCollisions[6];//the parts in which the current part is blocked (in each direction)
															//there can be more than only one part per direction

	PQP_Model* m_pqpModel;
	//int m_vcollideId;

	int m_countRestrictedDirections;
	CollisionData* m_explosionDirection;// Pointer to the CollisionData that represents the explosion direction
	double m_currentDistanceExploded;
	bool m_exploded;

	osg::Node* m_osgNode;
	osg::PositionAttitudeTransform* m_osgTransform;
	osg::PositionAttitudeTransform* m_osgOriginalTransform;
	osg::BoundingBox* m_boundingBox;

	ProxyPart* m_ptrFirstProxyPart;
	ProxyPart* m_ptrLastProxyPart;
	bool m_visited;
	


private:
	
	

};

#endif