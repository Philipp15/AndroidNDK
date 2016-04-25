#include "MeshData.h"
//#include "Vertex.h"

#include <iostream>
#include <iterator>

#include <algorithm>


typedef std::vector<vectorThree> vecCollection;
typedef std::vector<vectorTwo> veTexturecCollection;


bool TexturesIncluded;
bool NormalsIncluded;

int extraElelements = 0;

MeshData::MeshData(vecCollection * vertexPositions, veTexturecCollection * vertexTetures,
	vecCollection * vertexNormals, std::vector<int> indexs, std::vector<std::string> * CompoundedIndexes, std::string textureName)
{
	IndexSize = indexs.size();

	Vertices = vertexPositions;
	Normals = vertexNormals;
	Textures = vertexTetures;

	StrCompoundIndexes = CompoundedIndexes;
	MeshTextureName = textureName;
}


std::vector<GLfloat> MeshData::GetRawArrayofValues(bool includeTextures, bool includeNormals)
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

	int TotalSize = sizeOfData * Vertices->size();

	float * RawGraphicsData = new float[TotalSize];

	std::vector<float> rawGraphicsData(TotalSize);
	std::vector<bool> positionsOccupied(TotalSize);

	for (IndexIter iter = StrCompoundIndexes->begin();
		iter != StrCompoundIndexes->end(); iter++)
	{
		PrcocessCompoundIndexes(iter, rawGraphicsData, positionsOccupied);
	}

	//int actualSize = rawGraphicsData.size();

	//float * GraphicsData = new float[actualSize];


	//std::copy(rawGraphicsData.begin(), rawGraphicsData.end(), GraphicsData);//_SCL_SECURE_NO_WARNINGS in preprocessor 

	Indexes = new GLushort[VIndexes.size()];


	std::copy(VIndexes.begin(), VIndexes.end(), Indexes);//_SCL_SECURE_NO_WARNINGS in preprocessor 

	/*for (int i = 0; i < VIndexes.size(); i++)
	{
		std::cout << "," << GraphicsData[i] <<std::endl;
		
	}*/

	return rawGraphicsData;

}

bool MeshData::IsDuplicate(int position, std::vector<float> &rawGraphicsData, int textureElement, int NormalElement)
{
	position += 3;

	bool TexturesMatch = false;
	bool NormalsMatch = false;

	/*if (TexturesIncluded)
	{
		vectorTwo texture = Textures->at(textureElement);

		if (rawGraphicsData.at(position) == texture.x && rawGraphicsData.at(position + 1) == texture.y)
		{
			position += 2;
			TexturesMatch = true;
			
		}
	}*/

	if (NormalsIncluded)
	{
		vectorThree normal = Normals->at(NormalElement);

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

void MeshData::PrcocessCompoundIndexes(IndexIter iter, std::vector<float> &rawGraphicsData, std::vector<bool> &positionsOccupied)
{

	std::vector<std::string> elements;

	Helpers::split(*iter, '/', elements);

	int vertexElement = atoi(elements.at(0).c_str()) -1;
	int textureElement = atoi(elements.at(1).c_str()) - 1;
	int normalElement = atoi(elements.at(2).c_str()) - 1;

	vectorThree OrderedVertex = Vertices->at(vertexElement);

	int position = CalculateRawPosition(vertexElement);

	if (positionsOccupied.at(position))
	{

		if (IsDuplicate(position,rawGraphicsData,textureElement, normalElement))
		{
			VIndexes.push_back(vertexElement);
			return;
		}
		else
		{
			extraElelements++;
			int end = Vertices->size() -1 + extraElelements;

			VIndexes.push_back(end);

			AddtoRawData(rawGraphicsData, positionsOccupied, position, OrderedVertex.x);
			AddtoRawData(rawGraphicsData, positionsOccupied, position, OrderedVertex.y);
			AddtoRawData(rawGraphicsData, positionsOccupied, position, OrderedVertex.z);

			/*vectorTwo OrderedTexture = Textures->at(textureElement);

			if (TexturesIncluded)
			{
				AddtoRawData(rawGraphicsData, positionsOccupied, position, OrderedTexture.x);
				AddtoRawData(rawGraphicsData, positionsOccupied, position, OrderedTexture.y);
			}*/

			if (NormalsIncluded)
			{
				vectorThree  OrderedNormal = Normals->at(normalElement);

				AddtoRawData(rawGraphicsData, positionsOccupied, position, OrderedNormal.x);
				AddtoRawData(rawGraphicsData, positionsOccupied, position, OrderedNormal.y);
				AddtoRawData(rawGraphicsData, positionsOccupied, position, OrderedNormal.z);
			}

			return;
		}
		
	}

	VIndexes.push_back(vertexElement);
	
	InjectIntoRawData(rawGraphicsData,positionsOccupied, position, OrderedVertex.x);
	InjectIntoRawData(rawGraphicsData,positionsOccupied, position, OrderedVertex.y);
	InjectIntoRawData(rawGraphicsData,positionsOccupied, position, OrderedVertex.z);


	/*if (TexturesIncluded)
	{
		vectorTwoOrderedTexture = Textures->at(textureElement);

		InjectIntoRawData(rawGraphicsData,positionsOccupied, position, OrderedTexture.x);
		InjectIntoRawData(rawGraphicsData,positionsOccupied, position, OrderedTexture.y);
	}*/

	if (NormalsIncluded)
	{
		vectorThree OrderedNormal = Normals->at(normalElement);

		InjectIntoRawData(rawGraphicsData, positionsOccupied, position, OrderedNormal.x);
		InjectIntoRawData(rawGraphicsData, positionsOccupied, position, OrderedNormal.y);
		InjectIntoRawData(rawGraphicsData, positionsOccupied, position, OrderedNormal.z);
	}

}


int MeshData::CalculateRawPosition(int index)
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

	return (rowSize * index) ;
}


void MeshData::InjectIntoRawData(std::vector<float> &rawGraphicsData, std::vector<bool> &positionsOccupied, int &position, float value)
{
		rawGraphicsData.at(position) = value;
		positionsOccupied.at(position) = true;
		position++;
}


void MeshData::AddtoRawData(std::vector<float> &rawGraphicsData, std::vector<bool> &positionsOccupied, int &position, float value)
{
	rawGraphicsData.push_back(value);
	positionsOccupied.push_back(true);
	position++;
}

