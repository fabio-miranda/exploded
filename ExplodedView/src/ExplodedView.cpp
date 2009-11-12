#include "ExplodedView.h"


#define MINIMUM_DISTANCE_TO_CONSIDER_CONTACT 0
#define STEPSIZE 0.1
#define ITERATIONS 500
#define VISUALIZE_GRAPH_BUILDING true
#define PRINT_GRAPH true


USE_OSGPLUGIN(3ds);
USE_GRAPHICSWINDOW();

ExplodedView::ExplodedView(){
	m_sceneGraphRoot = new osg::Group();
	m_viewer = new osgViewer::Viewer();
	//m_vCollide = new VCollide();
}

 
void ExplodedView::setUp(char* modelName){

    m_viewer->setCameraManipulator( new osgGA::TrackballManipulator() );
	m_viewer->getCameraManipulator()->setHomePosition(osg::Vec3d(400,400,400), osg::Vec3d(0,0,0), osg::Vec3d(0,0,1));

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

	//box
	buildBox();

	//model
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
	FindNamedPartVisitor findParts("part_");

	loadedModel->accept(findParts);
	m_partsGraph = findParts.getPartList();
	
	
	for (int i=0; i<m_partsGraph.size(); i++) {
		//m_sceneGraphRoot->addChild(m_partsGraph[i]->getOSGNode());
		//m_partsGraph[i]->setUp(m_vCollide, m_sceneGraphRoot);
		m_partsGraph[i]->setUp(m_sceneGraphRoot);
	}

	//Set the scene
	m_viewer->setSceneData(m_sceneGraphRoot);

}

void ExplodedView::buildPartsGraph(){

	

	
	


	//Detect collisions
	int insertedParts = 0;
	
	findBlockedDirections();

	while(insertedParts < m_partsGraph.size()){
		
		/*
		for(int i=0; i<m_partsGraph.size(); i++){
			if(m_partsGraph[i]->m_inserted == false){
				m_partsGraph[i]->resetRestrictedMoviments();
			}
		}
		*/

		//Find the directions in which the parts are blocked or not
		//TODO: only do it once. After one iteration, we already know every possible collision between the parts
		findBlockedDirections();

		//Find the distance that the parts have to walk in order to get out of the bounding box
		//calculateDistancesOutBB();
		//this is done on the findBlockedDirections
		
		countBlockedDirections();

		//Find the smallest distance and insert it on the graph
		Part* aux = findSmallestDistanceOutBoundingBox();
		
		findContainers();
		
		//m_vCollide->DeactivateObject(aux->m_vcollideId);
		if(aux != NULL){
			insertOnPartsGraph(aux);
		}
		else{
			//Check for interlocked cases
			for(int i=0; i<m_partsGraph.size(); i++){
				if(m_partsGraph[i]->m_container){
					//Interactive explode the container on each of the 6 directions and find the smallest distance
					//in order to make the interlocked node visible
					m_partsGraph[i]->split(m_sceneGraphRoot, m_viewer, STEPSIZE);
				}

			}
		}

		insertedParts++;
		
	}

	if(PRINT_GRAPH)
		printGraph();

	//Reset positions
	for(int i=0; i<m_partsGraph.size(); i++){
		m_partsGraph[i]->m_osgTransform->setPosition(osg::Vec3d(0, 0, 0));
	}
}

