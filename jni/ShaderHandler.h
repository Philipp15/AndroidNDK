
#ifndef SHADERHANDLER_H
#define SHADERHANDLER_H

//#include  <fstream>
//#include  <iostream>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#include "ShaderDetails.h"
#include <string>

class ShaderHandler
{
public:
	
	static const  ShaderDetails* InstallShaders(const std::string& Vertexpath, const std::string& FragmentPath);


private :
	static std::string readShaderCode(std::string filename);
	static bool CheckShaderStatus(GLuint shaderId);

};

#endif