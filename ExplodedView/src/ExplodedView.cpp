#include "ExplodedView.h"


USE_OSGPLUGIN(3ds);
USE_GRAPHICSWINDOW();

ExplodedView::ExplodedView(){
	m_activeGraphRoot = new osg::Group();
	m_viewer = new osgViewer::Viewer();
	m_vcollide = new VCollide();
	
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

void ExplodedView::buildPartsGraph(char* modelName){

	

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



	///Find the parts
	FindNamedPartVisitor findNode("part_");

	loadedModel->accept(findNode);

	std::vector< osg::ref_ptr< Part > > nodeList = findNode.getPartList();
	
	for (int i=0; i<nodeList.size(); i++) {
		m_activeGraphRoot->addChild(nodeList[i]);

		nodeList[i]->setCollision(m_vcollide);
	}


	//Set the scene
	m_viewer->setSceneData(m_activeGraphRoot);



	//Collisions
	//Detect collisions

	//Detect a node without any collisions
	int activeNodes = 0;
	bool* collisions = new bool[nodeList.size()];
	for(int i=0; i<nodeList.size(); i++){
		collisions[i] = false;
	}

	while(activeNodes < nodeList.size()){
		

		VCReport report;
		m_vcollide->Collide( &report , VC_ALL_CONTACTS);


		for (int i = 0; i < report.numObjPairs(); i++){
			std::cout<<"Detected collision between objects " <<report.obj1ID(i) <<" and "<< report.obj2ID(i) <<"\n";
			collisions[i] = true;
		}

		for(int i=0; i<nodeList.size(); i++){
			collisions[i] = false;
		}
	}
	

	
	
	



}


void ExplodedView::loop(){
	
	while(!m_viewer->done()){
		m_viewer->frame();
	}

}


