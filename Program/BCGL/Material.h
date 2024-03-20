#pragma once
#include"RGB.h"

class CMaterial
{
public:
	CMaterial(void);
	virtual ~CMaterial(void);
	void SetAmbientRef(CRGB ambientRef);// ���û���������
	void SetDiffuseRef(CRGB diffuseRef);// ������������
	void SetSpecularRef(CRGB specularRef);// ���þ��淴����
	void SetSpecularExponent(double n);
public:
	CRGB ambientRef;// ����������
	CRGB diffuseRef;// ��������
	CRGB specularRef;// ���淴����
	double specularExponent;// �߹�ָ��

	double f;// Fresnel������
	double k;// ����ֲڶ�
};

