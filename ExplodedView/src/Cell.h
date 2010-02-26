#ifndef Cell_H
#define Cell_H

#include <osg/Vec3>

#include <vector>

class Cell{

public:
	Cell(bool pActive, osg::Vec3* pIndex, int* pVerticesIndex, int* pNeighboursIndex, float pProp);
	bool isActive();
	bool isOnAPart();
	bool isOnEdge();

private:
	bool mActive;
	bool mIsOnAPart;
	bool mIsOnEdge;
	osg::Vec3* mIndex;
	int* mVerticesIndex;
	int* mNeighboursIndex;
	float mProp;


};


#endif