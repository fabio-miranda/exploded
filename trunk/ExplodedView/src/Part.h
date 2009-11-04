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
	void insertVertexFrom(Part* vertexToPart);
	void explode(double stepSize);
	
	void findDistancesOutBoundingBox(osgViewer::Viewer* viewer, double stepSize, double minimumDistance, bool visualize);
	void checkCollisionsAlongAxis(osgViewer::Viewer* viewer, Part* compareTo, int x, int y, int z, double stepSize, int numIterations, double minimumDistance, bool visualize);
	double calculateDistance(Part* compareTo, PQP_REAL translation_x, PQP_REAL translation_y, PQP_REAL translation_z);

	PQP_Model* m_pqpModel;
	bool m_inserted; //nodes already inserted into the collision graph
	//CollisionData m_collisions[6]; //0,1:x ; 2,3:y; 4,5:z
	std::vector< CollisionData* > m_collisions[6]; //the parts in which the current part is blocked (in each direction)
												   //there can be more than only one part per direction
	int m_countRestrictedDirections;
	CollisionData* m_explosionDirection;// Pointer to the CollisionData that represents the explosion direction
	double m_currentDistanceExploded;
	bool m_exploded;

	osg::Node* m_osgNode;
	osg::PositionAttitudeTransform* m_osgTransform;

	ProxyPart* m_ptrFirstProxyPart;
	ProxyPart* m_ptrLastProxyPart;
	bool m_visited;
	


private:
	
	

};

#endif