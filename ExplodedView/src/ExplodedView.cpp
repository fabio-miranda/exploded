#include "ExplodedView.h"



ExplodedView::ExplodedView(){

	ModelLoader::Geresim("namorado_so.gmdl", mGeometry, mCellArray);
	setUpOSG();
	findParts();
}

void ExplodedView::setUpOSG(){
	mViewer = new osgViewer::Viewer();

	mViewer->addEventHandler( new osgGA::StateSetManipulator(mViewer->getCamera()->getOrCreateStateSet()) );
	mViewer->addEventHandler(new osgViewer::ThreadingHandler);
	mViewer->addEventHandler(new osgViewer::WindowSizeHandler);
	mViewer->addEventHandler(new osgViewer::StatsHandler);

	mViewer->setUpViewInWindow(50, 25, 1280, 720);
	mViewer->getCamera()->setClearColor(osg::Vec4(0,0,0,0));

	osg::setNotifyLevel(osg::NOTICE);
	
	osgUtil::Optimizer optimizer;
	//optimizer.optimize(m_sceneGraphRoot, optimizer.CHECK_GEOMETRY);

	mViewer->setCameraManipulator( new osgGA::TrackballManipulator() );
	mViewer->getCameraManipulator()->setHomePosition(osg::Vec3d(2500, 3500, -3500), osg::Vec3d(1500, 1500, 3242), osg::Vec3d(0,0,-1));
}

void ExplodedView::findParts(){

	for(unsigned int i=0; i < mCellArray->size(); i++){

		
	}


}

void ExplodedView::run(){


}