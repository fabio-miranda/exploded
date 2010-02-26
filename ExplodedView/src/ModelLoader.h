#ifndef ModelLoader_H
#define ModelLoader_H


#include "Cell.h"

#include <fstream>
#include <iostream>


#include <osg/Geometry>

#include <boost/multi_array.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/shared_ptr.hpp>


class ModelLoader{

public:
	static void Geresim(char* pModelName, osg::Geometry* pNode, std::vector<Cell*>* pCellMatrix);

};



#endif


