#pragma once
#include"RGB.h"

class CMaterial
{
public:
	CMaterial(void);
	virtual ~CMaterial(void);
	void SetAmbientRef(CRGB ambientRef);// 设置环境反射率
	void SetDiffuseRef(CRGB diffuseRef);// 设置漫反射率
	void SetSpecularRef(CRGB specularRef);// 设置镜面反射率
	void SetSpecularExponent(double n);
public:
	CRGB ambientRef;// 环境反射率
	CRGB diffuseRef;// 漫反射率
	CRGB specularRef;// 镜面反射率
	double specularExponent;// 高光指数

	double f;// Fresnel反射率
	double k;// 表面粗糙度
};

