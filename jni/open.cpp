#include "open.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "ShaderHandler.h"
#include <android/log.h>
//#include <stdio.h>
//#include <stdlib.h>
#include "glm/glm.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Renderer.h"
#include "Camera.h"

#define APPNAME "OpenGLEngine"


GLuint VertexId;
	GLuint bufId;


static void checkGlError(const char* op) {
    GLint error = glGetError();
    
    //__android_log_print("after %s() ", op);
  //  __android_log_print("glError %d ", error);
}


const GLfloat gTriangleVertices[] = { 0.0f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f };


Renderer m_renderer;
Camera cam;

void open::DrawFrame()
{
	glm::mat4 Projection = glm::perspective(45.0f, 1024.0f / 768.0f, 0.1f, 20.0f);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


m_renderer.Render();
/*GLfloat verts[] =
{
	-1.0f, -1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,
	 0.0f, 1.0f, 0.0f,

};

  //checkGlError("glClear");
glBindBuffer(GL_ARRAY_BUFFER, bufId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gTriangleVertices), gTriangleVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

 //checkGlError("glDrawArrays");

glVertexAttribPointer(
	  VertexId,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	   2,                  // size
	   GL_FLOAT,           // type
	   GL_FALSE,           // normalized?
	   0,                  // stride
	    gTriangleVertices            // array buffer offset
	);

glDrawArrays(GL_TRIANGLES, 0,3);
// GLint error = glGetError();
//__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Error %d", error);      
*/
}


void open::Init(char * VertexShader, char * FragmentShader)
{

	__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Setting camera");      

	m_renderer.SetCamera(cam);

__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Initing shit");      

	m_renderer.Init();

__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Preparing shit");      


	m_renderer.Prepare();


__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Wow it succeeded !");      


/*	GLfloat verts[] =
	{
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f, 1.0f, 0.0f,

	};


glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	

	int programId = ShaderHandler::InstallShaders(VertexShader, FragmentShader);
	
__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Cleared Stuff Programm ID = %d" , programId);      

	 VertexId = glGetAttribLocation(programId, "position");
	__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "GetAttribLocation Position %d", VertexId);      


	//GLint FragmentShaderTextureId = glGetAttribLocation(programId, "vertexUV");

	//__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "GetAttribLocation VertexUV %d",FragmentShaderTextureId );      

	glEnableVertexAttribArray(VertexId);
	//glEnableVertexAttribArray(FragmentShaderTextureId);


__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Generating Buffers");      
	glGenBuffers(1, &bufId);
	//glBindBuffer(GL_ARRAY_BUFFER, bufId);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(gTriangleVertices), gTriangleVertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);


__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Bind Buffers");      



__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "SIZE OF ARRAY %d " , sizeof(gTriangleVertices) );      


	glVertexAttribPointer(
	  VertexId,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	   2,                  // size
	   GL_FLOAT,           // type
	   GL_FALSE,           // normalized?
	   0,                  // stride
	    gTriangleVertices            // array buffer offset
	);

		GLenum err = glGetError();


	__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Finished Initialising Errors : %d" , err );      
	glViewport(0, 0, 620, 1180);
	*/
}
