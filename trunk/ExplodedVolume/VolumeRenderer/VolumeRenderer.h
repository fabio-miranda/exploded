#ifndef VOLUMERENDERER_H
#define VOLUMERENDERER_H


#include "GL/glew.h"
#include "GL/glfw.h"
//#include "IL/il.h"
#include "IL/ilut.h"
#include "Cube.h"
#include "Square.h"
#include "FBO.h"
#include "RayCastShader.h"
#include <limits>
#ifdef max
#undef max
#endif


class VolumeRenderer
{
public:
	VolumeRenderer();
	~VolumeRenderer();

	void render();
	void loadFile(char* fileName);
	void init();
	void resize(int width, int height);

private:
	void renderBackFace();
	void renderFrontFace();
	void renderToScreen();
	void updateInput();


	int mSize;
	float* mScalars;
	Cube* mCube;
	Square* mSquare;
	RayCastShader* mRayCastShader;
	Shader* mPosToTexShader;
	FBO* mFBOFront;
	FBO* mFBOBack;
	GLuint mVolumeTextureId;
	GLuint mTextureTemp;
	int mMouseX;
	int mMouseY;
	float mModelRotX;
	float mModelRotZ;


};

#endif // VOLUMERENDERER_H
