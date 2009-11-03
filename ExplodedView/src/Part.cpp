#include "Part.h"


Part::Part(){
	m_inserted = false;
	m_pqpModel = new PQP_Model();
	m_osgTransform = new osg::PositionAttitudeTransform();
	m_ptrFirstProxyPart = NULL;
	m_ptrLastProxyPart = NULL;
	

	resetRestrictedMoviments();
}


void Part::setPQPModel(){
	m_pqpModel = new PQP_Model();
	m_inserted = false;

	m_pqpModel->BeginModel();

	AddTrianglesCollision triangles(m_pqpModel);

	m_osgNode->accept(triangles);

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
		m_collisions[i].clear();
		//m_collisions[i].collided = false;
		//m_collisions[i].collidedWith = NULL;
	}
}

void Part::explode(double stepSize){



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

double Part::calculateDistance(Part* compareTo, PQP_REAL translation_x, PQP_REAL translation_y, PQP_REAL translation_z){
	
	PQP_DistanceResult dres;
	double rel_err = 0.0, abs_err = 0.0;
	PQP_REAL R1[3][3],R2[3][3],T1[3],T2[3];
	R1[0][0] = R1[1][1] = R1[2][2] = 1.0;
	R1[0][1] = R1[1][0] = R1[2][0] = 0.0;
	R1[0][2] = R1[1][2] = R1[2][1] = 0.0;

	R2[0][0] = R2[1][1] = R2[2][2] = 1.0;
	R2[0][1] = R2[1][0] = R2[2][0] = 0.0;
	R2[0][2] = R2[1][2] = R2[2][1] = 0.0;

	T1[0] = translation_x;  T1[1] = translation_y; T1[2] = translation_z;
	T2[0] = 0.0;  T2[1] = 0.0; T2[2] = 0.0;
	PQP_Distance(&dres,R1,T1,m_pqpModel,R2,T2,compareTo->m_pqpModel,rel_err,abs_err, 100);

	return dres.Distance();

}

void Part::findDistancesOutBoundingBox(osgViewer::Viewer* viewer, double stepSize, double minimumDistance, bool visualize){
	
	double distance;
	double x, y, z;
	int count = 0;
	for(int i=0; i<6; i++){
		for(int j=0; j<m_collisions[i].size(); j++){
			if(m_collisions[i][j]->collided == true){
				bool insideBoundingBox = true;
				while(insideBoundingBox){
					x = - m_collisions[i][j]->collisionDirection[0] * count * stepSize;
					y = - m_collisions[i][j]->collisionDirection[1] * count * stepSize;
					z = - m_collisions[i][j]->collisionDirection[2] * count * stepSize;
					distance = calculateDistance(m_collisions[i][j]->collidedWith, x, y, z);

					if(visualize){
						m_osgTransform->setPosition(osg::Vec3d(x, y, z));
						viewer->frame();
					}

					if(distance > minimumDistance){
						insideBoundingBox = false;

						m_collisions[i][j]->distanceOutBoundingBox = distance;
					}
					count++;
				}
			}
		}
	}

}

void Part::checkCollisionsAlongAxis(osgViewer::Viewer* viewer, Part* compareTo, int x, int y, int z, double stepSize, int numIterations, double minimumDistance, bool visualize){
	double distance;
	for(int i=0; i<numIterations; i++){
		distance = calculateDistance(compareTo, i*stepSize*x, i*stepSize*y, i*stepSize*z);


		if(visualize){
			m_osgTransform->setPosition(osg::Vec3d(i*stepSize*x, i*stepSize*y, i*stepSize*z));
			viewer->frame();
		}


		if(distance < minimumDistance){
			CollisionData* aux = new CollisionData();
			aux->collided = true;
			aux->collidedWith = compareTo;
			aux->collisionDirection[0] = x; 
			aux->collisionDirection[1] = y; 
			aux->collisionDirection[2] = z; 
			if(x == 1){
				m_collisions[0].push_back(aux);
			}
			else if(x == -1){
				m_collisions[1].push_back(aux);
			}
			else if(y == 1){
				m_collisions[2].push_back(aux);	
			}
			else if(y == -1){
				m_collisions[3].push_back(aux);	
			}
			else if(z == 1){
				m_collisions[4].push_back(aux);	
			}
			else if(z == -1){
				m_collisions[5].push_back(aux);
			}



			m_countRestrictedDirections++;

			return;
		}

	}
}