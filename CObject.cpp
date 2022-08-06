#include "CObject.h"

CObject::CObject()
{
	this->PosX = 0.0f;
	this->PosY = 0.0f;
	this->PosZ = 0.0f;
	this->RotX = 0.0f;
	this->RotY = 0.0f;
	this->RotZ = 0.0f;
	this->Scale = 1.0f;
	this->red = this->green = this->blue = 1.0f;		// White
}

CObject::~CObject()
{

}