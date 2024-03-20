#pragma once
#include"Material.h"
#include"LightSource.h"
#include"Vector3.h"

class CLighting
{
public:
	CLighting(void);
	virtual ~CLighting(void);
	void SetLightSource(CLightSource* lightSource);// …Ë÷√π‚‘¥
	CRGB SimpleIlluminate(CP3 point, CP3 mainCamPos, CVector3 normal, const CMaterial* material);
	CRGB CookIlluminate(CP3 point, CP3 mainCamPos, CVector3 normal, const CMaterial* material);
public:
	CLightSource* lightSource;
};

