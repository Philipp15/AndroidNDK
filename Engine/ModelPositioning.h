
#pragma once

#include <vector>
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"




class ModelPositioning
{
public:
	static glm::mat4 CenterModel(std::vector<glm::vec3> vertices);

private:

	template <class CompareFunction>
	static float Find(const std::vector<glm::vec3> & vertices, CompareFunction & a);
	



};

