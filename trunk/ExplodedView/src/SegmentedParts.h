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


class SegmentedPart {

public:
	SegmentedPart(int, osg::Group* sceneRoot, Part* containerNode, osg::Vec3 normal);
	


private:
	osg::PositionAttitudeTransform* m_osgTransform;
	osg::Node* m_osgNode;
	
	

};

#endif