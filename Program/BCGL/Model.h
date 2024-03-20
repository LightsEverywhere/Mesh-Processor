#pragma once
#include"P3.h"
#include"Face.h"
#include"Projection.h"
#include"Material.h"
#include"Vector3.h"
#include"ZBuffer.h"
#include"Triangle.h"
#include"Lighting.h"
#include"Texture.h"

class CModel
{
public:
	CModel(void);
	CModel(int nTotalVertex, int nTotalTriangle);
	virtual ~CModel(void);
	void GatherStatistics(void);// ͳ�Ƹ�������
	void ReadVertex(void);// ���붥����Ϣ
	void ReadTexture(void);// ������ʲ�����Ϣ
	void ReadNormal(void);// ���뷨����Ϣ
	void ReadFace(void);// ��������Ϣ
	void ReadTriangle(void);// ��ȡƬԪ��Ϣ
	void GetScreenBox(void);// ����ģ��ͶӰ�����Ļ��Χ��			//���Կ�����ΪԤ�����ֱ�ӱ����޳�+������
	void BindTexture(CTexture* targetTexture);// ���������

	void SetMaterial(CMaterial* material);
	void SetScene(CLighting* scene);
	void SetZBuffer(CZBuffer* zbuffer);
	bool InsideTriangle(float x, float y,const CP3* point);// �������ڲ��ж�

	void DrawPointCloud(CDC* pDC);// ���Ƶ���
	void DrawWireframe(CDC* pDC);// �����߿�
	void DrawTriWireframe(CDC* pDC);// �����߿�
	void BlinnPhongShader(CDC* pDC);// Blinn-Phong�������� rasterization
public:
	CString fileName;// obj�ļ���
	// ģ������
	CP3* vertex;// �������
	CT2* textureCoord;// ������������
	CVector3* normal;// �������
	CFace* face;// �����
	CTriangle* triangle;// ͼԪ(������)����
	int nTotalVertex;// ��������
	int nTotalTexture;// �������������
	int nTotalNormal;// ��������
	int nTotalFace;// ������
	int nTotalTriangle;// �����Σ�ͼԪ������
	float maxY, maxX, maxZ, minY, minX, minZ;// ģ�����Χ��
	float modelLength, modelWidth, modelHeight;// ģ�͵ĳ����
	CP3 centerPoint;//ģ�����ĵ�
	int xLeft, xRight, yTop, yBottom;// ͶӰ�����Ļ��Χ��
	int screenH, screenW;// ͶӰ����Ļ��Χ�гߴ�
	// ģ������
	CMaterial* material;// �������
	CTexture* targetTexture;// Ŀ������
	// ģ�������
	CZBuffer* zbuffer;// Ŀ����Ȼ�����
	CProjection* camera;// �������
	CLighting* scene;// ���ڹ��ճ���
};

