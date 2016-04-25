#include "ThreeDObjectBase.h"


void ThreeDObjectBase::Render()
{
	glUseProgram(programId);
	glm::mat4 ModelViewProject(1.0f);
	glm::mat4 View(1.0f);
	glm::mat4 Model(1.0f);

	glm::mat4 Projection = glm::perspective(45.0f, 1024.0f / 768.0f, 0.1f, 15.0f);

	Model = CenterMatrix;

	ModelViewProject = Projection * camera->getWorldToViewMatrix() *  Model;

	GLint transform = glGetUniformLocation(programId, "ModelViewProject");

	glUniformMatrix4fv(transform, 1, false, glm::value_ptr(ModelViewProject));;

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);

	glVertexAttribPointer(AttribVertexPositionLocId, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(AttribTextureLocId, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (char *)(sizeof(float) * 3));  // stride of 5 floats describing my colors starting at position 2

	// mutiple textures first bind texture, draw triangles for it, bind next texture draw triangles for it etc..
	for (std::map<GLuint, size_t>::iterator it = IndexPerTexture.begin(); it != IndexPerTexture.end(); it++)
	{
		glBindTexture(GL_TEXTURE_2D, it->first); // Bind that texture temporarily
		glActiveTexture(it->first);

		glDrawElements(GL_TRIANGLES, it->second, GL_UNSIGNED_SHORT, 0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
	glBindTexture(GL_TEXTURE_2D, NULL);

}


void ThreeDObjectBase::PrepareGraphics()
{
	//Get my shaderType (for the base it will always be standard one)
	const ShaderDetails *shderDetails = ShaderFactory::GetProgramId(GetShaderType());

	FragmentShaderTextureId = shderDetails->FragmentShaderID;
	VertexShaderId = shderDetails->VertexShaderID;
	programId = shderDetails->ProgramID;

	glUseProgram(programId);

	GLuint textureHandle;

	std::vector<GLfloat> rawData;

	objMtlLoader meshLoader;
	data = meshLoader.LoadFromFile(modelFileName); // Load file

	GLfloat * RawGraphicsData = data->GetRawArrayofValues(true, false); // get vertices raw 

	IndexPerTexture = data->IndxsPrTxtHndl(programId);  // get Texture handles for each texture

	std::vector<unsigned short> Indexes; // cannot be more than 15 textures any way

	std::vector<glm::vec3> * Vertices = data->GetVertices();

	CenterMatrix = ModelPositioning::CenterModel(*Vertices);

	indexes = data->getIndexes();

	indexSize = data->GetIndexSize();

	AttribVertexPositionLocId = glGetAttribLocation(programId, "position");

	AttribTextureLocId = glGetAttribLocation(programId, "vertexUV");
	glEnable(GL_DEPTH_TEST);

	glGenBuffers(1, &vertexBufferId);

	glEnableVertexAttribArray(AttribVertexPositionLocId);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	//indexSize
	glBufferData(GL_ARRAY_BUFFER, data->GetVertexSize() *  sizeof(float), RawGraphicsData, GL_STATIC_DRAW);

	glGenBuffers(1, &indexBufferId);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * sizeof(GLushort), indexes, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(AttribTextureLocId);

	//Unbind all
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
	glBindTexture(GL_TEXTURE_2D, NULL);

}

//
//int ThreeDObjectBase::InstallShader(const std::string& shaderPath)
//{
//    programId = ShaderHandler::InstallShaders("VertexShaderCode.glsl", "FragmentShaderCode.glsl");
//
//
//	return programId;
//}

