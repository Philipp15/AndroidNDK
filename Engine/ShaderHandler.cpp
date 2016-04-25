      #include "ShaderHandler.h"

#include <GLES2\gl2.h>

std::string ShaderHandler::readShaderCode(std::string filename)
{

	

	std::ifstream inputfile(filename);
	if (!inputfile.good())
	{
		std::cout << "Error file input bad" << std::endl;
		system("pause");
	}

	return std::string(std::istreambuf_iterator<char>(inputfile), std::istreambuf_iterator<char>());
}

bool ShaderHandler::CheckShaderStatus(GLuint shaderId)
{
	GLint compileStatus;

	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);

	if (compileStatus != GL_TRUE)
	{
		GLint infoLoglength;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLoglength);
		char * buffer = new char[infoLoglength];

		GLsizei size;

		glGetShaderInfoLog(shaderId, infoLoglength, &size, buffer);
		std::cout << buffer << std::endl;

		delete[] buffer;
		system("pause");
		return false;
	}

	return true;
}


const ShaderDetails* ShaderHandler::InstallShaders(const std::string& Vertexpath, const std::string& FragmentPath)
{

	

	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);

	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	GLuint programId;

	GLint someEr;

	const char* adapter[1];
	someEr = glGetError();

	std::string VertexShaderCode = readShaderCode(Vertexpath); // "VertexShaderCode.glsl"

	adapter[0] = VertexShaderCode.c_str();
	glShaderSource(vertexShaderId, 1, adapter, 0);

	std::string FragmentShaderCode = readShaderCode(FragmentPath); // "FragmentShaderCode.glsl"

	adapter[0] = FragmentShaderCode.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderId);
	glCompileShader(fragmentShaderID);

	programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderID);

	if (!CheckShaderStatus(vertexShaderId) || !CheckShaderStatus(fragmentShaderID))
	{
	}

	glLinkProgram(programId);

	glUseProgram(programId);

	ShaderDetails *shderDtls = new ShaderDetails(programId, vertexShaderId, fragmentShaderID);

	return shderDtls;

}

