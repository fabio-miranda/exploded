#include "Cube.h"

Cube::Cube(){
	 


	
	GLfloat vertices[] = {1,1,1,  -1,1,1,  -1,-1,1,  1,-1,1,        // v0-v1-v2-v3
                      1,1,1,  1,-1,1,  1,-1,-1,  1,1,-1,        // v0-v3-v4-v5
                      1,1,1,  1,1,-1,  -1,1,-1,  -1,1,1,        // v0-v5-v6-v1
                      -1,1,1,  -1,1,-1,  -1,-1,-1,  -1,-1,1,    // v1-v6-v7-v2
                      -1,-1,-1,  1,-1,-1,  1,-1,1,  -1,-1,1,    // v7-v4-v3-v2
                      1,-1,-1,  -1,-1,-1,  -1,1,-1,  1,1,-1};   // v4-v7-v6-v5

	GLfloat texCoords[] = {0,0,  1,0,  1,1,  0,1,
						   0,0,  1,0,  1,1,  0,1,
						   0,0,  1,0,  1,1,  0,1,
						   0,0,  1,0,  1,1,  0,1,
						   0,0,  1,0,  1,1,  0,1,
						   0,0,  1,0,  1,1,  0,1};
	



	// Generate And Bind The Vertex Buffer
	glGenBuffersARB( 1, &mVboVertices );					// Get A Valid Name
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, mVboVertices );			// Bind The Buffer
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, 24*3*sizeof(GLfloat), vertices, GL_STATIC_DRAW_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	// Generate And Bind The TextureCoord Buffer
	glGenBuffersARB( 1, &mVboTexCoord );					// Get A Valid Name
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, mVboTexCoord );			// Bind The Buffer
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, 24*2*sizeof(GLfloat), texCoords, GL_STATIC_DRAW_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

}

Cube::~Cube(){


}

void Cube::render(){

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, mVboVertices);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, mVboTexCoord);
	glTexCoordPointer(2,GL_FLOAT,0,0);


	//glDrawElements(GL_LINES, 24, GL_UNSIGNED_BYTE, mIndices);
	glDrawArrays(GL_QUADS, 0, 24);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glBindBufferARB(GL_TEXTURE_BUFFER_ARB, 0);

	glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	

/*
	GLfloat v[8][3];
	v[0][0] = v[1][0] = v[2][0] = v[3][0] = -1;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = 1;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -1;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = 1;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = 1;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = -1;
  */

  //GLint faces[6][4] = {  /* Vertex indices for the 6 faces of a cube. */
  //{0, 1, 2, 3}, {3, 2, 6, 7}, {7, 6, 5, 4},
  //{4, 5, 1, 0}, {5, 6, 2, 1}, {7, 4, 0, 3} };

  //GLfloat n[6][3] = {  /* Normals for the 6 faces of a cube. */
  //{-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
  //{0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0} };


/*
	int i;
	for (i = 0; i < 6; i++) {
    glBegin(GL_QUADS);
    glNormal3fv(&n[i][0]);
    glVertex3fv(&v[faces[i][0]][0]);
    glVertex3fv(&v[faces[i][1]][0]);
    glVertex3fv(&v[faces[i][2]][0]);
    glVertex3fv(&v[faces[i][3]][0]);
    glEnd();
  }
  */
/*
  glBegin(GL_TRIANGLES);
    glColor3f(1,0,0);
    glVertex3f(-1,0,0);
    glVertex3f(1,0,0);
    glVertex3f(0,1,0);
    glEnd();

*/



}