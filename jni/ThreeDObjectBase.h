#ifndef THREEDOBJECTBASE_H
#define THREEDOBJECTBASE_H

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglplatform.h>

#include  <GLES2/gl2.h>
#include  <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>



#include <string>
#include "ShaderHandler.h"


#include "objMtlLoader.h"
//#include "BmpLoader.h"
#include <string>
#include "ModelPositioning.h"
#include <map>
#include "Camera.h"


#include <string>
#include "ShaderFactory.h"
 class ThreeDObjectBase
{
public:

	ThreeDObjectBase( Camera  * const cam, const std::string modelFile) : camera(cam),
		 modelFileName(modelFile){};

	virtual ~ThreeDObjectBase()
	{
		delete[] RawGraphicsData;
		RawGraphicsData = NULL;
	
		delete data;
		data = NULL;
	
	};

	virtual Shaders GetShaderType()
	{
		//Shaders ShaderInstance; return standard shader
	//Shaders sh(1);	
		return (Shaders) 1;
	}

	glm::mat4 getCenterMatrix()
	{
		return CenterMatrix;
	}


	void setCenterMatrix(const glm::mat4 &newMatrix)
	{
		CenterMatrix = newMatrix;
	}

	void changeCenterMatrix(const glm::mat4  &transformMatrix)
	{
		CenterMatrix *= transformMatrix;
	}

	bool hasStandardShader()
	{
		return StdShader;
	
	}

	virtual void PrepareGraphics();
	virtual void Render();

protected:

	GLuint programId;
	MeshMultipleTextures * data;
	Camera  * const camera; //never delete or change the camera 
	GLfloat * RawGraphicsData;

	glm::mat4 CenterMatrix;
	glm::vec3 eye;
	int indexSize;

	GLushort * indexes;

	std::map<GLuint, size_t> IndexPerTexture;

	
	//virtual int InstallShader(const std::string&);

	GLuint indexBufferId = 0;
	GLuint vertexBufferId = 0;


	GLint VertexShaderId =0;
	GLint FragmentShaderTextureId =0;

	const std::string modelFileName;


	GLuint AttribVertexPositionLocId;
	GLuint AttribTextureLocId;


private:
	bool StdShader = true;
};


#endif