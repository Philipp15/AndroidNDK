#include "MeshMultipleTextures.h"
#include <stdlib.h>


#include <jni.h>
#include <android/log.h>
#define APPNAME "OpenGLEngine"
//#include "opengl_jni_Natives.cpp"

typedef std::vector<glm::vec3> vecCollection;
typedef std::vector<glm::vec2> veTexturecCollection;

GLuint LoadBMPTextureFromFile( char * directory, char const * filename,  GLuint& texture);

MeshMultipleTextures::MeshMultipleTextures(vecCollection * vertexPositions, veTexturecCollection * vertexTetures,
	vecCollection * vertexNormals, std::vector<int> indexs, std::vector<std::string> * CompoundedIndexes,
	std::vector<std::pair<size_t, std::string> > indexendpertexture)
{
	IndexSize = indexs.size();

	Vertices = vertexPositions;
	Normals = vertexNormals;
	Textures = vertexTetures;

	StrCompoundIndexes = CompoundedIndexes;

	IndexEndPerTexture = indexendpertexture;
}



GLfloat * MeshMultipleTextures::GetRawArrayofValues(bool includeTextures, bool includeNormals)
{
	TexturesIncluded = includeTextures;

	NormalsIncluded = includeNormals;

	int sizeOfData = 3; //because vertices x,y,z

	if (includeTextures)
	{
		sizeOfData += 2;
	}

	if (includeNormals)
	{
		sizeOfData += 3;
	}


__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Getting size ");      
	int TotalSize = sizeOfData * Vertices->size();

__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Allocating buffer  ");      

	float * RawGraphicsData = new float[TotalSize];

	std::vector<float> rawGraphicsData(TotalSize);
	std::vector<bool> positionsOccupied(TotalSize);

__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Processing Indexes ");      

	for (IndexIter iter = StrCompoundIndexes->begin();
		iter != StrCompoundIndexes->end(); iter++)
	{
		PrcocessCompoundIndexes(iter, rawGraphicsData, positionsOccupied);
	}

	vertexSize = rawGraphicsData.size();

	float * GraphicsData = new float[vertexSize];


__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Copying data over ");      

	std::copy(rawGraphicsData.begin(), rawGraphicsData.end(), GraphicsData);//_SCL_SECURE_NO_WARNINGS in preprocessor 

	Indexes = new GLushort[VIndexes.size()];


	std::copy(VIndexes.begin(), VIndexes.end(), Indexes);//_SCL_SECURE_NO_WARNINGS in preprocessor 


__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Copying indexes over ");      

	return GraphicsData;

}


bool MeshMultipleTextures::IsDuplicate(int position, std::vector<float> &rawGraphicsData, int textureElement, int NormalElement)
{
	position += 3;

	bool TexturesMatch = false;
	bool NormalsMatch = false;

	if (TexturesIncluded)
	{
		glm::vec2 texture = Textures->at(textureElement);

		if (rawGraphicsData.at(position) == texture.x && rawGraphicsData.at(position + 1) == texture.y)
		{
			position += 2;
			TexturesMatch = true;

		}
	}

	if (NormalsIncluded)
	{
		glm::vec3 normal = Normals->at(NormalElement);

		if (rawGraphicsData.at(position) == normal.x && rawGraphicsData.at(position + 1) == normal.y && rawGraphicsData.at(position + 2) == normal.z)
		{
			NormalsMatch = true;
			return TexturesMatch && NormalsMatch;
		}
	}
	else
	{
		return TexturesMatch;
	}

	return false;
}


std::map <GLuint, size_t> MeshMultipleTextures::PrepareTextures(GLuint programId)
{
	GLuint textureHandle =0;

	std::map <GLuint, size_t> TextureMap;

	for (std::vector<std::pair<size_t, std::string> >::iterator it = IndexEndPerTexture.begin(); it != IndexEndPerTexture.end(); it++)
	{

		char const * a = "abc";

		char * b = "aaa";



		//LoadBMPTextureFromFile(b,a,textureHandle);


		__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Loading texutres %s ", it->second.c_str());      

		LoadBMPTextureFromFile("", it->second.c_str(), textureHandle); // bind this texture name to TextureHandle
		
		__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, " texutres Loaded ");      


		//insert this TextureIndexEnd and Texture Hadnle Into map

		TextureMap.insert({ textureHandle , it->first, }); //equivalent std::map<int, int>::value_type(it , textureHandle);

	}

	return TextureMap;
}


