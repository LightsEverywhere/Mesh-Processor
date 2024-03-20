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
	int vnum(SDVertex* vert)// ���ض�Ӧ�����ڱ���Ƭ�е��������
	{
		for (int i = 0; i < 3; i++)
		{
			if (v[i] == vert) return i;
		}
		return -1;
	}

	SDFace* nextFace(SDVertex* vert)// ��������ڱ���Ƭ ��Ӧ�����ڽӵ���һ����Ƭ
	{
		return f[vnum(vert)];
	}

	SDFace* prevFace(SDVertex* vert)// ��������ڱ���Ƭ ��Ӧ�����ڽӵ�ǰһ����Ƭ
	{
		return f[PREV(vnum(vert))];
	}

	SDVertex* nextVert(SDVertex* vert)// ���ر���Ƭ�� ��Ӧ�������һ������
	{
		return v[NEXT(vnum(vert))];
	}

	SDVertex* prevVert(SDVertex* vert)// ���ر���Ƭ�� ��Ӧ�����ǰһ������
	{
		return v[PREV(vnum(vert))];
	}

	SDVertex* OtherVertex(SDVertex* v0, SDVertex* v1)// ���ظñ���ԵĶ���
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
	SDFace* f[3];// �ڽ���Ƭ
	SDFace* childern[4];// �����һϸ�ּ��������Ƭָ��
};

struct SDVertex
{
	// SDVertex Constructor
	SDVertex(CP3 pt = CP3(0.0, 0.0, 0.0))
		:P(pt), startFace(NULL), child(NULL), regular(FALSE), boundary(FALSE) {}

	// SDVertex Methods
	inline int valence()// ���ر�����ļ���
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

	void OneRing(CP3* P)// �����ָ��ָ���㹻����ڴ�ռ䣬�Դ洢Χ��ĳ����ĵ���
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
	CP3 P;// �����λ������
	bool regular, boundary;// ���졢�߽��ж�
	SDFace* startFace;// �����ڽӱ����ָ��
	SDVertex* child;// �����һϸ�ּ�����Ӷ���ָ��
};

struct SDEdge
{
	// SDEdge Constructor
	SDEdge(SDVertex* v0 = NULL, SDVertex* v1 = NULL)
	{
		v[0] = min(v0, v1);
		v[1] = max(v0, v1);
		f[0] = f[1] = NULL;
		f0EdgeNum = -1;// ������f[0]�еĵ�λ
	}

	// SDEdge Methods
	bool operator<(const SDEdge& other) const// ������set.find()����
	{
		if (v[0] == other.v[0])
			return v[1] < other.v[1];
		return v[0] < other.v[0];
	}

	// SDEdge Data
	SDVertex* v[2];
	SDFace* f[2];// ����ñߵ���Ƭ
	int f0EdgeNum;
};

class CLoopSubdiv
{
public:
	CLoopSubdiv(int nTotalTriangle, int nTotalVertex, const int* vertexIndices, const CP3* P, int nLevels);
	virtual ~CLoopSubdiv(void);
	void Subdivide() const;// ϸ�ֲ���
	CModel* OutputResModel();

private:
	static float beta(int valence);
	CP3 WeightOneRing(SDVertex* vert, float beta) const;
	void InitResultModel();
	int GetVertIndex(SDVertex* vert);

private:
	int nLevels;
	std::vector<SDVertex* > vertices;// ����������
	std::vector<SDFace*> faces;// �����������

	mutable std::vector<SDVertex*> outVertices;
	mutable std::vector<SDFace*> outFaces;

	mutable std::vector<SDVertex*> vertsMemory;// �������ڴ��
	mutable std::vector<SDFace*> facesMemory;// ��Ƭ���ڴ��
	mutable std::vector<SDVertex*> vertMemory;// �����ڴ��
	mutable std::vector<SDFace*> faceMemory;// ��Ƭ�ڴ��

	CModel* resultModel;
};

