
///////////////////////////////////////////////////////////////////////////////
// main.cpp
// ========
// Use this to run the volume renderer without the QT interface
//
//  AUTHOR: Fábio Markus Miranda (fabiom@gmail.com)
///////////////////////////////////////////////////////////////////////////////


#include "VolumeRenderer.h"




int main()
{
	glfwInit();

	int ok = glfwOpenWindow(
							1280, 720,          // Width and height of window
							8, 8, 8,           // Number of red, green, and blue bits for color buffer
							8,                 // Number of bits for alpha buffer
							24,                // Number of bits for depth buffer (Z-buffer)
							0,                 // Number of bits for stencil buffer
							GLFW_WINDOW        // We want a desktop window (could be GLFW_FULLSCREEN)
						);

    if( !ok )
    {
        glfwTerminate();
    }

	VolumeRenderer* volumeRenderer = new VolumeRenderer();
	volumeRenderer->resize(1280, 720);

	
	while(true){
		volumeRenderer->render();
		glfwSwapBuffers();
	}
	
	
}
