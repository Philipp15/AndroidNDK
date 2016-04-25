#include "objMtlLoader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <stdlib.h>
#include "DocumentSearchHelpers.h"
#include <android/log.h>
#include <jni.h>

#include <sstream>
#include <stdlib.h>
const std::string vertexHeader = "v";
const std::string vertexTextureHeader = "vt";
const std::string  vertexNormalHeader = "vn";
const std::string faceHeader = "f";
const std::string useMaterial = "usemtl";


#define APPNAME "OpenGLEngine"



char * LoadAsset( char * directory,  char * filename,  size_t &fileSize);

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

	//__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Parsing Vertex : ' %s '",  coordinates.c_str()	);

	std::string::size_type sz;

	std::string Coords(coordinates);

	char *xEnd;

	float x = strtof(Coords.c_str(), &xEnd);

	//__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "X : ' %f '",  x	);

		char *yEnd;
		float y = strtof(xEnd, &yEnd);

	//__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Y : ' %f '",  y	);

	char *zEnd;

	float z = strtof(yEnd, NULL);

	//__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Z : ' %f '",  z	);

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

	char * xEnd;

	float x = strtof(Coords.c_str(), &xEnd);

//	float x = std::atof(Coords.c_str());

	Coords = Coords.substr(sz, Coords.length());

	float y = strtof(xEnd, NULL);

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


MeshMultipleTextures *  objMtlLoader::LoadFromFile(const std::string& file)
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



	size_t objSize;

	char *cstrFile = new char[file.length() + 1];
	strcpy(cstrFile, file.c_str());

	__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, " obj file : ' %s '", cstrFile);

	char * objFile =  LoadAsset("", cstrFile ,objSize );



//	__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Contents of the obj file : ' %s '", objFile);


std::stringstream ss(objFile);
  std::string line;

  if (objFile != NULL)
  {
    while(std::getline(ss,line,'\n'))
    {
    	//__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "First line : ' %s '", line.c_str());
			if (line.empty())
			{
				continue;
			}

			std::string firstChar;
			firstChar = line[0];

		//__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "First character : ' %s '",  firstChar.c_str()	);

			if (DocumentSearchHelpers::Compare(firstChar, "v"))
			{

				if (DocumentSearchHelpers::Compare(line, vertexHeader))
				{
					line = line.substr(vertexHeader.size(), line.size());

					//__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Pusing to vertex position : ' %s '", line.c_str()	);
					vertexPostion->push_back(AparseVertices(line));

					continue;
				}

				if (DocumentSearchHelpers::Compare(line, vertexTextureHeader))
				{
					line = line.substr(vertexTextureHeader.size(), line.size());

				//	__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Pusing to vertex texture position : ' %s '", line.c_str()	);
					vertexTexture->push_back(AparseTexures(line));

					continue;
				}

				if (DocumentSearchHelpers::Compare(line, vertexNormalHeader))
				{
					line = line.substr(vertexNormalHeader.size(), line.size());
				//	__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Pusing to vertex Normals position : ' %s '", line.c_str()	);
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

					//	__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Pusing to Indexes : ' %s '", line.c_str()	);
						vIndexTexture.push_back(std::pair<size_t, std::string>( Index.size(), textureName) );
						//delegate all faces to this material

						materialName = DocumentSearchHelpers::parseMaterial(line); //get new material name
						textureName = DocumentSearchHelpers::lookupTextureFromMtl(materialName, file);
					}
				}

			}
		//}



    }
  }


	//std::ifstream myfile(file);

	//std::string line;
	//if (myfile.is_open())
	//{




	//	while (getline(myfile, line))
	//	{

			//if (line.empty())
		//		continue;
			//std::string firstChar;
			//firstChar = line[0];


	/*		if (DocumentSearchHelpers::Compare(firstChar, "v"))
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
		//}

		*/
		vIndexTexture.push_back(std::pair<size_t, std::string>(Index.size(), textureName));
		mesh = new MeshMultipleTextures(vertexPostion, vertexTexture,
										vertexNormal, Index, ACompundedIndexes, vIndexTexture);


		//myfile.close();

		return mesh;
	//}



}

