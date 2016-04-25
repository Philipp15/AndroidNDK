#pragma once
#include "ThreeDObjectBase.h"
class Ship : public ThreeDObjectBase
{
public:
	Ship(Camera * const  cam) : ThreeDObjectBase(cam, std::string("Ship50.obj")){};

	~Ship(){};
};

