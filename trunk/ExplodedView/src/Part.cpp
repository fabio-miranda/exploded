#include "Part.h"


Part::Part(){
	m_inserted = false;
	m_visited = false;
	m_exploded = false;
	//m_pqpModel = new PQP_Model();
	m_osgTransform = new osg::PositionAttitudeTransform();
	m_osgOriginalTransform = new osg::PositionAttitudeTransform();
	m_ptrFirstProxyPart = NULL;
	m_ptrLastProxyPart = NULL;
	m_currentDistanceExploded = 0;
	

	resetRestrictedMoviments();


	//Calculate bounding box
	m_boundingBox = new osg::BoundingBox();
	//const osg::BoundingBox& bb1 = m_osgNode->getBound();
	
	//TODO: optimized this. The bounding box is bigger than it needs to be (it is the bounding box of the bounding sphere)
	
	
}

void Part::setUp(VCollide* vCollide, osg::Group* sceneGraphRoot){
	m_osgTransform->addChild(m_osgNode);
	sceneGraphRoot->addChild(m_osgTransform);
	m_osgOriginalTransform->setPosition(m_osgTransform->getPosition());
	setVCollide(vCollide);

	m_boundingBox->expandBy(m_osgNode->getBound());
}
 

void Part::setVCollide(VCollide* vCollide){
	//m_pqpModel = new PQP_Model();
	m_inserted = false;

	//m_pqpModel->BeginModel();
	vCollide->NewObject(&m_vcollideId);

	AddTrianglesCollision triangles(vCollide);

	m_osgNode->accept(triangles);

	//m_pqpModel->EndModel();
	vCollide->EndObject();
	

}
/*
void Part::setOSGNode(osg::Node* node){
	m_osgNode = node;

}

osg::Node* Part::getOSGNode(){
	return m_osgNode;
}
*/

void Part::resetRestrictedMoviments(){
	
	m_countRestrictedDirections = 0;
	
	for(int i=0; i<6; i++){
		m_allDistanceCollisions[i].clear();
		//m_collisions[i].collided = false;
		//m_collisions[i].collidedWith = NULL;
		//m_smallestDistanceCollisions[i] = NULL;
	}
	
}

void Part::resetPosition(VCollide* vCollide){
	double trans[4][4];

	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++)
			if(i==j) trans[i][i] = 1;
			else trans[i][j] = 0;

	trans[0][3] = m_osgOriginalTransform->getPosition().x();
	trans[1][3] = m_osgOriginalTransform->getPosition().y();
	trans[2][3] = m_osgOriginalTransform->getPosition().z();

	vCollide->UpdateTrans(m_vcollideId, trans);

	m_osgTransform->setPosition(m_osgOriginalTransform->getPosition());

}

void Part::explode(double stepSize){

	if(m_exploded) return;

	osg::Vec3d currentPosition = m_osgTransform->getPosition();
	double* explosionDirection = m_explosionDirection->collisionDirection;

	osg::Vec3d newPosition = osg::Vec3d(currentPosition.x() - explosionDirection[0]*stepSize,
										   currentPosition.y() - explosionDirection[1]*stepSize,
										   currentPosition.z() - explosionDirection[2]*stepSize);
	
	m_currentDistanceExploded += (newPosition - currentPosition).length();

	if(m_currentDistanceExploded < m_explosionDirection->distanceOutBoundingBox)
		m_osgTransform->setPosition(newPosition);
	else
		m_exploded = true;

}

void Part::insertVertexFrom(Part* vertexFrom){
	
	ProxyPart* proxyPart = new ProxyPart(this);

	if(vertexFrom->m_ptrFirstProxyPart == NULL){
		vertexFrom->m_ptrFirstProxyPart = proxyPart;
		vertexFrom->m_ptrLastProxyPart = proxyPart;
	}
	else{
		vertexFrom->m_ptrLastProxyPart->m_ptrNextProxyPart = proxyPart;
		vertexFrom->m_ptrLastProxyPart = proxyPart;
	}

}



