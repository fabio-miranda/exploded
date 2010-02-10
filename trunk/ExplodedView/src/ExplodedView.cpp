#include "ExplodedView.h"


#define MINIMUM_DISTANCE_TO_CONSIDER_CONTACT 0
#define STEPSIZE 0.1
#define EXPLOSION_STEPSIZE 2.0
#define ITERATIONS 100
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

void ExplodedView::loadModelOSG(char* modelName){

	m_loadedModel = osgDB::readNodeFile(modelName);
    if (!m_loadedModel) 
    {
        std::cout << "No data loaded" << std::endl;
		exit(1);
    }
	m_loadedModel->setCullingActive (false);

	findPartsOSG();

}



void ExplodedView::loadModelGeresim(char* modelName){
	
	std::ifstream file;
	std::string lineRead;
	std::vector<std::string> tokens;
	int ni, nj, nk, numActiveCells, numVertices;

	//osg::Group* root = new osg::Group();
	//osg::Geode* geode = new osg::Geode();
	//osg::Geometry* geometry = new osg::Geometry();

	osg::Vec3Array* verticesArray = new osg::Vec3Array();
	m_cellsVector = new std::vector<Cell*>();
	m_propSet = new std::set<float>();
	//std::multimap<float, Cell*>* cellsMap = new std::multimap<float, Cell*>();;
	

	file.open(modelName);

	if(!file){
		std::cout << "No data loaded" << std::endl;
		exit(1);
	}

	//First line
	std::getline(file, lineRead);
	StringExplode(lineRead, " ", &tokens);
	ni = atoi(tokens[0].c_str()); nj = atoi(tokens[1].c_str()); nk = atoi(tokens[2].c_str());
	numActiveCells = atoi(tokens[3].c_str()); numVertices = atoi(tokens[4].c_str());

	

	//Vertices
	for(int i=0; i< numVertices; i++){

		tokens.clear();
		std::getline(file, lineRead);
		StringExplode(lineRead, " ", &tokens);

		verticesArray->push_back(osg::Vec3(atof(tokens[0].c_str()), atof(tokens[1].c_str()), atof(tokens[2].c_str())));


	}
	//geometry->setVertexArray(verticesArray);


	//Cells
	osg::Vec4Array* colorsArray = new osg::Vec4Array();
	for(int i=0; i< ni*nj*nk; i++){
		

		std::getline(file, lineRead);
		


		if(lineRead[0] == '1'){
			
			tokens.clear();
			StringExplode(lineRead, " ", &tokens);
			std::vector<int>* neighbours = new std::vector<int>();
			vector<int>* verticesIndex = new vector<int>();
			osg::Vec3* cellIndex;
			bool active = 1;
			

			if(true){
			//if(atoi(tokens[3].c_str()) == 0){

				cellIndex = new osg::Vec3(atoi(tokens[1].c_str()), atoi(tokens[2].c_str()), atoi(tokens[3].c_str()));
				
				for(int j=4; j<12; j++){
					verticesIndex->push_back(atoi(tokens[j].c_str()));
				}
				for(int k=12; k<18; k++){
					neighbours->push_back(atoi(tokens[k].c_str()));
				}

				Cell* cell = new Cell(active, cellIndex, verticesIndex, neighbours, atof(tokens[18].c_str()));
				//cell->addPrimitiveSets(geometry);
				m_cellsVector->push_back(cell);
		
				
				//Color
				srand(cell->mProp * 10000.0);
				osg::Vec4f color = osg::Vec4f(((float)rand() / RAND_MAX), (float)rand() / RAND_MAX, (float)rand() / RAND_MAX, 1.0);
				//osg::Vec4f color = osg::Vec4f(0, 0, cell->mProp, 1);
				//osg::Vec4f color = osg::Vec4f(1, 0, 0, 1);
				for(int j = 0; j<6; j++){
					colorsArray->push_back(color);
				}
				//cellsMap->insert(pair<float, Cell*>(cell->mProp, cell));
				m_propSet->insert(cell->mProp);
				

			}
		}
	}
	file.close();

	//geometry->setColorArray(colorsArray);
	//geometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
	
	//Light
	
	osg::Light * light = new osg::Light();
	osg::LightSource * lightsource = new osg::LightSource();
	lightsource->setLight(light); 
	m_sceneGraphRoot->addChild(lightsource);
	light->setAmbient(osg::Vec4d(1.0, 1.0, 1.0, 1.0));
	
	//Toon shading
	/*
	osgFX::Cartoon* toon = new osgFX::Cartoon();
	//toon->setLightNumber(0);
	//toon->setUpDemo();
	toon->addChild(root);
	
	//Add nodes to the scene graph
	geode->addDrawable(geometry);
	root->addChild(geode);
	m_sceneGraphRoot->addChild(toon);
	*/

	findPartsGeresim(verticesArray, colorsArray);
}

void ExplodedView::findPartsOSG(){
	
	//Find the parts
	FindNamedPartVisitor findParts("part_");

	m_loadedModel->accept(findParts);
	m_partsGraph = findParts.getPartList();

}


