#ifndef TRIANGLEINDEXVISITOR_H
#define TRIANGLEINDEXVISITOR_H

//#include "PQP.H"
//#include "VCollide.H"

#include <osg/NodeVisitor>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>

#include <osgSim/DOFTransform>
#include <iostream>
#include <vector>
#include <iostream>
#include <string>
using namespace std;

class TriangleIndexVisitor
{
public:
	vector<int>* m_indicesArray;
	
	TriangleIndexVisitor(){
		m_indicesArray = new vector<int>();
	}


	void operator() (int a, int b, int c)
	{
		m_indicesArray->push_back(a);
		m_indicesArray->push_back(b);
		m_indicesArray->push_back(c);

	}

 
	

};

#endif

