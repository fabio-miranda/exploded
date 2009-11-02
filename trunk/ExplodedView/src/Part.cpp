#include "Part.h"



void Part::setCollision(VCollide* vcollide){

	vcollide->NewObject(&m_vcollideId);

	AddTrianglesCollision triangles(vcollide);

	this->accept(triangles);

	vcollide->EndObject();
	
	

}