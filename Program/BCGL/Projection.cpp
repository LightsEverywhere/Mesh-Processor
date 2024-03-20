#include "pch.h"
#include "Projection.h"
constexpr auto PI = 3.1415926;

CProjection::CProjection(void)
{
	R = 1000, d = 2000, Phi = 90, Theta = 0;//视点位于屏幕正前方		看向-z方向
	zNear = 800, zFar = 2000;//近剪切面与远剪切面
	InitialParameter();
}

CProjection::~CProjection(void)
{
}

void CProjection::SetEye(double Phi, double Theta)
{
	R = 1000, d = 2000;
	zNear = 800, zFar = 2000;
	this->Phi = Phi;
	this->Theta = Theta;
	InitialParameter();
}

CP3 CProjection::GetEye(void)
{
	return Eye;
}

void CProjection::InitialParameter(void)
{
	k[0] = sin(PI * Theta / 180);//Phi代表θ
	k[1] = sin(PI * Phi / 180);//Phi代表φ
	k[2] = cos(PI * Theta / 180);
	k[3] = cos(PI * Phi / 180);
	k[4] = k[1] * k[2];
	k[5] = k[0] * k[1];
	k[6] = k[2] * k[3];
	k[7] = k[0] * k[3];
	Eye = CP3(R * k[5], R * k[3], R * k[4]);//设置视点
}

CP3 CProjection::PerspectiveProjection(CP3 WorldPoint)
{
	CP3 ViewPoint;//观察坐标系三维点
	ViewPoint.x = k[2] * WorldPoint.x - k[0] * WorldPoint.z;
	ViewPoint.y = -k[7] * WorldPoint.x + k[1] * WorldPoint.y - k[6] * WorldPoint.z;
	ViewPoint.z = -k[5] * WorldPoint.x - k[3] * WorldPoint.y - k[4] * WorldPoint.z + R;
	ViewPoint.c = WorldPoint.c;
	CP3 ScreenPoint;//屏幕坐标系三维点
	ScreenPoint.x = d * ViewPoint.x / ViewPoint.z;
	ScreenPoint.y = d * ViewPoint.y / ViewPoint.z;
	ScreenPoint.z = zFar * (1 - zNear / ViewPoint.z) / (zFar - zNear);
	ScreenPoint.c = ViewPoint.c;
	return ScreenPoint;
}

CP3 CProjection::ViewTransform(CP3 WorldPoint)
{
	CP3 ViewPoint;//观察坐标系三维点
	ViewPoint.x = k[2] * WorldPoint.x - k[0] * WorldPoint.z;
	ViewPoint.y = -k[7] * WorldPoint.x + k[1] * WorldPoint.y - k[6] * WorldPoint.z;
	ViewPoint.z = -k[5] * WorldPoint.x - k[3] * WorldPoint.y - k[4] * WorldPoint.z + R;
	ViewPoint.c = WorldPoint.c;
	return ViewPoint;
}

CP3 CProjection::PerspTransform(CP3 ViewPoint)
{
	CP3 ScreenPoint;//屏幕坐标系三维点
	ScreenPoint.x = d * ViewPoint.x / ViewPoint.z;
	ScreenPoint.y = d * ViewPoint.y / ViewPoint.z;
	ScreenPoint.z = zFar * (1 - zNear / ViewPoint.z) / (zFar - zNear);
	ScreenPoint.c = ViewPoint.c;
	return ScreenPoint;
}

CP3 CProjection::InverseViewTransform(CP3 ViewPoint)
{
	CP3 WorldPoint;
	WorldPoint.x = k[2] * ViewPoint.x - k[7] * ViewPoint.y - k[5] * ViewPoint.z + R * k[5] * ViewPoint.w;
	WorldPoint.y = k[1] * ViewPoint.y - k[3] * ViewPoint.z + R * k[3] * ViewPoint.w;
	WorldPoint.z = -k[0] * ViewPoint.x - k[6] * ViewPoint.y - k[4] * ViewPoint.z + R * k[4] * ViewPoint.w;
	WorldPoint.c = ViewPoint.c;
	return WorldPoint;
}
