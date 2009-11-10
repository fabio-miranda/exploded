#include "Part.h"


Part::Part(){
	m_inserted = false;
	m_visited = false;
	m_exploded = false;
	m_pqpModel = new PQP_Model();
	m_osgTransform = new osg::PositionAttitudeTransform();
	m_osgOriginalTransform = new osg::PositionAttitudeTransform();
	m_ptrFirstProxyPart = NULL;
	m_ptrLastProxyPart = NULL;
	m_currentDistanceExploded = 0;
	m_explosionDirection = NULL;
	

	resetRestrictedMoviments();


	//Calculate bounding box
	m_boundingBox = new osg::BoundingBox();
	//const osg::BoundingBox& bb1 = m_osgNode->getBound();
	
	//TODO: optimized this. The bounding box is bigger than it needs to be (it is the bounding box of the bounding sphere)
	
	
}

//void Part::setUp(VCollide* vCollide, osg::Group* sceneGraphRoot){
void Part::setUp(osg::Group* sceneGraphRoot){
	m_osgTransform->addChild(m_osgNode);
	sceneGraphRoot->addChild(m_osgTransform);
	m_osgOriginalTransform->setPosition(m_osgTransform->getPosition());
	//setVCollide(vCollide);
	setPQP();

	m_boundingBox->expandBy(m_osgNode->getBound());
}
 

