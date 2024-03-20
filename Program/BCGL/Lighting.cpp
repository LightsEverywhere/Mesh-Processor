#include "pch.h"
#include "Lighting.h"
constexpr auto PI = 3.1415926;

CLighting::CLighting(void)
{
	lightSource = new CLightSource[4];
	lightSource[0].lightPosition = CP3(1000, 1000, 1000);
	lightSource[1].lightPosition = CP3(-1000, 800, 1000);
	lightSource[2].lightPosition = CP3(500, 0, 1000);
	lightSource[3].lightPosition = CP3(-700, -100, 1000);
}

CLighting::~CLighting(void)
{
	if (lightSource != NULL)
	{
		delete[] lightSource;
		lightSource = NULL;
	}
}

void CLighting::SetLightSource(CLightSource* lightSource)
{
	this->lightSource = lightSource;
}

CRGB CLighting::SimpleIlluminate(CP3 point, CP3 mainCamPos,  CVector3 normal, const CMaterial* material)
{
	CRGB result = CRGB(0.0, 0.0, 0.0);

	CVector3 viewVector;// ������
	viewVector = CVector3(point, mainCamPos);
	viewVector.Normalize();

	for (int nLight = 0; nLight < 4; nLight++)
	{
		if (lightSource[nLight].lightOn)
		{
			CVector3 lightVector;// ������
			lightVector = CVector3(point, lightSource[nLight].lightPosition);
			double d = lightVector.Norm();// ��Դ�����������ľ���
			lightVector.Normalize();

			// ���㻷���ⲿ��
			CRGB La, ka, Ia;
			ka = material->ambientRef;
			Ia = lightSource[nLight].ambientInten * 0.25 / (4 * PI * d * d * 0.00000005);
			La = ka * Ia;

			// ����������ⲿ��
			CRGB Ld, kd, Id;
			kd = material->diffuseRef;
			Id = lightSource[nLight].diffuseInten / (4 * PI * d * d * 0.00000005);
			Ld = kd * Id * max(DotProduct(normal, lightVector), 0);

			// ���㾵�淴��ⲿ��
			CRGB Ls, ks, Is;
			CVector3 halfwayVector;
			halfwayVector = (viewVector + lightVector).Normalized();
			ks = material->specularRef;
			Is = lightSource[nLight].specularInten / (4 * PI * d * d * 0.00000005);
			Ls = ks * Is * pow(max(DotProduct(normal, halfwayVector), 0), material->specularExponent);

			result += La + Ld + Ls;
		}
	}
	result.Normalize();
	return result;
}

CRGB CLighting::CookIlluminate(CP3 point, CP3 mainCamPos, CVector3 normal, const CMaterial* material)
{
	return CRGB();
}