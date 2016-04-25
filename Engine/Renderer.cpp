#include "Renderer.h"

#include "Cube.h"
//#include "WaterObject.h"
//#include "Ship.h"
//#include "Boat.h"
//#include "SkyBox.h"
#pragma once

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
	ShaderFactory::InstallShaders();

	ThreeDObjectBase *cube = new Cube(camera);
	
	v3DObjects.insert(std::pair<std::string,ThreeDObjectBase*>(std::string("Cube"),cube));
	
}

void Renderer::Prepare()
{
	for (std::map<std::string, ThreeDObjectBase*>::iterator obj = v3DObjects.begin(); obj != v3DObjects.end(); obj++)
	{
		obj->second->PrepareGraphics();
	}
	glEnable(GL_DEPTH_TEST);
	//glDepthMask(GL_FALSE);
	glFrontFace(GL_CW);
	
}

void Renderer::Render()
{
	/*glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepthf(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);



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
