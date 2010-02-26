#ifndef CELL_H
#define CELL_H

#include <osg/Node>
#include <osg/Geometry>

class Cell{
	
public:
	Cell(bool active, osg::Vec3* cellIndex, std::vector<int>* verticesIndex, std::vector<int>* neighboursIndex, float prop);
	void addPrimitiveSets(osg::Geometry* geometry);

	bool mActive;
	osg::Vec3* mCellIndex;
	std::vector<int>* mVerticesIndex;
	std::vector<int>* mNeighboursIndex;

	float mProp;

};

#endif