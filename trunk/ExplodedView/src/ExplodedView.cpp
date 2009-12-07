#include "ExplodedView.h"


#define MINIMUM_DISTANCE_TO_CONSIDER_CONTACT 0
#define STEPSIZE 0.5
#define EXPLOSION_STEPSIZE 0.8
#define ITERATIONS 50
#define VISUALIZE_GRAPH_BUILDING true
#define PRINT_GRAPH true


USE_OSGPLUGIN(3ds);
USE_GRAPHICSWINDOW();

ExplodedView::ExplodedView(){
	m_sceneGraphRoot = new osg::Group();
	m_viewer = new osgViewer::Viewer();
	m_explodingLevels = new std::vector< std::vector<Part*> >();
	m_inplodingLevels = new std::vector< std::vector<Part*> >();
	m_ptrCurrentPart = NULL;
	//m_vCollide = new VCollide();
}

 
void ExplodedView::setUp(char* modelName){

	m_viewer->getCamera()->setClearColor(osg::Vec4(1,1,1,1));

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

	//explosion
	myKeyboardEventHandler* keyboard = new myKeyboardEventHandler(this);
	m_viewer->addEventHandler(keyboard);

    // windowed
	m_viewer->setUpViewInWindow(50, 25, 1280, 720);
	m_viewer->getCamera()->setClearColor(osg::Vec4(1,1,1,1));

	//box
	buildBox();

	//model
	// load the data
	//osg::ref_ptr<osgDB::ReaderWriter::Options> opt = new osgDB::ReaderWriter::Options; 
	//opt->setObjectCacheHint(osgDB::ReaderWriter::Options::CACHE_NONE); 
	m_loadedModel = osgDB::readNodeFile(modelName);
    if (!m_loadedModel) 
    {
        std::cout << "No data loaded" << std::endl;
		exit(1);
    }


    // optimize the scene graph, remove redundant nodes and state etc.
    osgUtil::Optimizer optimizer;
	optimizer.optimize(m_loadedModel);



	///Find the parts
	FindNamedPartVisitor findParts("part_");

	m_loadedModel->accept(findParts);
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
		//findBlockedDirections();

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
				if(m_partsGraph[i]->m_container && m_partsGraph[i]->m_segmentedParts == NULL){
					//Interactive explode the container on each of the 6 directions and find the smallest distance
					//in order to make the interlocked node visible
					m_partsGraph[i]->split(m_sceneGraphRoot, m_viewer, STEPSIZE);
					insertOnPartsGraph(m_partsGraph[i]);
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
	CollisionData* currentCollisionData;

	Part* aux = NULL;

	for(int i=0; i<m_partsGraph.size(); i++){
		if(m_partsGraph[i]->m_inserted == false){

			currentCollisionData = m_partsGraph[i]->findSmallestDistanceOutBoundingBox();
			

			//if(currentCollisionData->collided == false)
				//return m_partsGraph[i];

			if(currentCollisionData != NULL
				&& currentCollisionData->distanceOutBoundingBox <= smallestDistance
				&& m_partsGraph[i]->m_countRestrictedDirections < 6){

				if((currentCollisionData->collided && currentCollisionData->collidedWith->m_inserted)
					|| (currentCollisionData->collided == false)){

					aux = m_partsGraph[i];
					smallestDistance = currentCollisionData->distanceOutBoundingBox;
				}
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
		if(m_partsGraph[i]->m_inserted == false)
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

void ExplodedView::switchExplodeInplode(int index){
	
	if(index >= m_partsGraph.size()) return;

	if(m_partsGraph[index]->m_inploded == false){
		inplode(index);
	}
	else if(m_partsGraph[index]->m_exploded == false){
		explode(index);
	}
}

void ExplodedView::explode(int index){
	m_ptrCurrentPart = m_partsGraph[index];
	//m_partsGraph[index]->turnHighlight(true);
	bfs(m_partsGraph[index], &m_explodingLevels);
}

void ExplodedView::inplode(int index){
	m_ptrCurrentPart = NULL;
	//m_partsGraph[index]->turnHighlight(false);
	bfs(m_partsGraph[index], &m_inplodingLevels);
}

void ExplodedView::bfs(Part* v, std::vector< std::vector<Part*> >** levels){

	for(int i=0; i<m_partsGraph.size(); i++)
		m_partsGraph[i]->m_visited = false;

	*levels = new std::vector< std::vector<Part*> >();

	std::list< Part* > queue;
	v->m_visited = true;
	queue.push_back(v);
	(*levels)->push_back(std::vector<Part*>());
	(*levels)->back().push_back(v);


	while(queue.size() > 0){
		Part* v = queue.front();
		queue.pop_front();
		(*levels)->push_back(std::vector<Part*>());
		ProxyPart* vNeighbour = v->m_ptrFirstProxyPart;

		while(vNeighbour != NULL){

			if(vNeighbour->m_ptrActualPart->m_visited == false){
				queue.push_back(vNeighbour->m_ptrActualPart);
				vNeighbour->m_ptrActualPart->m_visited = true;
				(*levels)->back().push_back(vNeighbour->m_ptrActualPart);
			}

			vNeighbour = vNeighbour->m_ptrNextProxyPart;
		}

		if((*levels)->back().size() == 0)
			(*levels)->pop_back();
	}

}

void ExplodedView::updateExplodingParts(){
	
	bool allFromLevelExploded = true;
	//Explode the leaf nodes
	if((*m_explodingLevels).size() > 1){ //do not explode the root node
		for(int j=0; j<(*m_explodingLevels).back().size(); j++){
			(*m_explodingLevels).back()[j]->explode(EXPLOSION_STEPSIZE);
			
			m_viewer->frame();


			if((*m_explodingLevels).back()[j]->m_exploding && (*m_explodingLevels).back()[j]->m_exploded == false)
				allFromLevelExploded = false;
		}

		if(allFromLevelExploded){
			(*m_explodingLevels).pop_back();
		}
	}
	else if((*m_explodingLevels).size() == 1){
		(*m_explodingLevels).back()[0]->m_exploded = true; //mark the root as exploded
		(*m_explodingLevels).back()[0]->m_inploded = false;
		m_explodingLevels->clear();
	}
	
}

void ExplodedView::updateInplodingParts(){
	
	bool allFromLevelInploded = true;
	if((*m_inplodingLevels).size() > 1){ //do not explode the root node
		for(int j=(*m_inplodingLevels)[1].size()-1; j>=0; j--){
			(*m_inplodingLevels)[1][j]->inplode(EXPLOSION_STEPSIZE);
			
			m_viewer->frame();


			if((*m_inplodingLevels)[1][j]->m_inploding && (*m_inplodingLevels)[1][j]->m_inploded == false)
				allFromLevelInploded = false;
		}

		if(allFromLevelInploded){
			(*m_inplodingLevels).erase((*m_inplodingLevels).begin()+1);
		}
	}
	else if((*m_inplodingLevels).size() == 1){
		(*m_inplodingLevels).back()[0]->m_inploded = true; //mark the root as inploded
		(*m_inplodingLevels).back()[0]->m_exploded = false;
		m_inplodingLevels->clear();
	}
	
}
/*
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
*/


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

	

	while(!m_viewer->done()){
		updateExplodingParts();
		updateInplodingParts();
		//verifyExplodingParts();
		m_viewer->frame();

		if(m_ptrCurrentPart != NULL){
			
			osg::Vec3 eye = osg::Vec3();
			osg::Vec3 center = osg::Vec3();
			osg::Vec3 up = osg::Vec3();
			m_viewer->getCamera()->getViewMatrixAsLookAt(eye, center, up);
			m_ptrCurrentPart->checkVisibility(eye, &m_partsGraph);
		}
		
		
		
		
	}

	

}


