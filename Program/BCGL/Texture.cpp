#include "pch.h"
#include "Texture.h"
#define ROUND(d) int(floor(d+0.5))

CTexture::CTexture(void)
{
	isImageExist = FALSE;
}

CTexture::~CTexture(void)
{
}

void CTexture::SetImagePath(CString imgPath)
{
	this->imgPath = imgPath;
}

void CTexture::ReadImage()
{
	if (image.Load(imgPath) != S_OK)
	{
		MessageBox(NULL, _T("Î´ÕÒµ½Í¼Ïñ£¡"), _T("¾¯¸æ"), MB_ICONEXCLAMATION);
		return;
	}
	else
	{
		isImageExist = TRUE;
	}
}

CRGB CTexture::SampleTexture(CT2 textureCoord)
{
	int imgWidth = image.GetWidth(), imgHeight = (image.GetHeight());
	double uCoord = textureCoord.u, vCoord = textureCoord.v;


	int realU = ROUND((imgWidth - 1) * uCoord);
	int realV = imgHeight - ROUND((imgHeight - 1) * vCoord);

	if (realU < 0)
		realU = 0;
	if (realU > imgWidth - 1)
		realU = imgWidth - 1;
	if (realV < 0)
		realV = 0;
	if (realV > imgHeight - 1)
		realV = imgHeight - 1;

	double red = GetRValue(image.GetPixel(realU, realV));
	double green = GetGValue(image.GetPixel(realU, realV));
	double blue = GetBValue(image.GetPixel(realU, realV));

	CRGB texelColor = CRGB(
		GetRValue(image.GetPixel(realU, realV)) / 255.0,
		GetGValue(image.GetPixel(realU, realV)) / 255.0,
		GetBValue(image.GetPixel(realU, realV)) / 255.0
	);

	return texelColor;
}
