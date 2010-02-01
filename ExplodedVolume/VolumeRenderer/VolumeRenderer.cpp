#include "VolumeRenderer.h"

VolumeRenderer::VolumeRenderer()
{

	

	init();

	loadFile("HeadVolume.dds");

	
	
	mCube = new Cube();
	mSquare = new Square();

	

	mRayCastShader = new RayCastShader("../Resources/raycast.vert", "../Resources/raycast.frag");
	mPosToTexShader = new Shader("../Resources/positionToTexture.vert", NULL);
	mFBOBack = new FBO(1280, 720);
	//mFBOFront = new FBO(1280, 720);

	mModelRotX = 0;
	mModelRotZ = 0;
	

	/*
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	mTextureTemp = ilutGLLoadImage("a.jpg");
	*/
	
	
	

	
}

VolumeRenderer::~VolumeRenderer()
{

}

void VolumeRenderer::init(){
	
	glewInit();
	glfwInit();
	ilInit();
	iluInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);



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
    gluLookAt(0,2,0,0,0,0,0,0,1);




}

void VolumeRenderer::render(){
	
	updateInput();
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glRotatef(mModelRotX, 0.0, 0.0, 1.0);
	glRotatef(mModelRotZ, 1.0, 0.0, 0.0);

	//Render backface
	renderBackFace();
	
	//Render front face (final image)
	renderFrontFace();

	
	//Render final image to screen
	//renderToScreen();

	

	glfwSwapBuffers();

	
	
}

void VolumeRenderer::renderBackFace(){

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	mFBOBack->Enable();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	mPosToTexShader->Enable();
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D,mTextureTemp);
	mCube->render();
	//glBindTexture(GL_TEXTURE_2D,0);
	glDisable(GL_CULL_FACE);
	mPosToTexShader->Disable();
	mFBOBack->Disable();
	/*
	float* store = new float[1280 * 720];
	glBindTexture(GL_TEXTURE_2D,mFBOBack->mTextureId);
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

//Render the front face and perform the ray casting
void VolumeRenderer::renderFrontFace(){
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//mFBOFront->Enable();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	mRayCastShader->Enable(mFBOBack->mTextureId, mVolumeTextureId);

	mCube->render();

	
	mRayCastShader->Disable();
	glDisable(GL_CULL_FACE);

	//mFBOFront->Disable();
	
	
	
}
/*
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
	glBindTexture(GL_TEXTURE_2D,mFBOBack->mTextureId);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &store[0]);
	
	ILuint ImageName;
	ilInit();
	iluInit();

	ilGenImages(1, &ImageName);
	ilBindImage(ImageName);

	

	ilTexImage(1280, 720, 1, 4, IL_RGBA, IL_UNSIGNED_BYTE, &store[0]);
	ilEnable(IL_FILE_OVERWRITE);
	ilSaveImage("teste.png");
	


}
*/

void VolumeRenderer::loadFile(char* fileName){
	
	ILuint imgId = 0;
	ilGenImages(1, &imgId);
	ilBindImage(imgId);

	ilLoadImage(fileName);


	glGenTextures(1, &mVolumeTextureId);
	glBindTexture(GL_TEXTURE_3D, mVolumeTextureId);


	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage3D(GL_TEXTURE_3D, 0, ilGetInteger(IL_IMAGE_BPP), 
				 ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
				 ilGetInteger(IL_IMAGE_DEPTH), 0, ilGetInteger(IL_IMAGE_FORMAT), 
				 ilGetInteger(IL_IMAGE_TYPE), ilGetData());


	ilBindImage(0);
	ilDeleteImage(imgId);


}


void VolumeRenderer::updateInput(){

	
	
	if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT)){

		int x, y;
		glfwGetMousePos(&x,&y);

		if(x != mMouseX){

			if(x < mMouseX)
				mModelRotX = -0.1;
			else
				mModelRotX = 0.1;

			

			mMouseX = x;
			
		}
		else{
			mModelRotX = 0;
		}

		if(y != mMouseY){
			if(y < mMouseY)
				mModelRotZ = 0.1;
			else
				mModelRotZ = -0.1;

			mMouseY = y;

		}
		else{
			mModelRotZ = 0;

		}
	}
	else{
		mModelRotX = 0;
		mModelRotZ = 0;
	}


	
}