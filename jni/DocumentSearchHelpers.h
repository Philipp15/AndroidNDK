#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>

namespace DocumentSearchHelpers
{

	const std::string newMtl = "newmtl";
	const std::string texturePath = "map_Kd";

	bool Compare(const std::string line, std::string MeshComponent);
	std::string lookupTextureFromMtl(std::string mtl, std::string objFileName);
	std::string parseMaterial(std::string line);
	std::string ExtractTextureName(std::string texturePath);

	 bool isSlash(char in);

	//std::string lookupTextureFromMtl(std::string mtl, std::string objFileName);
}