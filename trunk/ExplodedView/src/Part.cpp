#include "Part.h"


Part::Part(){
	m_inserted = false;
	m_visited = false;
	m_exploded = false;
	m_exploding = false;
	m_inploding = false;
	m_inploded = true;
	m_container = false;
	m_pqpModel = new PQP_Model();
	m_osgTransform = new osg::PositionAttitudeTransform();
	m_osgOriginalTransform = new osg::PositionAttitudeTransform();
	m_osgOcclusionQueryNode = new osg::OcclusionQueryNode();
	m_ptrFirstProxyPart = NULL;
	m_ptrLastProxyPart = NULL;
	m_currentDistanceExploded = 0;
	m_segmentedParts = NULL;
	m_explosionDirection = NULL;
	created = false;
	

	resetRestrictedMoviments();


	//Calculate bounding box
	//m_boundingBox = new osg::BoundingBox();
	//const osg::BoundingBox& bb1 = m_osgNode->getBound();
	
	//TODO: optimized this. The bounding box is bigger than it needs to be (it is the bounding box of the bounding sphere)
	
	
}

//void Part::setUp(VCollide* vCollide, osg::Group* sceneGraphRoot){
void Part::setUp(osg::Group* sceneGraphRoot){
	
	m_osgTransform->addChild(m_osgOcclusionQueryNode);
	m_osgOcclusionQueryNode->addChild(m_osgNode);
	m_osgOcclusionQueryNode->setDebugDisplay(true);
	sceneGraphRoot->addChild(m_osgTransform);
	m_osgOriginalTransform->setPosition(m_osgTransform->getPosition());
	//setVCollide(vCollide);
	setPQP();

	//TODO: Improve the bounding box
	osg::ComputeBoundsVisitor cbbv;
	m_osgNode->accept(cbbv);
	m_boundingBox = new osg::BoundingBox(cbbv.getBoundingBox());
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
	for(unsigned int i=0; i < triangleVertexVisitor.m_verticesArray->size()-2; i+=3){
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


	if(m_container){
		m_segmentedParts->explode(stepSize);

		if(m_segmentedParts->m_exploded == false){
			m_exploding = true;
			m_exploded = false;
		}
		else{
			m_exploding = false;
			m_exploded = true;
		}
	}
	else{

		if(m_exploded) return;

		osg::Vec3d currentPosition = m_osgTransform->getPosition();
		double* explosionDirection = m_explosionDirection->collisionDirection;

		osg::Vec3d newPosition = osg::Vec3d(currentPosition.x() - explosionDirection[0]*stepSize,
											   currentPosition.y() - explosionDirection[1]*stepSize,
											   currentPosition.z() - explosionDirection[2]*stepSize);
		
		m_currentDistanceExploded += (newPosition - currentPosition).length();

		if(m_currentDistanceExploded < m_explosionDirection->distanceOutBoundingBox){
			m_osgTransform->setPosition(newPosition);
			m_exploding = true;
		}
		else{
			m_exploded = true;
			m_inploded = false;
			m_exploding = false;
		}
	}

}

void Part::inplode(double stepSize){
	
	//if(m_exploded == false) return;
	//if(m_inploded == true) return;

	if(m_container){
		m_segmentedParts->inplode(stepSize);

		if(m_segmentedParts->m_inploded == false){
			m_inploding = true;
			m_inploded = false;
		}
		else{
			m_inploding = false;
			m_inploded = true;
		}

	}
	else{
	
		if(m_inploded == true) return;

		osg::Vec3d currentPosition = m_osgTransform->getPosition();
		double* explosionDirection = m_explosionDirection->collisionDirection;

		osg::Vec3d newPosition = osg::Vec3d(currentPosition.x() + explosionDirection[0]*stepSize,
											   currentPosition.y() + explosionDirection[1]*stepSize,
											   currentPosition.z() + explosionDirection[2]*stepSize);
		
		m_currentDistanceExploded -= (newPosition - currentPosition).length();

		if(m_currentDistanceExploded > 0){
			m_osgTransform->setPosition(newPosition);
			m_inploding = true;
		}
		else{
			m_inploded = true;
			m_exploded = false;
			m_inploding = false;
			m_currentDistanceExploded = 0;
			m_osgTransform->setPosition(m_osgOriginalTransform->getPosition());
		}
	}

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
	bool itsFreeDirection = true;
	CollisionData* freeDirection = new CollisionData();

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
		for(unsigned int  j=0; j<partsGraph.size(); j++){
			if(partsGraph[j] != this && partsGraph[j]->m_inserted == false){

				//T2[0] = partsGraph[j]->m_osgTransform->getPosition().x();
				//T2[1] = partsGraph[j]->m_osgTransform->getPosition().y();
				//T2[2] = partsGraph[j]->m_osgTransform->getPosition().z();
				
				PQP_Tolerance(&tres, R1, T1, m_pqpModel, R2, T2, partsGraph[j]->m_pqpModel, 0);
				//PQP_Distance(&dres, R1, T1, m_pqpModel, R2, T2, partsGraph[j]->m_pqpModel, 0, 0);
				
				//double distance = dres.Distance();
				CollisionData* aux = new CollisionData();
				
				if(tres.CloserThanTolerance()){
					aux->collided = true;
					aux->collidedWith = partsGraph[j];
					aux->collisionDirection[0] = x; 
					aux->collisionDirection[1] = y; 
					aux->collisionDirection[2] = z; 

					

					//if(m_smallestDistanceCollisions[arrayPosition] == NULL) m_countRestrictedDirections++;
					distanceOutBoundingBox = calculateDistanceOutBoundingBox(aux->collidedWith, aux->collisionDirection);
					aux->distanceOutBoundingBox = distanceOutBoundingBox;


					itsFreeDirection = false;


					
				}
				//free direction
				else if(itsFreeDirection){
					if(partsGraph[j]->contains(m_boundingBox) || contains(partsGraph[j]->m_boundingBox)){
						
						freeDirection->collided = false;
						freeDirection->collidedWith = partsGraph[j];
						freeDirection->collisionDirection[0] = -x; 
						freeDirection->collisionDirection[1] = -y; 
						freeDirection->collisionDirection[2] = -z; 

						

						//if(m_smallestDistanceCollisions[arrayPosition] == NULL) m_countRestrictedDirections++;
						distanceOutBoundingBox = calculateDistanceOutBoundingBox(freeDirection->collidedWith, freeDirection->collisionDirection);
						freeDirection->distanceOutBoundingBox = distanceOutBoundingBox;

					}
					
					
				}

				//Check if the collision with this part has been added before
				//TODO: improve that
				if(aux->collidedWith != NULL)
					if(m_allDistanceCollisions[arrayPosition].size() == 0)
						m_allDistanceCollisions[arrayPosition].push_back(aux);
					else
						for(unsigned int  k=0; k<m_allDistanceCollisions[arrayPosition].size(); k++){
							if(m_allDistanceCollisions[arrayPosition][k]->collidedWith == aux->collidedWith
								&& aux->collided == true)
							 //check the distance too and get the closest one
								if(m_allDistanceCollisions[arrayPosition][k]->distanceOutBoundingBox <= aux->distanceOutBoundingBox){
									m_allDistanceCollisions[arrayPosition].erase(m_allDistanceCollisions[arrayPosition].begin()+k);
									m_allDistanceCollisions[arrayPosition].push_back(aux);
								}
						}


			}
		}
	}

	if(itsFreeDirection){
		if(freeDirection->collidedWith != NULL)
				if(m_allDistanceCollisions[arrayPosition].size() == 0)
					m_allDistanceCollisions[arrayPosition].push_back(freeDirection);
				else
					for(unsigned int  k=0; k<m_allDistanceCollisions[arrayPosition].size(); k++){
						if(m_allDistanceCollisions[arrayPosition][k]->collidedWith == freeDirection->collidedWith
							&& freeDirection->collided == true)
						 //check the distance too and get the closest one
							if(m_allDistanceCollisions[arrayPosition][k]->distanceOutBoundingBox <= freeDirection->distanceOutBoundingBox){
								m_allDistanceCollisions[arrayPosition].erase(m_allDistanceCollisions[arrayPosition].begin()+k);
								m_allDistanceCollisions[arrayPosition].push_back(freeDirection);
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

	

	//resetPosition(vCollide);

	

}

CollisionData* Part::findSmallestDistanceOutBoundingBox(){
	
	double smallestDistance = std::numeric_limits<double>::infinity();
	CollisionData* currentCollisionData = NULL;
	for(int i=0; i<6; i++){
		double currentSmallestDistance = std::numeric_limits<double>::infinity();
		
		
		m_smallestDistanceOutBoundingBox[i] = NULL;
		for(unsigned int j=0; j<m_allDistanceCollisions[i].size(); j++){


			//Found a clear direction
			//TODO: this is not necessarily the shortest distance
			if(m_allDistanceCollisions[i][j]->collided == false){
				currentSmallestDistance = m_allDistanceCollisions[i][j]->distanceOutBoundingBox;
				smallestDistance = currentSmallestDistance;
				
				m_smallestDistanceOutBoundingBox[i] = m_allDistanceCollisions[i][j];
				m_explosionDirection = m_allDistanceCollisions[i][j];

				return  m_allDistanceCollisions[i][j];
			}
			
			if((m_allDistanceCollisions[i][j]->collidedWith->m_inserted == false
				&& m_allDistanceCollisions[i][j]->distanceOutBoundingBox < currentSmallestDistance)){
				currentSmallestDistance = m_allDistanceCollisions[i][j]->distanceOutBoundingBox;
				currentCollisionData = m_allDistanceCollisions[i][j];
				smallestDistance = currentSmallestDistance;
				
				m_smallestDistanceOutBoundingBox[i] = m_allDistanceCollisions[i][j];
				m_explosionDirection = m_allDistanceCollisions[i][j];

			}	
		}
	}

	return currentCollisionData;
}

void Part::countBlockedDirections(){
	
	m_countRestrictedDirections = 0;
	for(int i=0; i<6; i++){
		for(unsigned int j=0; j<m_allDistanceCollisions[i].size(); j++){
			if(m_allDistanceCollisions[i][j]->collidedWith->m_inserted == false
				&& m_allDistanceCollisions[i][j]->collided == true){
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

	vector.set(collisionDirection[0] * (osg::minimum(bb1->xMax(), bb2->xMax()) - osg::maximum(bb1->xMin(), bb2->xMin())),
				collisionDirection[1] * (osg::minimum(bb1->yMax(), bb2->yMax()) - osg::maximum(bb1->yMin(), bb2->yMin())),
				collisionDirection[2] * (osg::minimum(bb1->zMax(), bb2->zMax()) - osg::maximum(bb1->zMin(), bb2->zMin())));
	
	return vector.length();
}

bool Part::contains(osg::BoundingBox* bb){
	if(m_boundingBox->contains(bb->corner(0))
		&& m_boundingBox->contains(bb->corner(1))
		&& m_boundingBox->contains(bb->corner(2))
		&& m_boundingBox->contains(bb->corner(3))
		&& m_boundingBox->contains(bb->corner(4))
		&& m_boundingBox->contains(bb->corner(5))
		&& m_boundingBox->contains(bb->corner(6))
		&& m_boundingBox->contains(bb->corner(7)))
		return true;
	else
		return false;
}

void Part::findContainer(){
	
	bool interlocked = true;
	if(m_countRestrictedDirections == 6){
		for(int j=1; j<6; j++){
			if(m_smallestDistanceOutBoundingBox[j-1] != NULL && m_smallestDistanceOutBoundingBox[j] != NULL)
				if(m_smallestDistanceOutBoundingBox[j-1]->collidedWith != m_smallestDistanceOutBoundingBox[j]->collidedWith){
					if(m_smallestDistanceOutBoundingBox[j-1]->collidedWith->m_inserted == true)
						interlocked = false;
				}

		}

		if(interlocked){
			//Test which part contains which one.
			//In other words, test to see if it is a container or a interlocked part
			if(contains(m_smallestDistanceOutBoundingBox[0]->collidedWith->m_boundingBox)){
				m_container = true;
				m_partsContained.push_back(m_smallestDistanceOutBoundingBox[0]->collidedWith);
			}

		}
	}
	
	
	
}

void Part::split(osg::Group* sceneGraphRoot , osgViewer::Viewer* viewer, double stepsize){
	
	
	m_osgNode->setNodeMask(0);
	

	//Test the three possible cutting planes: xy, xz, yz
	//xy:
	SegmentedParts* segPart_xy = new SegmentedParts(0, sceneGraphRoot, this, osg::Vec3d(0,0,1));
	double distance_xy = segPart_xy->explodeUntilVisible(viewer, stepsize, m_partsContained[0]);

	//xz
	SegmentedParts* segPart_xz = new SegmentedParts(2, sceneGraphRoot, this, osg::Vec3d(0,1,0));
	double distance_xz = segPart_xz->explodeUntilVisible(viewer, stepsize, m_partsContained[0]);
	

	//yz
	SegmentedParts* segPart_yz = new SegmentedParts(4, sceneGraphRoot, this, osg::Vec3d(1,0,0));
	double distance_yz = segPart_yz->explodeUntilVisible(viewer, stepsize, m_partsContained[0]);

	
	if(distance_xy <= distance_xz && distance_xy <= distance_yz){
		delete segPart_yz;
		delete segPart_xz;

		m_segmentedParts = segPart_xy;
	}
	else if(distance_xz <= distance_xy && distance_xz <= distance_yz){
		delete segPart_xy;
		delete segPart_yz;

		m_segmentedParts = segPart_xz;
		
	}
	else{
		delete segPart_xz;
		delete segPart_xy;

		m_segmentedParts = segPart_yz;
	}
	m_segmentedParts->m_osgTransform1->setPosition(m_osgTransform->getPosition());
	m_segmentedParts->m_osgTransform2->setPosition(m_osgTransform->getPosition());

	
	
}


void Part::turnHighlight(bool turnHighlight){
	
	osg::Vec4 red(1.0f, 0.0f, 0.0f, 1.0f);
	m_osgNode->accept(ColorVisitor(red));
	
	

	
}

void Part::move(CollisionData* collision, int signal, double stepSize){

	


	if(m_currentDistanceExploded <= stepSize && signal == -1){
		m_currentDistanceExploded = 0;
		return;
	}

	if(m_container && m_segmentedParts != NULL){
		m_segmentedParts->move(collision, signal, stepSize);
	}

	//Move all its descendents
	/*
	ProxyPart* ptrProxyPart = m_ptrFirstProxyPart;
	while(ptrProxyPart != NULL){
		ptrProxyPart->m_ptrActualPart->move(ptrProxyPart->m_ptrActualPart->m_explosionDirection, signal, stepSize);
		ptrProxyPart = ptrProxyPart->m_ptrNextProxyPart;
	}
	*/
	

	
	osg::Vec3d currentPosition = m_osgTransform->getPosition();

	osg::Vec3d newPosition = osg::Vec3d(currentPosition.x() - signal* collision->collisionDirection[0]*stepSize,
										   currentPosition.y() - signal* collision->collisionDirection[1]*stepSize,
										   currentPosition.z() - signal* collision->collisionDirection[2]*stepSize);
	
	m_currentDistanceExploded += signal * (newPosition - currentPosition).length();
	
	m_osgTransform->setPosition(newPosition);


}


void Part::checkVisibility(osg::Vec3 eyePosition, std::vector< Part* >* partsGraph){

	int hitsCount = 0;
	for(int i=0; i<partsGraph->size(); i++){

		if(this != partsGraph->at(i) && partsGraph->at(i)->m_exploded == true){

			bool intercepted = false;
			for(int j=0; j<8; j++){
				
				osg::LineSegment* segment = new osg::LineSegment();
				segment->set(eyePosition,	osg::Vec3(m_boundingBox->corner(j))) ;

				partsGraph->at(i)->m_boundingBox->init();
				partsGraph->at(i)->m_boundingBox->expandBy(partsGraph->at(i)->m_osgTransform->computeBound());
				if(segment->intersect(*(partsGraph->at(i)->m_boundingBox))){
					hitsCount++;
					partsGraph->at(i)->move(partsGraph->at(i)->m_explosionDirection, 1, 0.1);
					intercepted = true;
				}
			}
			if(intercepted == false)
				partsGraph->at(i)->move(partsGraph->at(i)->m_explosionDirection, -1, 0.1);

		}
	}
	//cout << hitsCount;
	//cout << "\n";
	
	
	/*
	osg::Vec3 aux = osg::Vec3(m_boundingBox->corner(0));
	cout << aux.x() << aux.y() << aux.z();
	cout << "\n";
	cout << eyePosition.x() << eyePosition.y() << eyePosition.z();
	cout << "\n";
	*/
	/*
	cout << osg::Vec3(eyePosition - osg::Vec3(m_boundingBox->corner(0))).length();
	cout << "\n";
	*/

}