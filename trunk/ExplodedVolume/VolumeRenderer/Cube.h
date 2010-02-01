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
	GLuint mVboTexCoord;


};

#endif // CUBE_H
