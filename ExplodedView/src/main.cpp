#include "ExplodedView.h"

int main(int argc, char** argv){
	
	ExplodedView* explodedView = new ExplodedView();
	explodedView->setUp();
	explodedView->buildPartsGraph("test1.3ds");
	explodedView->loop();

}