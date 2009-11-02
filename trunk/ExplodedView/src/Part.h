#ifndef PART_H
#define PART_H

#include "AddTrianglesCollision.h"


#include "VCollide.H"

//osg
#include <osg/Node>


class Part : public osg::Node{

public:
	Part();
	void setCollision(VCollide* vcollide);


private:
	int m_vcollideId;

};

#endif