#ifndef VOLUMERENDERER_H
#define VOLUMERENDERER_H


#include "GL/glew.h"
#include "GL/glfw.h"
//#include "IL/ilut.h"
#include "Cube.h"
#include "Square.h"
#include "FBO.h"
#include "Shader.h"
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
	void setData(char* data, int size);
	void init();
	void resize(int width, int height);

private:
	void renderBackFace();
	void renderFrontFace();
	void renderToScreen();


	int mSize;
	float* mScalars;
	Cube* mCube;
	Square* mSquare;
	Shader* mRayCastShader;
	FBO* mFBOFront;
	FBO* mFBOBack;

};

#endif // VOLUMERENDERER_H
