#include "Cell.h"


Cell::Cell(bool active, osg::Vec3* cellIndex, std::vector<int>* verticesIndex, std::vector<int>* neighboursIndex, float prop){

	mActive = active;
	mCellIndex = cellIndex;
	mVerticesIndex = verticesIndex;
	mNeighboursIndex = neighboursIndex;
	mProp = (int) (prop * 10); //first decimal after the dot

}


void Cell::addPrimitiveSets(osg::Geometry* geometry){

	//Cube faces
	//Top
	
	{
		osg::DrawElementsUInt* primitiveSet = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);

		primitiveSet->push_back(mVerticesIndex->at(0));
		primitiveSet->push_back(mVerticesIndex->at(1));
		primitiveSet->push_back(mVerticesIndex->at(3));
		primitiveSet->push_back(mVerticesIndex->at(2));
		geometry->addPrimitiveSet(primitiveSet);

	}
	
	
	//Bottom
	{
		osg::DrawElementsUInt* primitiveSet = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
		primitiveSet->push_back(mVerticesIndex->at(6));
		primitiveSet->push_back(mVerticesIndex->at(7));
		primitiveSet->push_back(mVerticesIndex->at(5));
		primitiveSet->push_back(mVerticesIndex->at(4));
		geometry->addPrimitiveSet(primitiveSet);
	}
	//Left
	{
		osg::DrawElementsUInt* primitiveSet = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
		primitiveSet->push_back(mVerticesIndex->at(1));
		primitiveSet->push_back(mVerticesIndex->at(3));
		primitiveSet->push_back(mVerticesIndex->at(7));
		primitiveSet->push_back(mVerticesIndex->at(5));
		geometry->addPrimitiveSet(primitiveSet);
	}
	//Right
	{
		osg::DrawElementsUInt* primitiveSet = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
		primitiveSet->push_back(mVerticesIndex->at(0));
		primitiveSet->push_back(mVerticesIndex->at(2));
		primitiveSet->push_back(mVerticesIndex->at(6));
		primitiveSet->push_back(mVerticesIndex->at(4));
		geometry->addPrimitiveSet(primitiveSet);
	}
	{
		osg::DrawElementsUInt* primitiveSet = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
		primitiveSet->push_back(mVerticesIndex->at(1));
		primitiveSet->push_back(mVerticesIndex->at(5));
		primitiveSet->push_back(mVerticesIndex->at(4));
		primitiveSet->push_back(mVerticesIndex->at(0));
		geometry->addPrimitiveSet(primitiveSet);
	}
	{
		osg::DrawElementsUInt* primitiveSet = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
		primitiveSet->push_back(mVerticesIndex->at(2));
		primitiveSet->push_back(mVerticesIndex->at(3));
		primitiveSet->push_back(mVerticesIndex->at(7));
		primitiveSet->push_back(mVerticesIndex->at(6));
		geometry->addPrimitiveSet(primitiveSet);

	}
	


/*
   4_____ 7
   /     /|
6 /____5/ |
  |0 _ |_ |3
  | /  |  |
  |/___| / 
 2	    1
 */
}