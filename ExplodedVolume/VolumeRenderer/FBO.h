#ifndef FBO_H
#define FBO_H


#include "GL/glew.h"

/*
typedef struct{
	bool beingUsed;
	GLenum attachment;
} FBOAttachment;
*/

class FBO{

private:
	short mWidth;
	short mHeight;
	GLuint mFrameBufferId;
	

public:
	FBO(short,short);
	~FBO();
	void DeleteBuffer();
	void Enable();
	void Disable();

	
	GLuint mTextureId;

	
	
	

};

#endif