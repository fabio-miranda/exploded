#include "RayCastShader.h"

RayCastShader::RayCastShader(char* vertexShaderFile, char* fragShaderFile) : Shader(vertexShaderFile, fragShaderFile){


	
	//glUseProgramObjectARB(m_shaderProg);
	Shader::Enable();
	m_locBackTexture = glGetUniformLocationARB(m_shaderProg,"backSampler");
	glUniform1iARB(m_locBackTexture, 0);

	m_locVolumeTexture = glGetUniformLocationARB(m_shaderProg,"volumeSampler");
	glUniform1iARB(m_locVolumeTexture, 1);

	Shader::Disable();


	


	//m_locFrontTexture = glGetUniformLocationARB(m_shaderProg,"frontSampler");
	//glUniform1i(m_locFrontTexture, 1);
	//m_locVolumeTexture = glGetUniformLocationARB(m_shaderProg,"volumeSampler");
	//glUniform1i(m_locVolumeTexture, 2);

	

}

RayCastShader::~RayCastShader(){


}

void RayCastShader::Enable(GLuint backTextureId, GLuint volumeTextureId){
	
	
	//int locBackTexture = glGetUniformLocationARB(m_shaderProg, "backSampler");



	//

	Shader::Enable();

	glActiveTextureARB(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,backTextureId);
	
	glActiveTextureARB(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_3D,volumeTextureId);



	
	//glDisable(GL_TEXTURE_2D);

	//glDisable(GL_TEXTURE_2D);
	
	//glActiveTexture(GL_TEXTURE2);
	//glBindTexture(GL_TEXTURE_2D, frontTextureId);
	//glActiveTexture(GL_TEXTURE3);
	//glBindTexture(GL_TEXTURE_3D, volumeTextureId);

	
}

void RayCastShader::Disable(){
	Shader::Disable();

	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, 0);
	//glDisable(GL_TEXTURE_2D);
	
}

