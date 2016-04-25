#include "objMtlLoader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include "DocumentSearchHelpers.h"

const std::string vertexHeader = "v";
const std::string vertexTextureHeader = "vt";
const std::string  vertexNormalHeader = "vn";
const std::string faceHeader = "f";
const std::string useMaterial = "usemtl";

bool isDot(char input)
{
	if (input == '.')
	{
		return true;
	}

	return false;
}

//Not accurate conversion
glm::vec3 AparseVertices(const std::string coordinates)
{
	std::string::size_type sz;

	std::string Coords(coordinates);

	float x = std::stof(Coords, &sz);

	Coords = Coords.substr(sz, Coords.length());

	float y = std::stof(Coords, &sz);

	Coords = Coords.substr(sz, Coords.length());

	float z = std::stof(Coords, &sz);

	return glm::vec3(x, y, z);
}

void AparseIndex(std::string indexLine, std::vector<int> & Index, std::vector<std::string> * ACompundedIndexes)
{
	char delim = ' ';
	std::stringstream ss(indexLine);
	std::string item;
	std::string  temp;
	while (std::getline(ss, item, delim))
	{
		ACompundedIndexes->push_back(item);
		temp = item[0]; // the first element is the vertex index
		Index.push_back(atoi(temp.c_str()) - 1);
	}

}
std::string parseTextureFileName(std::string line)
{
	auto it = std::find_if(line.begin(), line.end(), isspace);

	std::string textureName;

	while (it != line.end())
	{
		textureName += (*it);
		it++;
	}

	auto iter = std::find_if(textureName.begin(), textureName.end(), isDot);

	return "";
}
//Not accurate
glm::vec2 AparseTexures(const std::string coordinates)
{

	std::string::size_type sz;

	std::string Coords(coordinates);

	float x = std::stof(Coords, &sz);

	Coords = Coords.substr(sz, Coords.length());

	float y = std::stof(Coords, &sz);

	return glm::vec2(x, y);
}

std::vector<std::string> &Asplit(const std::string &s, char delim, std::vector<std::string> &elems)
{
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}


MeshMultipleTextures *  objMtlLoader::LoadFromFile(std::string file)
{

	MeshMultipleTextures * mesh; // get Me a pointer to a mesh 
	std::vector< std::pair< size_t, std::string > > vIndexTexture;

	std::vector<std::string> * ACompundedIndexes = new std::vector<std::string>;;

	std::vector<glm::vec3> * vertexPostion = new std::vector<glm::vec3>;
	std::vector<glm::vec2> * vertexTexture = new std::vector<glm::vec2>;
	std::vector<glm::vec3> * vertexNormal = new std::vector<glm::vec3>;
	std::string materialName;
	std::vector<int> Index;

	std::string textureName;

	bool MaterialSet = false;

	std::ifstream myfile(file);

	std::string line;
	if (myfile.is_open())
	{

		while (getline(myfile, line))
		{

			if (line.empty())
				continue;
			std::string firstChar;
			firstChar = line[0];


			if (DocumentSearchHelpers::Compare(firstChar, "v"))
			{

				if (DocumentSearchHelpers::Compare(line, vertexHeader))
				{
					line = line.substr(vertexHeader.size(), line.size());

					vertexPostion->push_back(AparseVertices(line));

					continue;
				}

				if (DocumentSearchHelpers::Compare(line, vertexTextureHeader))
				{
					line = line.substr(vertexTextureHeader.size(), line.size());

					vertexTexture->push_back(AparseTexures(line));

					continue;
				}

				if (DocumentSearchHelpers::Compare(line, vertexNormalHeader))
				{
					line = line.substr(vertexNormalHeader.size(), line.size());

					vertexNormal->push_back(AparseVertices(line));

					continue;
				}
			}
			else{
				if (DocumentSearchHelpers::Compare(line, faceHeader))
				{
					line = line.substr(faceHeader.size() + 1, line.size()); // has space after
					AparseIndex(line, Index, ACompundedIndexes);
				}


				if (DocumentSearchHelpers::Compare(line, useMaterial)) // if we get a line instructing a material rememebr it and process faces until next material name is reached
				{
					if (!MaterialSet) // did not set a material yet so continue parsing faces 
					{
						MaterialSet = true;
						materialName = DocumentSearchHelpers::parseMaterial(line);
						textureName = DocumentSearchHelpers::lookupTextureFromMtl(materialName, file);

					}
					else{ 
						//set the index where the specific texture ends and the texture itself
						vIndexTexture.push_back(std::pair<size_t, std::string>( Index.size(), textureName) );
						//delegate all faces to this material

						materialName = DocumentSearchHelpers::parseMaterial(line); //get new material name
						textureName = DocumentSearchHelpers::lookupTextureFromMtl(materialName, file);
					}
				}

			}
		}
		vIndexTexture.push_back(std::pair<size_t, std::string>(Index.size(), textureName));
		mesh = new MeshMultipleTextures(vertexPostion, vertexTexture,
										vertexNormal, Index, ACompundedIndexes, vIndexTexture);


		myfile.close();

		return mesh;
	}



}

