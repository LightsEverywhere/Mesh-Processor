#pragma once
#include"P3.h"

class CProjection
{
public:
	CProjection(void);
	virtual ~CProjection(void);
	void SetEye(double Phi,double Theta);// 设置视点
	CP3 GetEye(void);// 获得视点
	void InitialParameter(void);
	CP3 PerspectiveProjection(CP3 WorldPoint);

	CP3 ViewTransform(CP3 WorldPoint);// View变换
	CP3 PerspTransform(CP3 ViewPoint);// 透视Projection变换
	CP3 InverseViewTransform(CP3 ViewPoint);// 逆View变换
public:
	CP3 Eye;// 视点
	double R, Phi, Theta, d;//视点球坐标
	double k[8];// 透视投影常数

	double zNear, zFar;// 近远剪切面
};

