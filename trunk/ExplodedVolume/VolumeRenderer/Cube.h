#ifndef CUBE_H
#define CUBE_H

#include "GL/glew.h"


class Cube
{
public:
	Cube();
	~Cube();

	void render();

private:
	GLuint mVboVertices;
	GLuint mVboIndices;


};

#endif // CUBE_H