//void Part::setVCollide(VCollide* vCollide){
void Part::setPQP(){
	m_inserted = false;

	//AddTrianglesCollision triangles(vCollide);
	//AddTrianglesCollision triangles(m_pqpModel);
	//osg::TriangleIndexFunctor<TriangleIndexVisitor> triangleIndexVisitor;
	osg::TriangleFunctor<TriangleVertexVisitor> triangleVertexVisitor;

	osg::Geometry* geometry;
	geometry = (osg::Geometry *) m_osgNode->asGeode()->getDrawable(0);

	//geometry->accept(triangleIndexVisitor);
	geometry->accept(triangleVertexVisitor);

	//TODO: argh
	m_pqpModel->BeginModel();
	int aux = 0;
	//int index1;
	//int index2;
	//int index3;
	double* v1;
	double* v2;
	double* v3;
	for(int i=0; i<triangleVertexVisitor.m_verticesArray->size()-2; i+=3){
		//index1 = triangleIndexVisitor.m_indicesArray->at(i);
		//index2 = triangleIndexVisitor.m_indicesArray->at(i+1);
		//index3 = triangleIndexVisitor.m_indicesArray->at(i+2);
		v1 = triangleVertexVisitor.m_verticesArray->at(i);
		v2 = triangleVertexVisitor.m_verticesArray->at(i+1);
		v3 = triangleVertexVisitor.m_verticesArray->at(i+2);


		m_pqpModel->AddTri(v1, v2, v3, aux);
		aux++;
	}

	m_pqpModel->EndModel();
	

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

/*
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
*/


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



//void Part::checkCollisionsAlongAxis(osgViewer::Viewer* viewer, VCollide* vCollide, std::vector< Part* > partsGraph, int x, int y, int z, double stepSize, int numIterations, double minimumDistance, bool visualize){
void Part::checkCollisionsAlongAxis(osgViewer::Viewer* viewer, std::vector< Part* > partsGraph, int x, int y, int z, double stepSize, int numIterations, double minimumDistance, bool visualize){
	double distance;
	//double trans[4][4];
	double T1[3], T2[3], R1[3][3], R2[3][3];
	int arrayPosition;

	//VCReport report;

	if(x == 1) arrayPosition = 0;
	else if(x == -1) arrayPosition = 1;
	else if(y == 1)  arrayPosition = 2;
	else if(y == -1) arrayPosition = 3;
	else if(z == 1) arrayPosition = 4;
	else if(z == -1) arrayPosition = 5;
	/*
	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++)
			if(i==j) trans[i][i] = 1;
			else trans[i][j] = 0;
	*/
	
	R1[0][0] = R1[1][1] = R1[2][2] = 1.0;
	R1[0][1] = R1[1][0] = R1[2][0] = 0.0;
	R1[0][2] = R1[1][2] = R1[2][1] = 0.0;

	R2[0][0] = R2[1][1] = R2[2][2] = 1.0;
	R2[0][1] = R2[1][0] = R2[2][0] = 0.0;
	R2[0][2] = R2[1][2] = R2[2][1] = 0.0;

	T1[0] = 0.0;  T1[1] = 0.0; T1[2] = 0.0;
	T2[0] = 0.0;  T2[1] = 0.0; T2[2] = 0.0;

	double distanceOutBoundingBox = 0;

	//It begins from i = 1. That way, the initial collisions will be ignored.
	//So, that way, we won't have to bother about the initials collisions messing the whole parts graph
	//If there is a collision on i=0, the whole graph will be messed up
	for(int i=0; i<numIterations; i++){
		
		//T1[0] = m_osgTransform->getPosition().x() + i*stepSize*x;
		//T1[1] = m_osgTransform->getPosition().y() + i*stepSize*y;
		//T1[2] = m_osgTransform->getPosition().z() + i*stepSize*z;
		T1[0] = i*stepSize*x;
		T1[1] = i*stepSize*y;
		T1[2] = i*stepSize*z;
		
		
		//PQP:
		PQP_ToleranceResult tres;
		for(int j=0; j<partsGraph.size(); j++){
			if(partsGraph[j] != this && partsGraph[j]->m_inserted == false){

				//T2[0] = partsGraph[j]->m_osgTransform->getPosition().x();
				//T2[1] = partsGraph[j]->m_osgTransform->getPosition().y();
				//T2[2] = partsGraph[j]->m_osgTransform->getPosition().z();
				
				PQP_Tolerance(&tres, R1, T1, m_pqpModel, R2, T2, partsGraph[j]->m_pqpModel, 0);
				//PQP_Distance(&dres, R1, T1, m_pqpModel, R2, T2, partsGraph[j]->m_pqpModel, 0, 0);
				
				//double distance = dres.Distance();

				if(tres.CloserThanTolerance()){
					CollisionData* aux = new CollisionData();
					aux->collided = true;
					aux->collidedWith = partsGraph[j];
					aux->collisionDirection[0] = x; 
					aux->collisionDirection[1] = y; 
					aux->collisionDirection[2] = z; 

					

					//if(m_smallestDistanceCollisions[arrayPosition] == NULL) m_countRestrictedDirections++;
					distanceOutBoundingBox = calculateDistanceOutBoundingBox(aux->collidedWith, aux->collisionDirection);
					aux->distanceOutBoundingBox = distanceOutBoundingBox;


					//Check if the collision with this part has been added before
					//TODO: improve that
					if(m_allDistanceCollisions[arrayPosition].size() == 0)
						m_allDistanceCollisions[arrayPosition].push_back(aux);
					else
						for(int k=0; k<m_allDistanceCollisions[arrayPosition].size(); k++){
							if(m_allDistanceCollisions[arrayPosition][k]->collidedWith == aux->collidedWith)
							 //check the distance too and get the closest one
								if(m_allDistanceCollisions[arrayPosition][k]->distanceOutBoundingBox <= aux->distanceOutBoundingBox){
									m_allDistanceCollisions[arrayPosition].erase(m_allDistanceCollisions[arrayPosition].begin()+k);
									m_allDistanceCollisions[arrayPosition].push_back(aux);
								}
						}
				}

			}
		}
		


		//VCollide:
		/*
		trans[0][3] = i*stepSize*x;
		trans[1][3] = i*stepSize*y;
		trans[2][3] = i*stepSize*z;

		vCollide->UpdateTrans(m_vcollideId, trans);

		
		
		vCollide->Collide( &report, VC_ALL_CONTACTS);
		
		int numCollisions = 0;
		//If there is no collision on this direction, then we have a free direction
		for(int j=0; j<report.numObjPairs(); j++){
			if(report.obj1ID(j) == m_vcollideId || report.obj2ID(j) == m_vcollideId){

				CollisionData* aux = new CollisionData();
				aux->collided = true;
				aux->collidedWith = partsGraph[j];
				aux->collisionDirection[0] = x; 
				aux->collisionDirection[1] = y; 
				aux->collisionDirection[2] = z; 

				

				//if(m_smallestDistanceCollisions[arrayPosition] == NULL) m_countRestrictedDirections++;
				distanceOutBoundingBox = calculateDistanceOutBoundingBox(aux->collidedWith, aux->collisionDirection);
				aux->distanceOutBoundingBox = distanceOutBoundingBox;


				//Check if the collision with this part has been added before
				//TODO: improve that
				if(m_allDistanceCollisions[arrayPosition].size() == 0)
					m_allDistanceCollisions[arrayPosition].push_back(aux);
				else
					for(int k=0; k<m_allDistanceCollisions[arrayPosition].size(); k++){
						if(m_allDistanceCollisions[arrayPosition][k]->collidedWith == aux->collidedWith)
						 //check the distance too and get the closest one
							if(m_allDistanceCollisions[arrayPosition][k]->distanceOutBoundingBox <= aux->distanceOutBoundingBox){
								m_allDistanceCollisions[arrayPosition].erase(m_allDistanceCollisions[arrayPosition].begin()+k);
								m_allDistanceCollisions[arrayPosition].push_back(aux);
							}
					}
			}
		}
		*/
		/*
		//If there is two parts that are originally in contact, then this avoids assigning a collision on every direction
		if(numCollisions == 0){
			//m_allDistanceCollisions[arrayPosition].clear();
		}
		*/
	}

	

	//resetPosition(vCollide);

	

}

double Part::findSmallestDistanceOutBoundingBox(){
	
	double smallestDistance = std::numeric_limits<double>::infinity();
	for(int i=0; i<6; i++){
		double currentSmallestDistance = std::numeric_limits<double>::infinity();
		m_smallestDistanceOutBoundingBox[i] = NULL;

		for(int j=0; j<m_allDistanceCollisions[i].size(); j++){
			if(m_allDistanceCollisions[i][j]->collidedWith->m_inserted == false
				&& m_allDistanceCollisions[i][j]->distanceOutBoundingBox < currentSmallestDistance){
				currentSmallestDistance = m_allDistanceCollisions[i][j]->distanceOutBoundingBox;
				smallestDistance = currentSmallestDistance;
				
				m_smallestDistanceOutBoundingBox[i] = m_allDistanceCollisions[i][j];
				m_explosionDirection = m_allDistanceCollisions[i][j];

			}

			
		}
			

	}

	return smallestDistance;
}

void Part::countBlockedDirections(){
	
	m_countRestrictedDirections = 0;
	for(int i=0; i<6; i++){
		for(int j=0; j<m_allDistanceCollisions[i].size(); j++){
			if(m_allDistanceCollisions[i][j]->collidedWith->m_inserted == false){
				m_countRestrictedDirections++;
				break; //only count one collision per direction
			}
		}
	}
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