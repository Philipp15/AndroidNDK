#pragma once

#include <string>
#include "MeshMultipleTextures.h"
class objMtlLoader
{
public:

	MeshMultipleTextures *  LoadFromFile(std::string file);
};

