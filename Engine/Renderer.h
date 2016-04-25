#pragma once

#include <EGL\egl.h>
#include <EGL\eglext.h>
#include <EGL\eglplatform.h>

#include  <GLES2\gl2.h>
#include  <GLES2\gl2ext.h>

#include <string>
#include "ShaderHandler.h"


//#include "ObjLoader.h"
//#include "TextureLoader.h"
#include "ModelPositioning.h"
//#include "bmpTextureLoader.h"
#include "Camera.h"

#include "glm\glm.hpp"
#include "glm\gtx\rotate_vector.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

#include "BmpLoader.h"
#include <map>
#include "objMtlLoader.h"
#include "ShaderFactory.h"
#include "glm\gtx\rotate_vector.hpp"



#include "glm\gtx\rotate_vector.hpp"


class Renderer
{
public:
	void Render();
	void Prepare();
	void SetCamera(Camera& cam);
	void Init();
private:

	glm::mat4 CenterMatrix;
	glm::vec3 eye;
	Camera *camera;

	int indexSize = 0;

//	int InstallShaders();
	void SendDataToOpenGL();



};

