#ifndef SHADERFACTORY_H
#define SHADERFACTORY_H

#include "ShaderHandler.h"
#include <map>
#include "ShaderDetails.h"

enum Shaders{ WaterShader=0, StandardShader=1 };

class ShaderFactory
{


public:
	
	static const ShaderDetails* GetProgramId(Shaders shaderType)
	{
Shaders shaderInstance;
		switch (shaderType)
		{
		case   StandardShader:  // Shaders::StandardShader:
			return ShaderprogId[StandardShader];

		case WaterShader: //Shaders::WaterShader:
			return ShaderprogId[WaterShader];
		}
	}

	static void InstallShaders()
	{
		Shaders shaderInstance;
		ShaderprogId.insert(std::make_pair(StandardShader,
			ShaderHandler::InstallShaders
			("StandardVertexShaderCode.glsl",
			"StandardFragmentShaderCode.glsl")));

		ShaderprogId.insert(std::make_pair(Shaders::WaterShader,
			ShaderHandler::InstallShaders
			("WaterVertexShaderCode.glsl",
			"WaterFragmentShaderCode.glsl")));
	}
private:
	static std::map<Shaders,const ShaderDetails*> ShaderprogId;


};

#endif