#include "ThreeDObjectBase.h"
#include <jni.h>
#include <android/log.h>
#define APPNAME "OpenGLEngine"
void ThreeDObjectBase::Render()
{
	glUseProgram(programId);
	glm::mat4 ModelViewProject(1.0f);
	glm::mat4 View(1.0f);
	glm::mat4 Model(1.0f);

//__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Getting perspective ");      

	glm::mat4 Projection = glm::perspective(45.0f, 1024.0f / 768.0f, 0.1f, 15.0f);

	Model = CenterMatrix;

//__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Getting camera getWorldToViewMatrix ");      

	ModelViewProject = Projection * camera->getWorldToViewMatrix() *  Model;

	GLint transform = glGetUniformLocation(programId, "ModelViewProject");

//	__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Render 2");      

	glUniformMatrix4fv(transform, 1, false, glm::value_ptr(ModelViewProject));;

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);

//__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Render 3");      

	glVertexAttribPointer(AttribVertexPositionLocId, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(AttribTextureLocId, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (char *)(sizeof(float) * 3));  // stride of 5 floats describing my colors starting at position 2

	// mutiple textures first bind texture, draw triangles for it, bind next texture draw triangles for it etc..
	for (std::map<GLuint, size_t>::iterator it = IndexPerTexture.begin(); it != IndexPerTexture.end(); it++)
	{
		glBindTexture(GL_TEXTURE_2D, it->first); // Bind that texture temporarily
//		__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "The textureHandle is  %d", it->first );      		
		glActiveTexture(it->first);

		glDrawElements(GL_TRIANGLES, it->second, GL_UNSIGNED_SHORT, 0);

		//__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Drawing");      
	}

	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
	glBindTexture(GL_TEXTURE_2D, NULL);

}


void ThreeDObjectBase::PrepareGraphics()
{

	__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Going into shader details ");      

	//Get my shaderType (for the base it will always be standard one)
	const ShaderDetails *shderDetails = ShaderFactory::GetProgramId(GetShaderType());


GLuint glErrror = glGetError();

	__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Going out of shader details ");      

	FragmentShaderTextureId = shderDetails->FragmentShaderID;
	VertexShaderId = shderDetails->VertexShaderID;
	programId = shderDetails->ProgramID;

__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Using program:  %d , Using VertexShaderID: %d fragmentShaderId: %d", programId, VertexShaderId, FragmentShaderTextureId);      


	glUseProgram(programId);

__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Using program errors: %d", glErrror);


__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Finished using program ");      
	
	GLuint textureHandle;

	std::vector<GLfloat> rawData;

	objMtlLoader meshLoader;
__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Loading model now %s", modelFileName.c_str());      

	data = meshLoader.LoadFromFile(modelFileName); // Load file





	__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Model Loaded");      



	GLfloat * RawGraphicsData = data->GetRawArrayofValues(true, false); // get vertices raw 


		__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Printing vertices");      
		for ( int i =0 ; i <  data->GetVertexSize(); i++)
		{

			__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, " %f ", RawGraphicsData[i] );      			

		
		}

		__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "I AM HERE !! ");      


__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Got Raw Graphics Data ");      

	IndexPerTexture = data->IndxsPrTxtHndl(programId);  // get Texture handles for each texture

	std::vector<unsigned short> Indexes; // cannot be more than 15 textures any way

	std::vector<glm::vec3> * Vertices = data->GetVertices();

	ModelPositioning mdPos;


__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Centering matrix ");      

	CenterMatrix = mdPos.CenterModel(*Vertices);

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
__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Binding buffers ");      
	glGenBuffers(1, &indexBufferId);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * sizeof(GLushort), indexes, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(AttribTextureLocId);

	//Unbind all
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
	glBindTexture(GL_TEXTURE_2D, NULL);
__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Finished Preparing  ");      
}

//
//int ThreeDObjectBase::InstallShader(const std::string& shaderPath)
//{
//    programId = ShaderHandler::InstallShaders("VertexShaderCode.glsl", "FragmentShaderCode.glsl");
//
//
//	return programId;
//}

