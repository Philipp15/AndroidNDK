#pragma once
#include <vector>
#include <sstream>
#include <iostream>
#include <string>
//#include "glm\gtx\transform.hpp"
namespace Helpers
{
	void split(const std::string &s, char delim, std::vector<std::string> &elems);
	//void printMatrix(const glm::mat4 &matrix);
	
	//std::string lookupTextureFromMtl(std::string mtl, std::string objFileName);
}