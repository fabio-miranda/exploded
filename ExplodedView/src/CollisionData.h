#ifndef COLLISIONDATA_H
#define COLLISIONDATA_H

#include "Part.h"

//Forward declaration
class Part;

class CollisionData{
	
public:
	CollisionData(){
	}

	bool collided;
	Part* collidedWith;
	double collisionDirection[3];
	double distanceOutBoundingBox;

};

#endif