void Part::checkCollisionsAlongAxis(osgViewer::Viewer* viewer, VCollide* vCollide, std::vector< Part* > partsGraph, int x, int y, int z, double stepSize, int numIterations, double minimumDistance, bool visualize){
	double distance;
	double trans[4][4]; //transformation matrix
	VCReport report;
	
	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++)
			if(i==j) trans[i][i] = 1;
			else trans[i][j] = 0;
	
	double distanceOutBoundingBox = 0;
	for(int i=0; i<numIterations; i++){
		
		trans[0][3] = m_osgTransform->getPosition().x() + i*stepSize*x;
		trans[1][3] = m_osgTransform->getPosition().y() + i*stepSize*y;
		trans[2][3] = m_osgTransform->getPosition().z() + i*stepSize*z;

		vCollide->UpdateTrans(m_vcollideId, trans);
		
		
		if(visualize){
			m_osgTransform->setPosition(osg::Vec3d(trans[0][3], trans[1][3], trans[2][3]));
			viewer->frame();
		}
		

		vCollide->Collide( &report, VC_ALL_CONTACTS);

		//If there is no collision on this direction, then we have a free direction
		for(int j=0; j<report.numObjPairs(); j++){
			int collidedId;
			double distanceOutBoundingBox;
			int arrayPosition;

			report.obj1ID(j) != m_vcollideId ?  collidedId = report.obj1ID(j) : collidedId = report.obj2ID(j);


			CollisionData* aux = new CollisionData();
			aux->collided = true;
			aux->collidedWith = partsGraph[collidedId];
			aux->collisionDirection[0] = x; 
			aux->collisionDirection[1] = y; 
			aux->collisionDirection[2] = z; 

			if(x == 1) arrayPosition = 0;
			else if(x == -1) arrayPosition = 1;
			else if(y == 1)  arrayPosition = 2;
			else if(y == -1) arrayPosition = 3;
			else if(z == 1) arrayPosition = 4;
			else if(z == -1) arrayPosition = 5;

			//if(m_smallestDistanceCollisions[arrayPosition] == NULL) m_countRestrictedDirections++;
			distanceOutBoundingBox = calculateDistanceOutBoundingBox(aux->collidedWith, aux->collisionDirection);
			aux->distanceOutBoundingBox = distanceOutBoundingBox;

			/*
			if(m_smallestDistanceCollisions[arrayPosition] == NULL ||
				distanceOutBoundingBox < m_smallestDistanceCollisions[arrayPosition]->distanceOutBoundingBox){

				aux->distanceOutBoundingBox = distanceOutBoundingBox;
				m_smallestDistanceCollisions[arrayPosition] = aux;
			}
			*/

			//Check if the collision with this part has been added before
			//TODO: improve that
			if(m_allDistanceCollisions[arrayPosition].size() == 0)
				m_allDistanceCollisions[arrayPosition].push_back(aux);
			else
				for(int k=0; k<m_allDistanceCollisions[arrayPosition].size(); k++){
					if(m_allDistanceCollisions[arrayPosition][k]->collidedWith->m_vcollideId == aux->collidedWith->m_vcollideId)
					 //check the distance too and get the closest one
						if(m_allDistanceCollisions[arrayPosition][k]->distanceOutBoundingBox <= aux->distanceOutBoundingBox){
							m_allDistanceCollisions[arrayPosition].erase(m_allDistanceCollisions[arrayPosition].begin()+k);
							m_allDistanceCollisions[arrayPosition].push_back(aux);
						}
				}

			
		}

	}

	resetPosition(vCollide);

	

}

double Part::findSmallestDistance(){
	
	
	double smallestDistance = std::numeric_limits<double>::infinity();
	for(int i=0; i<6; i++){
		double currentSmallestDistance = std::numeric_limits<double>::infinity();
		m_smallestDistanceCollisions[i] = NULL;

		for(int j=0; j<m_allDistanceCollisions[i].size(); j++){
			if(m_allDistanceCollisions[i][j]->collidedWith->m_inserted == false
				&& m_allDistanceCollisions[i][j]->distanceOutBoundingBox < currentSmallestDistance){
				currentSmallestDistance = m_allDistanceCollisions[i][j]->distanceOutBoundingBox;
				smallestDistance = currentSmallestDistance;
				
				m_smallestDistanceCollisions[i] = m_allDistanceCollisions[i][j];
				m_explosionDirection = m_allDistanceCollisions[i][j];

			}
		}

		if(m_smallestDistanceCollisions[i] != NULL)
			m_countRestrictedDirections++;

	}

	return smallestDistance;

}

double Part::calculateDistanceOutBoundingBox(Part* collidedWith, double* collisionDirection){
	
	osg::Vec3d vector;

	osg::BoundingBox* bb1 = m_boundingBox;
	osg::BoundingBox* bb2 = collidedWith->m_boundingBox;

	vector.set(osg::minimum(bb1->xMax(), bb2->xMax()) - osg::maximum(bb1->xMin(), bb2->xMin()),
				osg::minimum(bb1->yMax(), bb2->yMax()) - osg::maximum(bb1->yMin(), bb2->yMin()),
				osg::minimum(bb1->zMax(), bb2->zMax()) - osg::maximum(bb1->zMin(), bb2->zMin()));

	return vector.length();
}