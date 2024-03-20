#include "pch.h"
#include "RGB.h"

CRGB::CRGB(void)
{
	red = 0.0;
	green = 0.0;
	blue = 0.0;
}

CRGB::~CRGB(void)
{
}

CRGB::CRGB(double red, double green, double blue)
{
	this->red = red;
	this->green = green;
	this->blue = blue;
}

void CRGB::Normalize(void)
{
	red = red < 0.0 ? 0.0 : (red > 1.0 ? 1.0 : red);
	green = green < 0.0 ? 0.0 : (green > 1.0 ? 1.0 : green);
	blue = blue < 0.0 ? 0.0 : (blue > 1.0 ? 1.0 : blue);
}

CRGB operator+(const CRGB& c0, const CRGB& c1)
{
	CRGB color;
	color.red = c0.red + c1.red;
	color.green = c0.green + c1.green;
	color.blue = c0.blue + c1.blue;
	return color;
}

CRGB operator-(const CRGB& c0, const CRGB& c1)
{
	CRGB color;
	color.red = c0.red - c1.red;
	color.green = c0.green - c1.green;
	color.blue = c0.blue - c1.blue;
	return color;
}

CRGB operator-(double num, const CRGB& c)
{
	CRGB color;
	color.red = num - c.red;
	color.green = num - c.green;
	color.blue = num - c.blue;
	return color;
}

CRGB operator*(const CRGB& c0, const CRGB& c1)
{
	CRGB color;
	color.red = c0.red * c1.red;
	color.green = c0.green * c1.green;
	color.blue = c0.blue * c1.blue;
	return color;
}

CRGB operator*(double scalar, const CRGB& c)
{
	CRGB color;
	color.red = c.red * scalar;
	color.green = c.green * scalar;
	color.blue = c.blue * scalar;
	return color;
}

CRGB operator*(const CRGB& c, double scalar)
{
	CRGB color;
	color.red = c.red * scalar;
	color.green = c.green * scalar;
	color.blue = c.blue * scalar;
	return color;
}

CRGB operator/(const CRGB& c, double scalar)
{
	CRGB color;
	color.red = c.red / scalar;
	color.green = c.green / scalar;
	color.blue = c.blue / scalar;
	return color;
}

CRGB operator/(const CRGB& c0, const CRGB& c1)
{
	CRGB color;
	color.red = c0.red / c1.red;
	color.green = c0.green / c1.green;
	color.blue = c0.blue / c1.blue;
	return color;
}

CRGB operator+=(CRGB& c0, CRGB& c1)
{
	c0.red += c1.red;
	c0.green += c1.green;
	c0.blue += c1.blue;
	return c0;
}

CRGB operator-=(CRGB& c0, CRGB& c1)
{
	c0.red -= c1.red;
	c0.green -= c1.green;
	c0.blue -= c1.blue;
	return c0;
}

CRGB operator*=(CRGB& c0, CRGB& c1)
{
	c0.red *= c1.red;
	c0.green *= c1.green;
	c0.blue *= c1.blue;
	return c0;
}

CRGB operator*=(CRGB& c, double scalar)
{
	c.red *= scalar;
	c.green *= scalar;
	c.blue *= scalar;
	return c;
}

CRGB operator/=(CRGB& c, double scalar)
{
	c.red /= scalar;
	c.green /= scalar;
	c.blue /= scalar;
	return c;
}
