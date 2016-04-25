#include "Renderer.h"

#include "Cube.h"
#include "WaterObject.h"
#include "Ship.h"


#include <jni.h>
#include <android/log.h>
//#include "WaterObject.h"
//#include "Ship.h"
//#include "Boat.h"
//#include "SkyBox.h"


#define APPNAME "OpenGLEngine"


GLuint programId;

int program;


std::map<std::string,ThreeDObjectBase*> v3DObjects;


void chngeObjPosition(const std::string objectName, const glm::mat4& changePosition)
{
	if (!v3DObjects.empty())
	{
			v3DObjects.at(objectName)->changeCenterMatrix(changePosition);
	}
}


void Renderer::Init()
{
__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, " Calling install shaders: ' ");

	ShaderFactory::InstallShaders();

	ThreeDObjectBase *cube = new Cube(camera);

__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, " Initalising objects now: ' ");
	
ThreeDObjectBase *water = new WaterObject(camera);

__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, " Initialised water: ' ");

v3DObjects.insert(std::pair<std::string, ThreeDObjectBase*>(std::string("Water"), water));

//ThreeDObjectBase *ship = new Ship(camera);

	//v3DObjects.insert(std::pair<std::string, ThreeDObjectBase*>(std::string("Ship"), ship));


__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, " Initialised Cube: ' ");

	v3DObjects.insert(std::pair<std::string,ThreeDObjectBase*>(std::string("Cube"),cube));


	__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, " Finished Initialising Water: ");
	
}

void Renderer::Prepare()
{
	__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, " Preparing water: ' ");
	for (std::map<std::string, ThreeDObjectBase*>::iterator obj = v3DObjects.begin(); obj != v3DObjects.end(); obj++)
	{
		obj->second->PrepareGraphics();
	}
	glEnable(GL_DEPTH_TEST);
	//glDepthMask(GL_FALSE);
	glFrontFace(GL_CW);
	__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, " Prepared water: ' ");

chngeObjPosition("Cube", glm::translate(glm::vec3(0.0f, 4.0f, 3.5f)));

//cheap trick for moving the camera for now
for (int i = 0; i < 40; i++)
	{
		camera->moveUp();
	}

	for (int i = 0; i < 25; i++)
	{
		camera->moveBackward();
	}

}

void Renderer::Render()
{
	//__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, " Entering the renderer: ' ");

	/*glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepthf(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


//__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, " Cleared staff: ' ");


	for (std::map<std::string, ThreeDObjectBase*>::iterator obj = v3DObjects.begin(); obj != v3DObjects.end(); obj++)
	{
		obj->second->Render();
	}
 }

void Renderer::SetCamera(Camera& cam)
{
	camera = &cam;
}


void Renderer::SendDataToOpenGL()
{
	

}
