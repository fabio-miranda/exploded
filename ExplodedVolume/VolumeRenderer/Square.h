#ifndef SQUARE_H
#define SQUARE_H

#include "GL/glew.h"


class Square
{
public:
	Square();
	~Square();

	void render();

private:
	GLuint mVboVertices;
	GLuint mVboTexCoord;


};

#endif // SQUARE_H
