#include "Helpers.h"


namespace Helpers
{


	void split(const std::string &s, char delim, std::vector<std::string> &elems)
	{
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			elems.push_back(item);
		}
	}


	//void Helpers::printMatrix(const glm::mat4 &matrix)
	//{
	//	for (int j = 0; j < 4; j++)
	//	{
	//		for (int i = 0; i < 4; i++)
	//		{
	//			std::cout << std::to_string(matrix[j][i]) << "  ";
	//		}

	//		std::cout << "\n";
	//	}

	//	std::cout << "\n";
	//	std::cout << "\n";
	//}


}


//
//	std::string Helpers::lookupTextureFromMtl(std::string mtl, std::string objFileName, std::string compare)
//	{
//		// we know that most cases the obj filename and mtl filename are the same
//
//		std::string mtLFileName;
//		std::string foundMaterial;
//		//auto it = std::find_if(objFileName.begin(), objFileName.end(), isDot);
//
//		int dotPos = objFileName.find('.');
//
//		auto fileBegin = objFileName.begin();
//
//		mtLFileName = objFileName.substr(0, dotPos);
//
//
//		mtLFileName += ".mtl";
//
//		std::ifstream myfile(mtLFileName);
//
//		std::string line;
//
//		bool findTexture = false;
//
//		if (myfile.is_open())
//		{
//			while (getline(myfile, line))
//			{
//				if (Compare(line, compare))
//				{
//					foundMaterial = AparseMaterial(line);
//
//					if (strcmp(foundMaterial.c_str(), mtl.c_str()) == 0)
//					{
//						findTexture = true;
//					}
//				}
//
//				if (findTexture)
//				{
//					if (Compare(line, texturePath))
//					{
//						return ExtractTextureName(line);
//
//					}
//				}
//
//
//			}
//
//
//
//
//			return "";
//
//		}
//
//
//}