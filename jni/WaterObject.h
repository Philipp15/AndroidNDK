#pragma once

#include "ThreeDObjectBase.h"

class WaterObject : public ThreeDObjectBase
{
public:

	WaterObject(Camera * const  cam) : ThreeDObjectBase(cam,"WaterManyVertices.obj"){};

	void Render();

	void PrepareGraphics();

	Shaders GetShaderType()
	{
		return Shaders::WaterShader;
	}

};

