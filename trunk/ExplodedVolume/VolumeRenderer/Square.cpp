#include "Square.h"

Square::Square(){
	 


	
	GLfloat texCoords[] = {0, 0,
						  1, 0,
						  1,1,
						  0, 1};

	GLfloat vertices[] = {0, 0, 0,
						  1, 0, 0,
						  1, 1, 0,
						  0, 1, 0};


	// Generate And Bind The Vertex Buffer
	glGenBuffersARB( 1, &mVboVertices );					// Get A Valid Name
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, mVboVertices );			// Bind The Buffer
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, 12*sizeof(GLfloat), vertices, GL_STATIC_DRAW_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	// Generate And Bind The TextureCoord Buffer
	glGenBuffersARB( 1, &mVboTexCoord );					// Get A Valid Name
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, mVboTexCoord );			// Bind The Buffer
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, 8*sizeof(GLfloat), texCoords, GL_STATIC_DRAW_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

}

Square::~Square(){


}

void Square::render(){


	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, mVboVertices);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, mVboTexCoord);
	glTexCoordPointer(2,GL_FLOAT,0,0);


    


	
	


	//glDrawElements(GL_LINES, 24, GL_UNSIGNED_BYTE, mIndices);
	glDrawArrays(GL_QUADS, 0, 4);


	glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glBindBufferARB(GL_TEXTURE_BUFFER_ARB, 0);


}