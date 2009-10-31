#include "ExplodedView.h"


USE_OSGPLUGIN(3ds);
USE_GRAPHICSWINDOW();

ExplodedView::ExplodedView(){
	m_activeGraphRoot = new osg::Group();
	m_viewer = new osgViewer::Viewer();
	
}


void ExplodedView::setUp(){

    m_viewer->setCameraManipulator( new osgGA::TrackballManipulator() );

    // add the state manipulator
    m_viewer->addEventHandler( new osgGA::StateSetManipulator(m_viewer->getCamera()->getOrCreateStateSet()) );
    
    // add the thread model handler
    m_viewer->addEventHandler(new osgViewer::ThreadingHandler);

    // add the window size toggle handler
    m_viewer->addEventHandler(new osgViewer::WindowSizeHandler);
        
    // add the stats handler
    m_viewer->addEventHandler(new osgViewer::StatsHandler);

    // windowed
	m_viewer->setUpViewInWindow(50, 25, 1024, 768);

}

void ExplodedView::loadModel(char* modelName){

	

	// load the data
    osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFile(modelName);
    if (!loadedModel) 
    {
        std::cout << "No data loaded" << std::endl;
		exit(1);
    }


    // optimize the scene graph, remove redundant nodes and state etc.
    osgUtil::Optimizer optimizer;
    optimizer.optimize(loadedModel.get());

	
	//initial position
	/*
	osg::Vec3 position(0,0,0);
	osg::PositionAttitudeTransform* transform = new osg::PositionAttitudeTransform();

	transform->setPosition(position);


	m_activeGraphRoot->addChild(transform);
	*/
	m_activeGraphRoot->addChild(loadedModel);

	m_viewer->setSceneData(m_activeGraphRoot);






	///temp
	FindNamedNodeVisitor findNode("part_");

	loadedModel->accept(findNode);

	std::vector< osg::ref_ptr<osg::Node> > nodeList = findNode.getNodeList();
	
	for (int i=0; i<nodeList.size(); i++) {
		//nodeList[i]->setNodeMask(0x0);
	}



}

void ExplodedView::loop(){
	
	while(!m_viewer->done()){
		m_viewer->frame();
	}

}


