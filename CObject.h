#ifndef _CObject_HG_
#define _CObject_HG_

#include "global.h"	// For the directX stuff...

// This describes WHERE the object is...
class CObject
{
public:
	CObject();
	virtual ~CObject();
	float PosX;
	float PosY;
	float PosZ;
	float RotX;
	float RotY;
	float RotZ;
	float Scale;
	float red, green, blue;
};

#endif