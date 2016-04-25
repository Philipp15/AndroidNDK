#ifndef CUBE_H
#define CUBE_H

#include "ThreeDObjectBase.h"



class Cube : public ThreeDObjectBase
{
public: 
	Cube(Camera * const  cam) : ThreeDObjectBase( cam,std::string( "untitled.obj" )){};

	~Cube(){};
};

#endif