void ExplodedView::findBlockedDirections(){
	for(int i=0; i<m_partsGraph.size(); i++){

		if(m_partsGraph[i]->m_inserted == false){
			/*
			m_partsGraph[i]->checkCollisionsAlongAxis(m_viewer, m_vCollide, m_partsGraph, 1, 0, 0, STEPSIZE, ITERATIONS, MINIMUM_DISTANCE_TO_CONSIDER_CONTACT, VISUALIZE_GRAPH_BUILDING);
			m_partsGraph[i]->checkCollisionsAlongAxis(m_viewer, m_vCollide, m_partsGraph, -1, 0, 0, STEPSIZE, ITERATIONS, MINIMUM_DISTANCE_TO_CONSIDER_CONTACT, VISUALIZE_GRAPH_BUILDING);
			m_partsGraph[i]->checkCollisionsAlongAxis(m_viewer, m_vCollide, m_partsGraph, 0, 1, 0, STEPSIZE, ITERATIONS, MINIMUM_DISTANCE_TO_CONSIDER_CONTACT, VISUALIZE_GRAPH_BUILDING);
			m_partsGraph[i]->checkCollisionsAlongAxis(m_viewer, m_vCollide, m_partsGraph, 0, -1, 0, STEPSIZE, ITERATIONS, MINIMUM_DISTANCE_TO_CONSIDER_CONTACT, VISUALIZE_GRAPH_BUILDING);
			m_partsGraph[i]->checkCollisionsAlongAxis(m_viewer, m_vCollide, m_partsGraph, 0, 0, 1, STEPSIZE, ITERATIONS, MINIMUM_DISTANCE_TO_CONSIDER_CONTACT, VISUALIZE_GRAPH_BUILDING);
			m_partsGraph[i]->checkCollisionsAlongAxis(m_viewer, m_vCollide, m_partsGraph, 0, 0, -1, STEPSIZE, ITERATIONS, MINIMUM_DISTANCE_TO_CONSIDER_CONTACT, VISUALIZE_GRAPH_BUILDING);
			*/
			
			m_partsGraph[i]->checkCollisionsAlongAxis(m_viewer, m_partsGraph, 1, 0, 0, STEPSIZE, ITERATIONS, MINIMUM_DISTANCE_TO_CONSIDER_CONTACT, VISUALIZE_GRAPH_BUILDING);
			m_partsGraph[i]->checkCollisionsAlongAxis(m_viewer, m_partsGraph, -1, 0, 0, STEPSIZE, ITERATIONS, MINIMUM_DISTANCE_TO_CONSIDER_CONTACT, VISUALIZE_GRAPH_BUILDING);
			m_partsGraph[i]->checkCollisionsAlongAxis(m_viewer, m_partsGraph, 0, 1, 0, STEPSIZE, ITERATIONS, MINIMUM_DISTANCE_TO_CONSIDER_CONTACT, VISUALIZE_GRAPH_BUILDING);
			m_partsGraph[i]->checkCollisionsAlongAxis(m_viewer, m_partsGraph, 0, -1, 0, STEPSIZE, ITERATIONS, MINIMUM_DISTANCE_TO_CONSIDER_CONTACT, VISUALIZE_GRAPH_BUILDING);
			m_partsGraph[i]->checkCollisionsAlongAxis(m_viewer, m_partsGraph, 0, 0, 1, STEPSIZE, ITERATIONS, MINIMUM_DISTANCE_TO_CONSIDER_CONTACT, VISUALIZE_GRAPH_BUILDING);
			m_partsGraph[i]->checkCollisionsAlongAxis(m_viewer, m_partsGraph, 0, 0, -1, STEPSIZE, ITERATIONS, MINIMUM_DISTANCE_TO_CONSIDER_CONTACT, VISUALIZE_GRAPH_BUILDING);
			
		}

		m_partsGraph[i]->m_osgTransform->setPosition(osg::Vec3d(0, 0, 0));
	}
}


Part* ExplodedView::findSmallestDistanceOutBoundingBox(){
	
	double smallestDistance = std::numeric_limits<double>::infinity();
	double currentDistance;

	Part* aux = NULL;

	for(int i=0; i<m_partsGraph.size(); i++){

		if(m_partsGraph[i]->m_inserted == false){
			currentDistance = m_partsGraph[i]->findSmallestDistanceOutBoundingBox();

			if(currentDistance <= smallestDistance && m_partsGraph[i]->m_countRestrictedDirections < 6){
				aux = m_partsGraph[i];
			}
			
		}
	}

	return aux;
}

void ExplodedView::countBlockedDirections(){

	for(int i=0; i<m_partsGraph.size(); i++){
		m_partsGraph[i]->countBlockedDirections();
	}

}

void ExplodedView::findContainers(){

	//Find interlocked parts
	for(int i=0; i<m_partsGraph.size(); i++){
		m_partsGraph[i]->findContainer();
	}
	

}


void ExplodedView::insertOnPartsGraph(Part* part){
	part->m_inserted = true;

	for(int i=0; i<6; i++){
		for(int j=0; j<part->m_allDistanceCollisions[i].size(); j++){
			if(part->m_allDistanceCollisions[i][j]->collided && part->m_allDistanceCollisions[i][j]->collidedWith->m_inserted == false){
				part->insertVertexFrom(part->m_allDistanceCollisions[i][j]->collidedWith);
			}
		}
		
	}
}

