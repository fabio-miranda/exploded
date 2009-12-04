#include "SegmentedParts.h"



SegmentedParts::SegmentedParts(int clipPlaneNum, osg::Group* sceneGraphRoot, Part* containerPart, osg::Vec3 normal){

	m_exploded = false;
	m_inploded = true;
	m_currentDistanceExploded = 0;
	m_parent = sceneGraphRoot;
	osg::CopyOp copyop = osg::CopyOp();
	osg::Vec3d bbCenter = osg::Vec3d(containerPart->m_boundingBox->center());

	//First segment
	m_normal1 = new osg::Vec3d(normal);
	m_osgTransform1 = new osg::PositionAttitudeTransform(*(containerPart->m_osgTransform), copyop.SHALLOW_COPY);
	m_osgNode1 = (osg::Node*) containerPart->m_osgNode->clone(copyop.SHALLOW_COPY);
	//TODO: improve the bounding box
	m_boundingBox1 = new osg::BoundingBox();
	m_boundingBox1->expandBy(m_osgNode1->getBound());
	createSegment(clipPlaneNum, sceneGraphRoot, m_osgTransform1, m_osgNode1, containerPart, m_normal1, bbCenter);

	//Second segment
	m_normal2 = new osg::Vec3d(-normal);
	m_osgTransform2 = new osg::PositionAttitudeTransform(*(containerPart->m_osgTransform), copyop.SHALLOW_COPY);
	m_osgNode2 = (osg::Node*) containerPart->m_osgNode->clone(copyop.SHALLOW_COPY);
	//TODO: improve the bounding box
	m_boundingBox2 = new osg::BoundingBox();
	m_boundingBox2->expandBy(m_osgNode2->getBound());
	createSegment(clipPlaneNum+1, sceneGraphRoot, m_osgTransform2, m_osgNode2, containerPart, m_normal2, bbCenter);

	



}

SegmentedParts::~SegmentedParts(){
	m_osgTransform1->removeChild(0, m_osgTransform1->getNumChildren());
	m_parent->removeChild(m_osgTransform1);
	//m_osgTransform1->unref();
	m_osgTransform2->removeChild(0, m_osgTransform2->getNumChildren());
	m_parent->removeChild(m_osgTransform2);
	//m_osgTransform2->unref();


}

void SegmentedParts::createSegment(int clipPlaneNum, osg::Group* sceneGraphRoot, osg::PositionAttitudeTransform* transform, osg::Node* node,
								   Part* containerPart, osg::Vec3d* normal, osg::Vec3d bbCenter){
	
									   
	
	node->setNodeMask(0x1);

	osg::ClipNode* clipNode = new osg::ClipNode;
	osg::ClipPlane* clipPlane = new osg::ClipPlane(clipPlaneNum, osg::Plane(*normal,bbCenter));
	
	clipNode->addClipPlane(clipPlane);
	clipNode->setCullingActive(false);

	
	//transform->setPosition(osg::Vec3d(normal->x()*100, normal->y()*100, normal->z()*100));
	sceneGraphRoot->addChild(transform);
	transform->addChild(clipNode);
	clipNode->addChild(node);
	
	

}


double SegmentedParts::explodeUntilVisible(osgViewer::Viewer* viewer, double stepSize, Part* partToBeVisible){

	//TODO: to test the visibility, cast a ray from the camera eye to each corner of the partToBeVisible bounding box
	//(just like a piker)
	
	double distance = 0;
	bool visible = partToBeVisible->m_osgOcclusionQueryNode->getPassed();
	bool intersectsBB1 = m_boundingBox1->intersects(*(partToBeVisible->m_boundingBox));
	bool intersectsBB2 = m_boundingBox2->intersects(*(partToBeVisible->m_boundingBox));
	while(visible == false || intersectsBB1 == true || intersectsBB2 == true){
		
		m_osgTransform1->setPosition(m_osgTransform1->getPosition() + ((*m_normal1) * stepSize));
		m_osgTransform2->setPosition(m_osgTransform2->getPosition() + ((*m_normal2) * stepSize));
		distance += stepSize;

		viewer->frame();

		visible = partToBeVisible->m_osgOcclusionQueryNode->getPassed();
		//m_osgNode1->dirtyBound();
		m_boundingBox1->init();
		m_boundingBox1->expandBy(m_osgTransform1->computeBound());
		//m_osgNode2->dirtyBound();
		m_boundingBox2->init();
		m_boundingBox2->expandBy(m_osgTransform2->computeBound());
		intersectsBB1 = m_boundingBox1->intersects(*(partToBeVisible->m_boundingBox));
		intersectsBB2 = m_boundingBox2->intersects(*(partToBeVisible->m_boundingBox));

	}

	m_distanceToExplode = distance;

	return distance;

}

void SegmentedParts::explode(double stepSize){

	if(m_exploded) return;

	


	if(m_currentDistanceExploded < m_distanceToExplode){

		osg::Vec3d oldPosition1 = m_osgTransform1->getPosition();
		osg::Vec3d oldPosition2 = m_osgTransform1->getPosition();

		osg::Vec3d newPosition1 = m_osgTransform1->getPosition() + ((*m_normal1) * stepSize);
		osg::Vec3d newPosition2 = m_osgTransform2->getPosition() + ((*m_normal2) * stepSize);

		m_osgTransform1->setPosition(newPosition1);
		m_osgTransform2->setPosition(newPosition2);
	
		m_currentDistanceExploded += (newPosition1 - oldPosition1).length();
	}
	else{
		m_exploded = true;
		m_inploded = false;
	}

}

void SegmentedParts::inplode(double stepSize){

	if(m_inploded) return;

	


	if(m_currentDistanceExploded > 0){

		osg::Vec3d oldPosition1 = m_osgTransform1->getPosition();
		osg::Vec3d oldPosition2 = m_osgTransform1->getPosition();

		osg::Vec3d newPosition1 = m_osgTransform1->getPosition() - ((*m_normal1) * stepSize);
		osg::Vec3d newPosition2 = m_osgTransform2->getPosition() - ((*m_normal2) * stepSize);

		m_osgTransform1->setPosition(newPosition1);
		m_osgTransform2->setPosition(newPosition2);
	
		m_currentDistanceExploded -= (newPosition1 - oldPosition1).length();
	}
	else{
		m_currentDistanceExploded = 0;

		m_inploded = true;
		m_exploded = false;
	}


}

void SegmentedParts::move(CollisionData* collision, int signal, double stepSize){

	if(m_currentDistanceExploded <= stepSize && signal == -1){
		m_currentDistanceExploded = 0;
		return;
	}

	
	
	osg::Vec3d newPosition1 = m_osgTransform1->getPosition() +  ((*m_normal1) * stepSize * signal);
	osg::Vec3d newPosition2 = m_osgTransform2->getPosition() + ((*m_normal2) * stepSize * signal);

	m_currentDistanceExploded += signal * (newPosition1 - m_osgTransform1->getPosition()).length();
	
	m_osgTransform1->setPosition(newPosition1);
	m_osgTransform2->setPosition(newPosition2);


}