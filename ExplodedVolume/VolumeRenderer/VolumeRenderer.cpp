#include "VolumeRenderer.h"

VolumeRenderer::VolumeRenderer()
{

}

VolumeRenderer::~VolumeRenderer()
{

}

void VolumeRenderer::render(){
	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable( GL_DEPTH_TEST ); 
	glLoadIdentity();
	
}
