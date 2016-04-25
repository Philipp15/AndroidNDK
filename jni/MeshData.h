#ifndef MESHDATA_H
#define MESHDATA_H


//#include "glm\glm.hpp"
//#include "glm\gtc\matrix_transform.hpp"
//#include "glm\gtc\type_ptr.hpp"
#include <vector>
//#include "Vertex.h"
#include "Helpers.h"
#include  <GLES2/gl2.h>

struct vectorThree
{
	float x, y, z;
public:
	vectorThree(float const & a, float const & b, float const & c) :x(a), y(b), z(c){}
};

struct vectorTwo
{

	float x, y;


public:
	vectorTwo(float const & a, float const & b) :x(a), y(b){}

};


typedef std::vector<vectorThree> vecCollection;
typedef std::vector<vectorTwo> veTexturecCollection;

typedef std::vector<std::string>::iterator IndexIter;

class MeshData
{
public:
	MeshData(vecCollection *vertexPositions, veTexturecCollection * vertexTetures,
		vecCollection * vertexNormals, std::vector<int> indexs, std::vector<std::string> * CompoundedIndexes, std::string textureName);

	std::vector<GLfloat> GetRawArrayofValues(bool includeTextures, bool includeNormals); // get the vertex coordinates concatenated with texture + normals

	GLushort * getIndexes(){ return Indexes; }

	unsigned int GetIndexSize(){ return IndexSize; }

	vecCollection * GetVertices()
	{
		return Vertices;
	}

	std::string GetMeshTextureName()
	{
		return MeshTextureName;
	}


	~MeshData()
	{
		delete[] Vertices;
		delete[] Textures;
		delete[] Normals;
		delete[] Indexes;

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
	



};

#endif // !MESHDATA_H
