#ifndef GET_VERTICES_H
#define GET_VERTICES_H

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

class TriangleVisitor

{
public:
	
	TriangleVisitor(){
		m_VertexList = new vector<double>();
	}


            void operator() (const osg::Vec3d& v1, const osg::Vec3d& v2, const osg::Vec3d& v3, bool)

            {

						m_VertexList->push_back(v1[0]);

                        m_VertexList->push_back(v1[1]);

                        m_VertexList->push_back(v1[2]);



                        m_VertexList->push_back(v2[0]);

                        m_VertexList->push_back(v2[1]);

                        m_VertexList->push_back(v2[2]);

 

                        m_VertexList->push_back(v3[0]);

                        m_VertexList->push_back(v3[1]);

                        m_VertexList->push_back(v3[2]);

            }

 
	vector<double>* m_VertexList;

};

#endif

