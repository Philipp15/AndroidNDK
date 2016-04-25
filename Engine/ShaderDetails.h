#pragma once

#include <GLES2\gl2.h>

class ShaderDetails
{
public:
	ShaderDetails(GLuint programId, GLuint vertexShaderId, GLuint FragmentShaderId)
	{
		ProgramID = programId;
		VertexShaderID = vertexShaderId;
		FragmentShaderID = FragmentShaderId;
	};

	GLuint ProgramID;
	GLuint VertexShaderID;
	GLuint FragmentShaderID;

	~ShaderDetails();

};

