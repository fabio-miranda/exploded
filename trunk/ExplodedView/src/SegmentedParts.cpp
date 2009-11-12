#include "SegmentedParts.h"



SegmentedParts::SegmentedParts(int clipPlaneNum, osg::Group* sceneGraphRoot, Part* containerPart, osg::Vec3 normal){

	osg::CopyOp copyop = osg::CopyOp();
	osg::Vec3d bbCenter = osg::Vec3d(containerPart->m_boundingBox->center());

	//First segment
	m_normal1 = new osg::Vec3d(normal);
	m_osgTransform1 = new osg::PositionAttitudeTransform(*(containerPart->m_osgTransform), copyop.SHALLOW_COPY);
	m_osgNode1 = new osg::Node(*(containerPart->m_osgNode), copyop.DEEP_COPY_PRIMITIVES);
	createSegment(clipPlaneNum, sceneGraphRoot, m_osgTransform1, m_osgNode1, containerPart, m_normal1, bbCenter);

	//Second segment
	/*
	m_normal2 = new osg::Vec3d(-normal);
	m_osgTransform2 = new osg::PositionAttitudeTransform(*(containerPart->m_osgTransform), copyop.SHALLOW_COPY);
	m_osgNode2 = new osg::Node(*(containerPart->m_osgNode), copyop.SHALLOW_COPY);
	createSegment(clipPlaneNum+1, sceneGraphRoot, m_osgTransform2, m_osgNode2, containerPart, m_normal2, bbCenter);
	*/



}

void SegmentedParts::createSegment(int clipPlaneNum, osg::Group* sceneGraphRoot, osg::PositionAttitudeTransform* transform, osg::Node* node,
								   Part* containerPart, osg::Vec3d* normal, osg::Vec3d bbCenter){
	
									   
	
	node->setNodeMask(0x1);

	osg::ClipNode* clipNode = new osg::ClipNode;
	osg::ClipPlane* clipPlane = new osg::ClipPlane(clipPlaneNum, osg::Plane(*normal,bbCenter));
	
	clipNode->addClipPlane(clipPlane);
	clipNode->setCullingActive(false);

	
	transform->setPosition(osg::Vec3d(normal->x()*100, normal->y()*100, normal->z()*100));
	sceneGraphRoot->addChild(transform);
	transform->addChild(clipNode);
	clipNode->addChild(node);
	
	

}


double SegmentedParts::explodeUntilVisible(osgViewer::Viewer* viewer, double stepSize, Part* partToBeVisible){

	//while(partToBeVisible->m_osgOcclusionQueryNode->getPassed() == false){
	for(int i=0; i<1000; i++){
		
		//m_osgTransform1->setPosition(m_osgTransform1->getPosition() + ((*m_normal1) * stepSize));
		//m_osgTransform2->setPosition(m_osgTransform2->getPosition() + ((*m_normal2) * stepSize));

		viewer->frame();
	}

	return 0;

}