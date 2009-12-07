#ifndef COLLISIONDATA_H
#define COLLISIONDATA_H

#include "Part.h"

//Forward declaration
class Part;

class CollisionData{
	
public:
	CollisionData(){
		collidedWith = NULL;
		collided = false;
		distanceOutBoundingBox = 0;
	}

	bool collided;
	Part* collidedWith;
	double collisionDirection[3];
	double distanceOutBoundingBox;

};

#endif