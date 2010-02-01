#ifndef RayCastShader_H
#define RayCastShader_H

#include "Shader.h"


class RayCastShader : public Shader{
	
	
public:
	RayCastShader(char*, char*);
	~RayCastShader();
	void Enable(GLuint backTextureId, GLuint volumeTextureId);
	void Disable();



private:
	GLuint m_locFrontTexture;
	GLuint m_locBackTexture;
	GLuint m_locVolumeTexture;
	
	

};


#endif