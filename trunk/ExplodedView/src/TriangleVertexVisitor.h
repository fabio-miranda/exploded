#ifndef TRIANGLEVERTEXVISITOR_H
#define TRIANGLEVERTEXVISITOR_H

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


class TriangleVertexVisitor
{
public:
	vector<double*>* m_verticesArray;

	TriangleVertexVisitor(){
		m_verticesArray = new vector<double*>();
	}

	void operator() (const osg::Vec3& v1, const osg::Vec3& v2, const osg::Vec3& v3, bool)
	{
		double* vertice1 = new double[3];
		double* vertice2 = new double[3];
		double* vertice3 = new double[3];
		vertice1[0] = v1.x(); vertice1[1] = v1.y(); vertice1[2] = v1.z();
		vertice2[0] = v2.x(); vertice2[1] = v2.y(); vertice2[2] = v2.z();
		vertice3[0] = v3.x(); vertice3[1] = v3.y(); vertice3[2] = v3.z();
		m_verticesArray->push_back(vertice1);
		m_verticesArray->push_back(vertice2);
		m_verticesArray->push_back(vertice3);

	}


	
};

#endif

