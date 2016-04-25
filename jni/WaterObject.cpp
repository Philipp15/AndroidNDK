#include "WaterObject.h"
#include <ctime>
#include "Cube.h"
#include <android/log.h>

#define APPNAME "OpenGLEngine"

float waveTime = 0.00;
float waveHeight = 0.65;

bool incrementHeight = false;

void balanceWaveTime(float &time, float &waveHeight, bool amplify);
bool amplify = true;


void WaterObject::Render()
{
	glUseProgram(programId);

	glm::mat4 ModelViewProject(1.0f);

	glm::mat4 View(1.0f);
	glm::mat4 Model(1.0f);

	glm::mat4 Projection = glm::perspective(45.0f, 1024.0f / 768.0f, 0.1f, 20.0f);

	//Model = CenterMatrix;

	ModelViewProject = Projection * camera->getWorldToViewMatrix() *  Model;

	GLint transform = glGetUniformLocation(programId, "ModelViewProject");

	glUniformMatrix4fv(transform, 1, false, glm::value_ptr(ModelViewProject));;

	GLint waveTimeLoc = glGetUniformLocation(programId, "waveTime");

	GLint waveHeightLoc = glGetUniformLocation(programId, "waveHeight");

	waveTime += 0.005;

	if (waveTime > 2)
	{
		waveTime = 0;
	}

	glUniform1f(waveTimeLoc, waveTime);
	glUniform1f(waveHeightLoc, waveHeight);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);

	glVertexAttribPointer(AttribVertexPositionLocId, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(AttribTextureLocId, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (char *)(sizeof(float) * 3));  // stride of 5 floats describing my colors starting at position 2

	for (std::map<GLuint, size_t>::iterator it = IndexPerTexture.begin(); it != IndexPerTexture.end(); it++)
	{
		glBindTexture(GL_TEXTURE_2D, it->first); // Bind that texture temporarily
		glActiveTexture(it->first);

		glDrawElements(GL_TRIANGLES, it->second , GL_UNSIGNED_SHORT, 0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
	glBindTexture(GL_TEXTURE_2D, NULL);


}

void WaterObject::PrepareGraphics()
{
	__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, " Start WaterObject::PrepareGraphics ");

	const ShaderDetails *shderDetails = ShaderFactory::GetProgramId(GetShaderType());


	__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, " Got shader details so far: ' ");


	FragmentShaderTextureId = shderDetails->FragmentShaderID;
	VertexShaderId = shderDetails->VertexShaderID;
	programId = shderDetails->ProgramID;

__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, " Got all id so far: ' ");


	glUseProgram(programId);

	GLuint textureHandle;

	std::vector<GLfloat> rawData;

__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, " used program: ' ");

	objMtlLoader meshLoader;
	data = meshLoader.LoadFromFile(modelFileName);

__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, " Loaded from file: ' ");

	GLfloat * RawGraphicsData = data->GetRawArrayofValues(true, false);

__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, " Got raw data: ' ");


	IndexPerTexture = data->IndxsPrTxtHndl(programId);

	std::vector<unsigned short> Indexes; // cannot be more than 15 textures any way

	std::vector<glm::vec3> * Vertices = data->GetVertices();

	//CenterMatrix = ModelPositioning::CenterModel(*Vertices);

	indexes = data->getIndexes();

	indexSize = data->GetIndexSize();

	AttribVertexPositionLocId = glGetAttribLocation(programId, "position");

	AttribTextureLocId = glGetAttribLocation(programId, "vertexUV");
	glEnable(GL_DEPTH_TEST);

	glGenBuffers(1, &vertexBufferId);

	glEnableVertexAttribArray(AttribVertexPositionLocId);



	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);

	glBufferData(GL_ARRAY_BUFFER, data->GetVertexSize() * sizeof(float), RawGraphicsData, GL_STATIC_DRAW);

	glGenBuffers(1, &indexBufferId);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * sizeof(GLushort), indexes, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(AttribTextureLocId);

	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
	glBindTexture(GL_TEXTURE_2D, NULL);

}
