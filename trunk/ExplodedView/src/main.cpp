#include "ExplodedView.h"

int main(int argc, char** argv){
	
	ExplodedView* explodedView = new ExplodedView();
	explodedView->setUp();
	explodedView->buildPartsGraph("test4.3ds");
	explodedView->explode();
	explodedView->loop();

}