#pragma once
#include"../framework.h"

class CRGB
{
public:
	CRGB(void);
	virtual ~CRGB(void);
	CRGB(double red, double green, double blue);
	void Normalize(void);// 归一化到[0，1]区间
	// 运算符重载
	friend CRGB operator+(const CRGB& c0, const CRGB& c1);
	friend CRGB operator-(const CRGB& c0, const CRGB& c1);
	friend CRGB operator-(double num, const CRGB& c);
	friend CRGB operator*(const CRGB& c0, const CRGB& c1);
	friend CRGB operator*(double scalar, const CRGB& c);
	friend CRGB operator*(const CRGB& c, double scalar);
	friend CRGB operator/(const CRGB& c, double scalar);
	friend CRGB operator/(const CRGB& c0, const CRGB& c1);
	friend CRGB operator+=(CRGB& c0, CRGB& c1);
	friend CRGB operator-=(CRGB& c0, CRGB& c1);
	friend CRGB operator*=(CRGB& c0, CRGB& c1);
	friend CRGB operator*=(CRGB& c, double scalar);
	friend CRGB operator/=(CRGB& c, double scalar);
public:
	double red, green, blue;
};

