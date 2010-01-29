#ifndef Shader_H
#define Shader_H

#include "GL/glew.h"
#include "textfile.h"


class Shader{
	
	
public:
	Shader(char*, char*);
	~Shader();
	void Enable();
	void Disable();
	void LoadFiles(char*, char*);

	GLhandleARB m_shaderProg;



private:
	GLhandleARB m_shaderVert;
	GLhandleARB m_shaderFrag;
	
	

};


#endif