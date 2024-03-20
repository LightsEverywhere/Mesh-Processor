#pragma once
#include"RGB.h"
#include"T2.h"

class CTexture
{
public:
	CTexture(void);
	virtual ~CTexture(void);
	void SetImagePath(CString imgPath);
	void ReadImage();
	CRGB SampleTexture(CT2 textureCoord);
public:
	CString imgPath;
	CImage image;
	BOOL isImageExist;
};

