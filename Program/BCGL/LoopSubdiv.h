#pragma once
#include<vector>
#include<set>
#include<map>
#include<algorithm>
#include"P3.h"
#include"Model.h"

#define NEXT(i) (((i)+1)%3)
#define PREV(i) (((i)+2)%3)

struct SDVertex;
struct SDEdge;

struct SDFace
{
	// SDFace Constructor
	SDFace()
		:v{ NULL,NULL,NULL }, f{ NULL,NULL,NULL }, childern{ NULL,NULL,NULL,NULL } {}

	// SDFace Methods
	int vnum(SDVertex* vert)// 返回对应顶点在本面片中的索引编号
	{
		for (int i = 0; i < 3; i++)
		{
			if (v[i] == vert) return i;
		}
		return -1;
	}

	SDFace* nextFace(SDVertex* vert)// 返回相对于本面片 对应顶点邻接的下一个面片
	{
		return f[vnum(vert)];
	}

	SDFace* prevFace(SDVertex* vert)// 返回相对于本面片 对应顶点邻接的前一个面片
	{
		return f[PREV(vnum(vert))];
	}

	SDVertex* nextVert(SDVertex* vert)// 返回本面片中 对应顶点的下一个顶点
	{
		return v[NEXT(vnum(vert))];
	}

	SDVertex* prevVert(SDVertex* vert)// 返回本面片中 对应顶点的前一个顶点
	{
		return v[PREV(vnum(vert))];
	}

	SDVertex* OtherVertex(SDVertex* v0, SDVertex* v1)// 返回该边相对的顶点
	{
		for (int i = 0; i < 3; i++)
		{
			if (v[i] != v0 && v[i] != v1)
			{
				return v[i];
			}
		}
		return NULL;
	}

	// SDFace Data
	SDVertex* v[3];
	SDFace* f[3];// 邻接面片
	SDFace* childern[4];// 针对下一细分级别的子面片指针
};

struct SDVertex
{
	// SDVertex Constructor
	SDVertex(CP3 pt = CP3(0.0, 0.0, 0.0))
		:P(pt), startFace(NULL), child(NULL), regular(FALSE), boundary(FALSE) {}

	// SDVertex Methods
	inline int valence()// 返回本顶点的价数
	{
		SDFace* f = startFace;
		if (!boundary)
		{
			// Compute valence of interior vertex
			int nf = 1;
			while ((f = f->nextFace(this)) != startFace)
			{
				nf++;
			}
			return nf;
		}
		else
		{
			// Compute valence of boundary vertex
			int nf = 1;
			while ((f = f->nextFace(this)) != NULL)
			{
				nf++;
			}
			f = startFace;
			while ((f = f->prevFace(this)) != NULL)
			{
				nf++;
			}
			return nf + 1;
		}
	}

	void OneRing(CP3* P)// 传入的指针指向足够大的内存空间，以存储围绕某顶点的单环
	{
		if (!boundary)
		{
			// Get one-ring vertices for interior vertex
			SDFace* face = startFace;
			do
			{
				*P++ = face->nextVert(this)->P;
				face = face->nextFace(this);
			} while (face != startFace);
		}
		else
		{
			// Get one-ring vertices for boundary vertex
			SDFace* face = startFace, * f2;
			while ((f2 = face->nextFace(this)) != NULL)
			{
				face = f2;
			}
			*P++ = face->nextVert(this)->P;
			do
			{
				*P++ = face->prevVert(this)->P;
				face = face->prevFace(this);
			} while (face != NULL);
		}
	}

	// SDVertex Data
	CP3 P;// 顶点的位置坐标
	bool regular, boundary;// 奇异、边界判定
	SDFace* startFace;// 任意邻接表面的指针
	SDVertex* child;// 针对下一细分级别的子顶点指针
};

struct SDEdge
{
	// SDEdge Constructor
	SDEdge(SDVertex* v0 = NULL, SDVertex* v1 = NULL)
	{
		v[0] = min(v0, v1);
		v[1] = max(v0, v1);
		f[0] = f[1] = NULL;
		f0EdgeNum = -1;// 本边在f[0]中的地位
	}

	// SDEdge Methods
	bool operator<(const SDEdge& other) const// 可用于set.find()方法
	{
		if (v[0] == other.v[0])
			return v[1] < other.v[1];
		return v[0] < other.v[0];
	}

	// SDEdge Data
	SDVertex* v[2];
	SDFace* f[2];// 共享该边的面片
	int f0EdgeNum;
};

class CLoopSubdiv
{
public:
	CLoopSubdiv(int nTotalTriangle, int nTotalVertex, const int* vertexIndices, const CP3* P, int nLevels);
	virtual ~CLoopSubdiv(void);
	void Subdivide() const;// 细分操作
	CModel* OutputResModel();

private:
	static float beta(int valence);
	CP3 WeightOneRing(SDVertex* vert, float beta) const;
	void InitResultModel();
	int GetVertIndex(SDVertex* vert);

private:
	int nLevels;
	std::vector<SDVertex* > vertices;// 网格点表数据
	std::vector<SDFace*> faces;// 网格面表数据

	mutable std::vector<SDVertex*> outVertices;
	mutable std::vector<SDFace*> outFaces;

	mutable std::vector<SDVertex*> vertsMemory;// 顶点组内存池
	mutable std::vector<SDFace*> facesMemory;// 面片组内存池
	mutable std::vector<SDVertex*> vertMemory;// 顶点内存池
	mutable std::vector<SDFace*> faceMemory;// 面片内存池

	CModel* resultModel;
};

