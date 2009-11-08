#ifndef GET_VERTICES_H
#define GET_VERTICES_H

//#include "PQP.H"
//#include "VCollide.H"

#include <osg/NodeVisitor>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/TriangleIndexFunctor>

#include <osgSim/DOFTransform>
#include <iostream>
#include <vector>
#include <iostream>
#include <string>
using namespace std;

class TriangleIndexVisitor

{
public:
	
	TriangleIndexVisitor(){
		m_VertexList = new vector<double>();
	}


            void operator() (int a, int b, int c)

            {
				int aux =0;

            }

			virtual void setVertexArray(unsigned int index,const osg::Vec3d* vertex){
				int aux =0;
				aux++;

				m_VertexList = NULL;

				for(int i=0; i<100; i++)
					aux++;

			}

 
	vector<double>* m_VertexList;

};

#endif