void ExplodedView::printGraph(){

	for(int i=0; i<m_partsGraph.size(); i++){
		std::cout << m_partsGraph[i]->m_osgNode->getName() << "-->";
		ProxyPart* currentProxyPart = m_partsGraph[i]->m_ptrFirstProxyPart;
		while(currentProxyPart != NULL){
			cout << currentProxyPart->m_ptrActualPart->m_osgNode->getName() << "-->";
			currentProxyPart = currentProxyPart->m_ptrNextProxyPart;
		}

		
		if(m_partsGraph[i]->m_explosionDirection != NULL){ //if it is equal to NULL, then we have the parts graph root (it doesn't move)
			double* collisionDirection = m_partsGraph[i]->m_explosionDirection->collisionDirection;
			cout << "\n";
			cout << ":: Explosion direction: "<< collisionDirection[0] << ", " << collisionDirection[1] << ", " << collisionDirection[2];
			cout << "\n:: Distance: "<< m_partsGraph[i]->m_explosionDirection->distanceOutBoundingBox;
		}

		cout << "\n\n";
	}

}


void ExplodedView::explode(){
	bfs(m_partsGraph[0]);
}

void ExplodedView::bfs(Part* v){

	m_explodingLevels = new std::vector< std::vector<Part*> >();

	std::list< Part* > queue;
	v->m_visited = true;
	queue.push_back(v);
	m_explodingLevels->push_back(std::vector<Part*>());
	m_explodingLevels->back().push_back(v);


	while(queue.size() > 0){
		Part* v = queue.front();
		queue.pop_front();
		m_explodingLevels->push_back(std::vector<Part*>());
		ProxyPart* vNeighbour = v->m_ptrFirstProxyPart;

		while(vNeighbour != NULL){

			if(vNeighbour->m_ptrActualPart->m_visited == false){
				queue.push_back(vNeighbour->m_ptrActualPart);
				vNeighbour->m_ptrActualPart->m_visited = true;
				m_explodingLevels->back().push_back(vNeighbour->m_ptrActualPart);
			}

			vNeighbour = vNeighbour->m_ptrNextProxyPart;
		}

		if(m_explodingLevels->back().size() == 0)
			m_explodingLevels->pop_back();
	}

}

void ExplodedView::updateExplodingParts(){
	
	bool allFromLevelExploded = true;
	//Explode the leaf nodes
	if((*m_explodingLevels).size() > 1){ //do not explode the root node
		for(int j=0; j<(*m_explodingLevels).back().size(); j++){ 
			(*m_explodingLevels).back()[j]->explode(0.1);
			m_viewer->frame();

			if((*m_explodingLevels).back()[j]->m_exploded == false)
				allFromLevelExploded = false;
		}

		if(allFromLevelExploded)
			(*m_explodingLevels).pop_back();
		
	}
}

void ExplodedView::verifyExplodingParts(){
	
	//Verify if the leaf nodes are on their correct positions
	if((*m_explodingLevels).size() > 1){ //do not explode the root node
		for(int j=0; j<(*m_explodingLevels).back().size(); j++){ 
			(*m_explodingLevels).back()[j]->explode(0.1);
			m_viewer->frame();
		}
		(*m_explodingLevels).pop_back();
	}

	(*m_explodingLevels).pop_back();
}


void ExplodedView::buildBox(){
	
	osg::ShapeDrawable* box = new osg::ShapeDrawable(new osg::Box(osg::Vec3d(0,0,0), 800));
	osg::StateSet* stateset = new osg::StateSet;
	osg::Geode* geode = new osg::Geode;
	osg::PolygonMode* polymode = new osg::PolygonMode;

	

	
	polymode->setMode(osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::LINE);
	stateset->setAttributeAndModes(polymode,osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);
	stateset->setMode(GL_LIGHTING,osg::StateAttribute::OVERRIDE|osg::StateAttribute::OFF);
	box->setStateSet(stateset);
	
    geode->addDrawable(box);

	m_sceneGraphRoot->addChild(geode);

}


void ExplodedView::run(){

	setUp("test4.3ds");
	
	for(int i=0; i<60; i++)
		m_viewer->frame();
	
	
	buildPartsGraph();
	explode();

	

	while(!m_viewer->done()){
		updateExplodingParts();
		//verifyExplodingParts();
		m_viewer->frame();

		
	}

	

}

