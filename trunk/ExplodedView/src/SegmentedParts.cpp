#include "SegmentedParts.h"



SegmentedPart::SegmentedPart(int clipPlaneNum, osg::Group* sceneRoot, Part* containerPart, osg::Vec3 normal){

	osg::CopyOp* copyop = new osg::CopyOp();
	m_osgTransform = new osg::PositionAttitudeTransform(*(containerPart->m_osgTransform), copyop->SHALLOW_COPY);
	m_osgNode = (osg::Node*) containerPart->m_osgNode->clone(copyop->SHALLOW_COPY);
	m_osgNode->setNodeMask(1);

	//osg::Group* parent = containerPart->m_osgNode->getParent(0);
	osg::Vec3d bbCenter = osg::Vec3d(containerPart->m_boundingBox->center());


	osg::ClipNode* clipNode = new osg::ClipNode;
	osg::ClipPlane* clipPlane = new osg::ClipPlane(clipPlaneNum, osg::Plane(normal,bbCenter));
	
	

	clipNode->addClipPlane(clipPlane);
	clipNode->setCullingActive(false);



	m_osgTransform->setPosition(osg::Vec3d(normal.x()*100, normal.y()*100, normal.z()*100));
	sceneRoot->addChild(m_osgTransform);
	m_osgTransform->addChild(clipNode);
	clipNode->addChild(m_osgNode);


}