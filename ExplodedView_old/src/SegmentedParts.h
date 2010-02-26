#ifndef SEGMENTEDPART_H
#define SEGMENTEDPART_H


#include "Part.h"

//osg
#include <osg/Node>
#include <osg/PositionAttitudeTransform>
#include <osgViewer/Viewer>
#include <osg/TriangleIndexFunctor>
#include <osg/TriangleFunctor>
#include <osg/OcclusionQueryNode>
#include <osg/ClipPlane>
#include <osg/ClipNode>


class SegmentedParts {

public:
	SegmentedParts(int, osg::Group* sceneGraphRoot, Part* containerPart, osg::Vec3 normal);
	~SegmentedParts();
	void createSegment(int, osg::Group* sceneGraphRoot, osg::PositionAttitudeTransform* transform, osg::Node* node, Part* containerPart, osg::Vec3d* normal, osg::Vec3d bbCenter);
	double explodeUntilVisible(osgViewer::Viewer* viewer, double stepSize, Part* partToBeVisible);
	void explode(double);
	void inplode(double);
	void move(CollisionData* collision, int signal, double stepSize);
	



	osg::PositionAttitudeTransform* m_osgTransform1;
	osg::Node* m_osgNode1;
	osg::Vec3d* m_normal1;

	osg::Group* m_parent;
	osg::PositionAttitudeTransform* m_osgTransform2;
	osg::Node* m_osgNode2;
	osg::Vec3d* m_normal2;
	double m_currentDistanceExploded;
	double m_distanceToExplode;
	bool m_exploded;
	bool m_inploded;
	osg::BoundingBox* m_boundingBox1;
	osg::BoundingBox* m_boundingBox2;
	
private:	

};

#endif