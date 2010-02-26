#ifndef Part_H
#define Part_H


#include "Cell.h"


class Part{

public:
	Part();


private:
	boost::multi_array<Cell*, 3> mCellMatrix;



};


#endif