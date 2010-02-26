#include "ModelLoader.h"



void ModelLoader::Geresim(char* pModelName, osg::Geometry* pGeometry , std::vector<Cell*>* pCellArray){

	std::ifstream file;
	std::string lineRead;
	std::vector<std::string> tokens;
	int ni, nj, nk, numActiveCells, numVertices;
	osg::Vec3Array* verticesArray = new osg::Vec3Array();

	file.open(pModelName);
	if(!file){
		std::cout << "No data loaded" << std::endl;
		exit(1);
	}

	//First line
	std::getline(file, lineRead);
	boost::split(tokens, lineRead, boost::is_any_of(" "));
	ni = atoi(tokens[0].c_str()); nj = atoi(tokens[1].c_str()); nk = atoi(tokens[2].c_str());
	numActiveCells = atoi(tokens[3].c_str()); numVertices = atoi(tokens[4].c_str());



	pCellArray->resize(ni*nj*nk);


	//Vertices
	for(int i=0; i< numVertices; i++){

		tokens.clear();
		std::getline(file, lineRead);
		boost::split(tokens, lineRead, boost::is_any_of(" "));

		verticesArray->push_back(osg::Vec3(atof(tokens[0].c_str()), atof(tokens[1].c_str()), atof(tokens[2].c_str())));

	}


	//Cells
	osg::Vec4Array* colorsArray = new osg::Vec4Array();
	int* neighbours;
	int* verticesIndex;
	osg::Vec3* cellIndex;
	Cell* cell;
	
	
	for(int i=0; i< ni*nj*nk ; i++){
		

		std::getline(file, lineRead);
		tokens.clear();
		boost::split(tokens, lineRead, boost::is_any_of(" "));

		int indexi = atoi(tokens[1].c_str());
		int indexj = atoi(tokens[2].c_str());
		int indexk = atoi(tokens[3].c_str());
		cellIndex = new osg::Vec3(indexi, indexj, indexk);

		verticesIndex = new int[8];
		for(int j=4; j<12; j++){
			verticesIndex[j - 4] = atoi(tokens[j].c_str());
		}

		neighbours = new int[6];
		for(int k=12; k<18; k++){
			neighbours[k - 12] = atoi(tokens[k].c_str());
		}
		
		float prop = atof(tokens[18].c_str());


		if(lineRead[0] == '1'){
			cell = new Cell(true, cellIndex, verticesIndex, neighbours, prop);
			
			//Color
			srand(prop * 10000.0);
			osg::Vec4f color = osg::Vec4f(((float)rand() / RAND_MAX), (float)rand() / RAND_MAX, (float)rand() / RAND_MAX, 1.0);
			for(int j = 0; j<4; j++){
				colorsArray->push_back(color);
			}
		}
		else{
			cell = new Cell(false, cellIndex, verticesIndex, neighbours, prop);

		}
		pCellArray->push_back(cell);

	}

	file.close();
	
	//pCellArray = cellMatrix;

	pGeometry = new osg::Geometry();
	pGeometry->setVertexArray(verticesArray);
	pGeometry->setColorArray(colorsArray);
	pGeometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	




}