void ExplodedView::findPartsGeresim(osg::Vec3Array* verticesArray, osg::Vec4Array* colorsArray){

	std::set<float>::iterator it;
	//m_partsGraph = new std::vector<Part*>();

	osg::Group* root = new osg::Group();
	

	//Look the cells and find the ones with the same color
	int i =0 ;
	for(it = m_propSet->begin(); it != m_propSet->end(); it++){
		
		osg::Geometry* geometry = new osg::Geometry();
		osg::Geode* geode = new osg::Geode();

		geometry->setVertexArray(verticesArray);
		geometry->setColorArray(colorsArray);
		geometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

		geode->addDrawable(geometry);
		
		
		//if(i==3)
		root->addChild(geode);
		

		for(int j=0; j<m_cellsVector->size(); j++){
			
			if(*it == m_cellsVector->at(j)->mProp){
				m_cellsVector->at(j)->addPrimitiveSets(geometry);
			}
		}
		Part* part = new Part();
		part->m_osgNode = geode;

		m_partsGraph.push_back(part);
	}

	osgFX::Cartoon* toon = new osgFX::Cartoon();
	//toon->setLightNumber(0);
	//toon->setUpDemo();
	toon->setOutlineLineWidth(0.1);
	toon->addChild(root);

	m_sceneGraphRoot->addChild(toon);
	

}
 
void ExplodedView::setUp(){

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
	//buildBox();

	//model
	// load the data
	//osg::ref_ptr<osgDB::ReaderWriter::Options> opt = new osgDB::ReaderWriter::Options; 
	//opt->setObjectCacheHint(osgDB::ReaderWriter::Options::CACHE_NONE); 
	

	osg::setNotifyLevel(osg::NOTICE);
    // optimize the scene graph, remove redundant nodes and state etc.
    osgUtil::Optimizer optimizer;
	/////optimizer.optimize(m_loadedModel);
	optimizer.optimize(m_sceneGraphRoot, optimizer.CHECK_GEOMETRY);

	

	
	for (int i=0; i<m_partsGraph.size(); i++) {
		//m_sceneGraphRoot->addChild(m_partsGraph[i]->getOSGNode());
		//m_partsGraph[i]->setUp(m_vCollide, m_sceneGraphRoot);
		/////m_partsGraph[i]->setUp(m_sceneGraphRoot);
	}

	m_viewer->setCameraManipulator( new osgGA::TrackballManipulator() );
	m_viewer->getCameraManipulator()->setHomePosition(osg::Vec3d(2500, 3500, -3500), osg::Vec3d(1500, 1500, 3242), osg::Vec3d(0,0,-1));

	//Set the scene
	m_viewer->setSceneData(m_sceneGraphRoot);


	//Disable small feature culling
	/*
	osg::CullStack::CullingMode cullingMode = m_viewer->getCamera()->getCullingMode();
	cullingMode &= ~(osg::CullStack::SMALL_FEATURE_CULLING);
	m_viewer->getCamera()->setCullingMode( cullingMode );
	m_viewer->getCamera()->setComputeNearFarMode(osg::CullSettings::COMPUTE_NEAR_FAR_USING_BOUNDING_VOLUMES);
	*/
	m_viewer->getCamera()->setClearColor(osg::Vec4(1,1,1,1));

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

				//Gambi here
				//if((currentCollisionData->collided && currentCollisionData->collidedWith->m_inserted)
					//|| (currentCollisionData->collided == false)){

					aux = m_partsGraph[i];
					smallestDistance = currentCollisionData->distanceOutBoundingBox;
				//}
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

	if(m_partsGraph[index]->m_osgNode->getNodeMask() == 0xffffffff)
		m_partsGraph[index]->m_osgNode->setNodeMask(0x0);
	else
		m_partsGraph[index]->m_osgNode->setNodeMask(0xffffffff);

	/*
	if(m_partsGraph[index]->m_inploded == false){
		inplode(index);
	}
	else if(m_partsGraph[index]->m_exploded == false){
		explode(index);
	}
	*/
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
			
			//m_viewer->frame();


			if((*m_explodingLevels).back()[j]->m_exploding && (*m_explodingLevels).back()[j]->m_exploded == false)
				allFromLevelExploded = false;
		}

		if(allFromLevelExploded){
			(*m_explodingLevels).pop_back();
			//return;
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
			
			//m_viewer->frame();


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

	
	//loadModelOSG("test0.3ds");
	loadModelGeresim("namorado_so.gmdl");
	setUp();
	
	for(int i=0; i<60; i++)
		m_viewer->frame();
	
	
	/////buildPartsGraph();


	while(!m_viewer->done()){
		/////updateExplodingParts();
		/////updateInplodingParts();
		//verifyExplodingParts();
		m_viewer->frame();
		/*
		if(m_ptrCurrentPart != NULL && (*m_explodingLevels).size() == 0){
			
			osg::Vec3 eye = osg::Vec3();
			osg::Vec3 center = osg::Vec3();
			osg::Vec3 up = osg::Vec3();
			m_viewer->getCamera()->getViewMatrixAsLookAt(eye, center, up);
			m_ptrCurrentPart->checkVisibility(eye, &m_partsGraph);
		}
		*/
	}
	
}


