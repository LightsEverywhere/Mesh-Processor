#include "pch.h"
#include "Material.h"

CMaterial::CMaterial(void)
{
	ambientRef = CRGB(0.5, 0.5, 0.5);
	diffuseRef = CRGB(0.8, 0.8, 0.8);
	specularRef = CRGB(1.0, 1.0, 1.0);
	specularExponent = 200;// 高光指数
}

CMaterial::~CMaterial(void)
{
}

void CMaterial::SetAmbientRef(CRGB ambientRef)
{
	this->ambientRef = ambientRef;
}

void CMaterial::SetDiffuseRef(CRGB diffuseRef)
{
	this->diffuseRef = diffuseRef;
}

void CMaterial::SetSpecularRef(CRGB specularRef)
{
	this->specularRef = specularRef;
}

void CMaterial::SetSpecularExponent(double specularExponent)
{
	this->specularExponent = specularExponent;
}
