#ifndef MESHMULTIPLETEXTURES_H
#define MESHMULTIPLETEXTURES_H

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include <vector>
#include "Vertex.h"
#include "Helpers.h"
#include  <GLES2\gl2.h>
//#include "BmpLoader.h"

#include <map>

typedef std::vector<glm::vec3> vecCollection;
typedef std::vector<glm::vec2> veTexturecCollection;

typedef std::vector<std::string>::iterator IndexIter;

class MeshMultipleTextures
{
public:
	MeshMultipleTextures(vecCollection *vertexPositions, veTexturecCollection * vertexTetures,
		vecCollection * vertexNormals, std::vector<int> indexs, std::vector<std::string> * CompoundedIndexes,
		std::vector<std::pair<size_t, std::string>> indexendpertexture);

	GLfloat * GetRawArrayofValues(bool includeTextures, bool includeNormals); // get the vertex coordinates concatenated with texture + normals

	GLushort * getIndexes(){ return Indexes; }

	unsigned int GetIndexSize(){ return IndexSize; }

	vecCollection * GetVertices()
	{
		return Vertices;
	}

	std::map<GLuint, size_t> IndxsPrTxtHndl(GLuint ProgramId)
	{
		return PrepareTextures(ProgramId);
	}

	std::string GetMeshTextureName()
	{
		return MeshTextureName;
	}


	GLuint GetVertexSize()
	{
		return vertexSize;
	}

	~MeshMultipleTextures()
	{
		delete[] Vertices;
		delete[] Textures;
		delete[] Normals;
		delete[] Indexes;
		delete[] StrCompoundIndexes;
		//need to delete rawArrayOfValues;
	}

private:

	//float * rawArrayOfValues;
	GLushort * Indexes;

	unsigned int IndexSize;

	vecCollection * Vertices;
	veTexturecCollection * Textures;

	vecCollection * Normals;

	std::vector<int> VIndexes;

	std::vector<std::string> * StrCompoundIndexes;
	void PrcocessCompoundIndexes(IndexIter iter, std::vector<float> &rawGraphicsData, std::vector<bool> &positionsOccupied);

	int CalculateRawPosition(int index);

	void InjectIntoRawData(std::vector<float> &rawGraphicsData, std::vector<bool> &positionsOccupied, int &position, float value);

	void AmmendIndexes();

	bool IsDuplicate(int position, std::vector<float> &rawGraphicsData, int textureElement, int NormalElement);

	void AddtoRawData(std::vector<float> &rawGraphicsData, std::vector<bool> &positionsOccupied, int &position, float value);

	std::string MeshTextureName;

	bool TexturesIncluded;
	bool NormalsIncluded;

	int extraElelements = 0;

	std::vector<std::pair<size_t, std::string>> IndexEndPerTexture;

	std::map <GLuint, size_t> PrepareTextures(GLuint programId);

	GLuint vertexSize;

};

#endif // !MeshMultipleTextures