void MeshMultipleTextures::PrcocessCompoundIndexes(IndexIter iter, std::vector<float> &rawGraphicsData, std::vector<bool> &positionsOccupied)
{

	std::vector<std::string> elements;

	Helpers::split(*iter, '/', elements);

	int vertexElement = atoi(elements.at(0).c_str()) - 1;
	int textureElement = atoi(elements.at(1).c_str()) - 1;
	int normalElement = atoi(elements.at(2).c_str()) - 1;

	glm::vec3 OrderedVertex = Vertices->at(vertexElement);

	int position = CalculateRawPosition(vertexElement);

	if (positionsOccupied.at(position))
	{

		if (IsDuplicate(position, rawGraphicsData, textureElement, normalElement))
		{
			VIndexes.push_back(vertexElement);
			return;
		}
		else
		{
			extraElelements++;
			int end = Vertices->size() - 1 + extraElelements;

			VIndexes.push_back(end);

			AddtoRawData(rawGraphicsData, positionsOccupied, position, OrderedVertex.x);
			AddtoRawData(rawGraphicsData, positionsOccupied, position, OrderedVertex.y);
			AddtoRawData(rawGraphicsData, positionsOccupied, position, OrderedVertex.z);

			glm::vec2 OrderedTexture = Textures->at(textureElement);

			if (TexturesIncluded)
			{
				AddtoRawData(rawGraphicsData, positionsOccupied, position, OrderedTexture.x);
				AddtoRawData(rawGraphicsData, positionsOccupied, position, OrderedTexture.y);
			}

			if (NormalsIncluded)
			{
				glm::vec3 OrderedNormal = Normals->at(normalElement);

				AddtoRawData(rawGraphicsData, positionsOccupied, position, OrderedNormal.x);
				AddtoRawData(rawGraphicsData, positionsOccupied, position, OrderedNormal.y);
				AddtoRawData(rawGraphicsData, positionsOccupied, position, OrderedNormal.z);
			}

			return;
		}

	}

	VIndexes.push_back(vertexElement);

	InjectIntoRawData(rawGraphicsData, positionsOccupied, position, OrderedVertex.x);
	InjectIntoRawData(rawGraphicsData, positionsOccupied, position, OrderedVertex.y);
	InjectIntoRawData(rawGraphicsData, positionsOccupied, position, OrderedVertex.z);


	if (TexturesIncluded)
	{
		glm::vec2 OrderedTexture = Textures->at(textureElement);

		InjectIntoRawData(rawGraphicsData, positionsOccupied, position, OrderedTexture.x);
		InjectIntoRawData(rawGraphicsData, positionsOccupied, position, OrderedTexture.y);
	}

	if (NormalsIncluded)
	{
		glm::vec3 OrderedNormal = Normals->at(normalElement);

		InjectIntoRawData(rawGraphicsData, positionsOccupied, position, OrderedNormal.x);
		InjectIntoRawData(rawGraphicsData, positionsOccupied, position, OrderedNormal.y);
		InjectIntoRawData(rawGraphicsData, positionsOccupied, position, OrderedNormal.z);
	}

}


int MeshMultipleTextures::CalculateRawPosition(int index)
{
	int rowSize = 3;

	if (TexturesIncluded)
	{
		rowSize += 2;
	}

	if (NormalsIncluded)
	{
		rowSize += 3;
	}

	return (rowSize * index);
}



void MeshMultipleTextures::InjectIntoRawData(std::vector<float> &rawGraphicsData, std::vector<bool> &positionsOccupied, int &position, float value)
{
	rawGraphicsData.at(position) = value;
	positionsOccupied.at(position) = true;
	position++;
}


void MeshMultipleTextures::AddtoRawData(std::vector<float> &rawGraphicsData, std::vector<bool> &positionsOccupied, int &position, float value)
{
	rawGraphicsData.push_back(value);
	positionsOccupied.push_back(true);
	position++;
}

