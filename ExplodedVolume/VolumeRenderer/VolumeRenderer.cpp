#include "VolumeRenderer.h"

VolumeRenderer::VolumeRenderer()
{

	init();
	
	mCube = new Cube();
	mSquare = new Square();
	mRayCastShader = new Shader("../Resources/raycast.vert", "../Resources/raycast.frag");
	mFBOFront = new FBO(1280, 720);
	mFBOBack = new FBO(1280, 720);

	/*
	ilInit();
	iluInit();

	ilutRenderer(ILUT_OPENGL);
	mFBOFront->mTextureId = ilutGLLoadImage("a.jpg");
	*/

	
}

VolumeRenderer::~VolumeRenderer()
{

}

void VolumeRenderer::init(){
	
	glewInit();
	glfwInit();
	glEnable(GL_DEPTH_TEST);



}

void VolumeRenderer::resize(int width, int height){

	

	glViewport( 0, 0, width, height );
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(                   // Set perspective view
        90.0,                         // Field of view = 65 degrees
        (double)width/(double)height, // Window aspect (assumes square pixels)
        0.01,                          // Near Z clipping plane
        10000.0                         // Far Z clippling plane
    );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();	    
    gluLookAt(2,2,2,0,0,0,0,0,1);




}

void VolumeRenderer::render(){
	
	//Render backface
	renderBackFace();
	
	//Render front face (final image)
	renderFrontFace();

	
	//Render final image to screen
	renderToScreen();

	glfwSwapBuffers();
	
}

void VolumeRenderer::renderBackFace(){

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	mFBOBack->Enable();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	mCube->render();
	glDisable(GL_CULL_FACE);
	mFBOBack->Disable();

}

//Render the front face and perform the ray casting
void VolumeRenderer::renderFrontFace(){
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	mFBOFront->Enable();
	mRayCastShader->Enable();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	mCube->render();
	glDisable(GL_CULL_FACE);
	mRayCastShader->Disable();
	mFBOFront->Disable();
}

void VolumeRenderer::renderToScreen(){
	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D,mFBOFront->mTextureId);
	
	

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 1, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	mSquare->render();

	glDisable(GL_TEXTURE_2D);
	//TODO: use push and pop, instead of calling resize
	resize(1280, 720);
	
	/*
	float* store = new float[1280 * 720];
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &store[0]);
	
	ILuint ImageName;
	ilInit();
	iluInit();

	ilGenImages(1, &ImageName);
	ilBindImage(ImageName);

	

	ilTexImage(1280, 720, 1, 4, IL_RGBA, IL_UNSIGNED_BYTE, &store[0]);
	ilEnable(IL_FILE_OVERWRITE);
	ilSaveImage("teste.png");
	

	
	*/

}

void VolumeRenderer::setData(char* data, int size){
	
	
	mScalars = new float[size];
	mSize = size;

	//scale the scalar values to [0, 1]
	char maxSize = std::numeric_limits<char>::max();
	for(int i=0; i<size; i++){
		mScalars[i] = data[i] / maxSize;
	}



}