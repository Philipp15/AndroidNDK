#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include  "ModelPositioning.h"


struct FindMax
{
	FindMax(char c) : coordinate(c){};

	//Functor
    float operator() (std::vector<glm::vec3>::const_iterator &it, glm::vec3 &v)
	{
		switch (coordinate)
		{
		case 'x':

			if (it->x > v.x)
			{
				v = *it;
				return it->x;
			}
			else
			{
				return v.x;
			}

		case 'y':

			if (it->y > v.y)
			{
				v = *it;
				return it->y;
			}
			else
			{
				return v.y;
			}

		case 'z':

			if (it->z > v.z)
			{
				v = *it;
				return it->z;
			}
			else
			{
				return v.z;
			}
		}
	}

private:
	char coordinate;

};

struct FindMin
{
	FindMin(char c) : coordinate(c){};

	//Functor
	float operator() (std::vector<glm::vec3>::const_iterator &it, glm::vec3 &v)
	{
		switch (coordinate)
		{
		case 'x':

			if (it->x < v.x)
			{
				v = *it;
				return it->x;
			}
			else
			{
				return v.x;
			}
			
		case 'y':

			if (it->y < v.y)
			{
				v = *it;
				return it->y;
			}
			else
			{

				return v.y;
			}

		case 'z':

			if (it->z < v.z)
			{
				v = *it;
				return it->z;
			}
			else
			{
				return v.z;
			}
		}
	}


private:
	char coordinate;

};

 template <class CompareFunction>
	  float ModelPositioning::Find(const std::vector<glm::vec3>& vertices, CompareFunction a)
	 {
		 std::vector<glm::vec3>::const_iterator it;

		 float Value;

		 glm::vec3 first;

		 first = vertices.at(0);

		 for (it = vertices.begin(); it < vertices.end(); it++)
		 {
			 Value = a(it, first);
		 }
		 return Value;
	 }

	 glm::mat4 ModelPositioning::CenterModel(std::vector<glm::vec3> vertices)
	{
		glm::mat4 Identity(1.0f);

		float MaxValueX = Find(vertices, FindMax('x'));
		float MinValueX = Find(vertices, FindMin('x'));

		float MaxValueY = Find(vertices, FindMax('y'));
		float MinValueY = Find(vertices, FindMin('y'));
		
		float MaxValueZ = Find(vertices, FindMax('z'));
		float MinValueZ = Find(vertices, FindMin('z'));

		float centerX = (MaxValueX + MinValueX) /2 * -1;
		float centerY = (MaxValueY + MinValueY) /2 * -1;
		float centerZ = (MaxValueZ + MinValueZ) /2 * -1;

		glm::mat4 centerMatrix =  glm::translate(Identity, glm::vec3(centerX, centerY, centerZ));

		return centerMatrix;
	}

	




