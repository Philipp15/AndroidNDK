
#ifndef MODELPOSITIONING_H 
#define MODELPOSITIONING_H

#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"




class ModelPositioning
{
public:
	 glm::mat4 CenterModel(std::vector<glm::vec3> vertices);

private:

	template <class CompareFunction>
	float Find(const std::vector<glm::vec3> & vertices, CompareFunction  a);
	



};

#endif