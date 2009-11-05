#ifndef GET_VERTICES_H
#define GET_VERTICES_H

//#include "PQP.H"
#include "VCollide.H"

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

class AddTrianglesCollision : public osg::NodeVisitor
{
public:
    AddTrianglesCollision(VCollide* vCollide):
	  osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN){
		m_vCollide = vCollide;
	  }
    
	virtual void apply(osg::Geode& geode)
    {
		osg::ref_ptr <osg::Geometry> geometry;
		osg::ref_ptr <osg::Vec3Array> vertexVector;
		osg::Vec3 vertex1, vertex2, vertex3;
		double v1[3], v2[3], v3[3];

		
		geometry = (osg::Geometry *) geode.getDrawable(0);
		vertexVector = (osg::Vec3Array *) geometry->getVertexArray();

		//int bla = geometry->getPrimitiveSet(0)->getMode();

		//osg::PrimitiveSet::Mode::TRIANGLES
		
		int aux = 0;
		for(int i=0; i< vertexVector->size(); i+=3){
			vertex1 = vertexVector->at(i);
			vertex2 = vertexVector->at(i+1);
			vertex3 = vertexVector->at(i+2);

			for(int j=0; j<3; j++){
				v1[j] = vertex1[j];
				v2[j] = vertex2[j];
				v3[j] = vertex3[j];
			}

			m_vCollide->AddTri(v1, v2, v3,aux);
			aux++;
		}

		traverse(geode);

    }

private:
	VCollide* m_vCollide;
};

#endif

