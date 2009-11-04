#include "ExplodedView.h"


#define MINIMUM_DISTANCE_TO_CONSIDER_CONTACT 0.0
#define STEPSIZE 0.5
#define ITERATIONS 100
#define VISUALIZE_GRAPH_BUILDING false
#define PRINT_GRAPH true


USE_OSGPLUGIN(3ds);
USE_GRAPHICSWINDOW();

ExplodedView::ExplodedView(){
	m_sceneGraphRoot = new osg::Group();
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
	FindNamedPartVisitor findParts("part_");

	loadedModel->accept(findParts);
	m_partsGraph = findParts.getPartList();
	
	for (int i=0; i<m_partsGraph.size(); i++) {
		//m_sceneGraphRoot->addChild(m_partsGraph[i]->getOSGNode());
		m_partsGraph[i]->m_osgTransform->addChild(m_partsGraph[i]->m_osgNode);
		m_sceneGraphRoot->addChild(m_partsGraph[i]->m_osgTransform);
		m_partsGraph[i]->setPQPModel();
	}

	//Set the scene
	m_viewer->setSceneData(m_sceneGraphRoot);

	//Detect collisions
	int insertedParts = 0;

	while(insertedParts < m_partsGraph.size()){
		for(int i=0; i<m_partsGraph.size(); i++){
			if(m_partsGraph[i]->m_inserted == false){
				m_partsGraph[i]->resetRestrictedMoviments();
			}
		}

		//Find the directions in which the parts are blocked or not
		findBlockedDirections();

		//Find the distance that the parts have to walk in order to get out of the bounding box
		calculateDistancesOutBB();

		//Find the smallest distance and insert it on the graph
		Part* aux = findSmallestDistance();

		insertOnPartsGraph(aux);

		//insertPart(currentPart);
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
			for(int j=0; j<m_partsGraph.size(); j++){
				if(j != i && m_partsGraph[j]->m_inserted == false){
					//just count a restriction in a given direction only once (if there is three parts adjacents, just count once)
					//if(m_partsGraph[i]->m_collisions[0].empty())
						m_partsGraph[i]->checkCollisionsAlongAxis(m_viewer, m_partsGraph[j], 1, 0, 0, STEPSIZE, ITERATIONS, MINIMUM_DISTANCE_TO_CONSIDER_CONTACT, VISUALIZE_GRAPH_BUILDING);
					//if(m_partsGraph[i]->m_collisions[1].empty())
						m_partsGraph[i]->checkCollisionsAlongAxis(m_viewer, m_partsGraph[j], -1, 0, 0, STEPSIZE, ITERATIONS, MINIMUM_DISTANCE_TO_CONSIDER_CONTACT, VISUALIZE_GRAPH_BUILDING);
					//if(m_partsGraph[i]->m_collisions[2].empty())
						m_partsGraph[i]->checkCollisionsAlongAxis(m_viewer, m_partsGraph[j], 0, 1, 0, STEPSIZE, ITERATIONS, MINIMUM_DISTANCE_TO_CONSIDER_CONTACT, VISUALIZE_GRAPH_BUILDING);
					//if(m_partsGraph[i]->m_collisions[3].empty())
						m_partsGraph[i]->checkCollisionsAlongAxis(m_viewer, m_partsGraph[j], 0, -1, 0, STEPSIZE, ITERATIONS, MINIMUM_DISTANCE_TO_CONSIDER_CONTACT, VISUALIZE_GRAPH_BUILDING);
					//if(m_partsGraph[i]->m_collisions[4].empty())
						m_partsGraph[i]->checkCollisionsAlongAxis(m_viewer, m_partsGraph[j], 0, 0, 1, STEPSIZE, ITERATIONS, MINIMUM_DISTANCE_TO_CONSIDER_CONTACT, VISUALIZE_GRAPH_BUILDING);
					//if(m_partsGraph[i]->m_collisions[5].empty())
						m_partsGraph[i]->checkCollisionsAlongAxis(m_viewer, m_partsGraph[j], 0, 0, -1, STEPSIZE, ITERATIONS, MINIMUM_DISTANCE_TO_CONSIDER_CONTACT, VISUALIZE_GRAPH_BUILDING);
				}
			}
		}
		m_partsGraph[i]->m_osgTransform->setPosition(osg::Vec3d(0, 0, 0));
	}
}

void ExplodedView::calculateDistancesOutBB(){
	for(int i=0; i<m_partsGraph.size(); i++){
		if(m_partsGraph[i]->m_inserted == false && m_partsGraph[i]->m_countRestrictedDirections < 6){
				m_partsGraph[i]->findDistancesOutBoundingBox(m_viewer, STEPSIZE, MINIMUM_DISTANCE_TO_CONSIDER_CONTACT, VISUALIZE_GRAPH_BUILDING);
			}
		}
}

Part* ExplodedView::findSmallestDistance(){
	double smallestDistance = std::numeric_limits<double>::infinity();
	Part* currentPart = NULL;
	CollisionData* currentCollision = NULL;
	Part* lastOne = NULL;
	for(int i=0; i<m_partsGraph.size(); i++){
		if(m_partsGraph[i]->m_inserted == false){
			lastOne = m_partsGraph[i];
			for(int j=0; j<6; j++){
				for(int k=0; k<m_partsGraph[i]->m_collisions[j].size(); k++){
					if(m_partsGraph[i]->m_collisions[j][k]->collided){
						if(m_partsGraph[i]->m_collisions[j][k]->distanceOutBoundingBox < smallestDistance)
							currentPart = m_partsGraph[i];
							currentCollision = m_partsGraph[i]->m_collisions[j][k];
					}
				}
			}
		}
	}

	if(currentPart == NULL){
		lastOne->m_explosionDirection = NULL;
		return lastOne;
	}
	else{
		currentPart->m_explosionDirection = currentCollision;
		return currentPart;

	}
}

void ExplodedView::insertOnPartsGraph(Part* part){
	part->m_inserted = true;

	for(int i=0; i<6; i++){
		for(int j=0; j<part->m_collisions[i].size(); j++){
			if(part->m_collisions[i][j]->collided && part->m_collisions[i][j]->collidedWith->m_inserted == false){
				part->insertVertexFrom(part->m_collisions[i][j]->collidedWith);
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
	//bfs(m_partsGraph[0]);
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


void ExplodedView::loop(){
	
	
	while(!m_viewer->done()){
		//updateExplodingParts();
		//verifyExplodingParts();
		m_viewer->frame();
	}

}


