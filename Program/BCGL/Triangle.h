#pragma once
#include"P3.h"

class CTriangle
{
public:
	CTriangle(void);
	virtual ~CTriangle(void);
public:
	int vertexIndex[3];// 片元顶点索引
	int textureIndex[3];// 片元纹理点索引
	int normalIndex[3];// 片元法向索引
};

