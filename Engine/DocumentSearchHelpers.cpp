#include "DocumentSearchHelpers.h"



bool DocumentSearchHelpers::Compare(const std::string line, std::string MeshComponent)
{
	if (line.length() < MeshComponent.length())
	{
		return false;
	}

	int i = 0;

	while (line[i])
	{
		char c = line[i];
		if (!isspace(c))
		{
			i++;
		}
		else
		{
			break;
		}
		if (i == std::string::npos)
			return false;
	}

	std::string lineBegin = line.substr(0, i);

	if (MeshComponent.compare(lineBegin) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}





std::string DocumentSearchHelpers::parseMaterial(std::string line)
{
	auto it = std::find_if(line.begin(), line.end(), isspace);
	it += 1; // miss the space
	std::string materialName;

	while (it != line.end())
	{
		materialName += (*it);
		it++;
	}

	return materialName;
}


bool DocumentSearchHelpers::isSlash(char in)
{
	if (in == '\\')
	{
		return true;
	}

	return false;
}


std::string DocumentSearchHelpers::ExtractTextureName(std::string texturePath)
{

	std::string::const_reverse_iterator position = std::find_if(texturePath.crbegin(), texturePath.crend(), isSlash);

	//position.base() convert to iter from reverse iter when converting will always point to n+1
	return std::string(position.base(), texturePath.end());
}


std::string DocumentSearchHelpers::lookupTextureFromMtl(std::string mtl, std::string objFileName)
{
	// we know that most cases the obj filename and mtl filename are the same

	std::string mtLFileName;
	std::string foundMaterial;
	//auto it = std::find_if(objFileName.begin(), objFileName.end(), isDot);

	int dotPos = objFileName.find('.');

	auto fileBegin = objFileName.begin();

	mtLFileName = objFileName.substr(0, dotPos);

	mtLFileName += ".mtl";

	std::ifstream myfile(mtLFileName); // LoadFile

	std::string line;

	bool findTexture = false;

	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			if (DocumentSearchHelpers::Compare(line, newMtl))
			{
				foundMaterial = parseMaterial(line);

				if (strcmp(foundMaterial.c_str(), mtl.c_str()) == 0)
				{
					findTexture = true;
				}
			}

			if (findTexture)
			{
				if (DocumentSearchHelpers::Compare(line, texturePath))
				{
					return ExtractTextureName(line);

				}
			}
		}

		return "";
	}
}


