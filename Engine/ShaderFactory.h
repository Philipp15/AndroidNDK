#pragma once

#include "ShaderHandler.h"
#include <map>
#include "ShaderDetails.h"

enum class Shaders{ WaterShader, StandardShader };

static class ShaderFactory
{


public:
	
	static const ShaderDetails* GetProgramId(Shaders shaderType)
	{
		switch (shaderType)
		{
		case Shaders::StandardShader:
			return ShaderprogId[Shaders::StandardShader];

		case Shaders::WaterShader:
			return ShaderprogId[Shaders::WaterShader];
		}
	}

	static void InstallShaders()
	{
		ShaderprogId.insert(std::make_pair(Shaders::StandardShader,
			ShaderHandler::InstallShaders
			("VertexShaderCode.glsl",
			"FragmentShaderCode.glsl")));

		/*ShaderprogId.insert(std::make_pair(Shaders::WaterShader,
			ShaderHandler::InstallShaders
			("Custom Shaders//WaterVertexShaderCode.glsl",
			"Custom Shaders//WaterFragmentShaderCode.glsl")));*/
	}
private:
	static std::map<Shaders,const ShaderDetails*> ShaderprogId;


};

