#include "Cell.h"


Cell::Cell(bool pActive, osg::Vec3* pIndex, int* pVerticesIndex, int* pNeighboursIndex, float pProp){

	mActive = pActive;
	mIndex = pIndex;
	mVerticesIndex = pVerticesIndex;
	mNeighboursIndex = pNeighboursIndex;
	mProp = pProp;

	mIsOnAPart = false;
	mIsOnEdge = false;

}


bool Cell::isActive(){
	return mActive;
}

bool Cell::isOnAPart(){
	return mIsOnAPart;
}

bool Cell::isOnEdge(){
	return mIsOnEdge;
}