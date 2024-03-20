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
	void GatherStatistics(void);// 统计各项数据
	void ReadVertex(void);// 读入顶点信息
	void ReadTexture(void);// 读入材质采样信息
	void ReadNormal(void);// 读入法向信息
	void ReadFace(void);// 读入面信息
	void ReadTriangle(void);// 获取片元信息
	void GetScreenBox(void);// 计算模型投影后的屏幕包围盒			//可以考虑作为预处理项，直接背面剔除+面排序
	void BindTexture(CTexture* targetTexture);// 绑定纹理对象

	void SetMaterial(CMaterial* material);
	void SetScene(CLighting* scene);
	void SetZBuffer(CZBuffer* zbuffer);
	bool InsideTriangle(float x, float y,const CP3* point);// 三角形内部判断

	void DrawPointCloud(CDC* pDC);// 绘制点云
	void DrawWireframe(CDC* pDC);// 绘制线框
	void DrawTriWireframe(CDC* pDC);// 绘制线框
	void BlinnPhongShader(CDC* pDC);// Blinn-Phong明暗处理 rasterization
public:
	CString fileName;// obj文件名
	// 模型数据
	CP3* vertex;// 顶点队列
	CT2* textureCoord;// 纹理采样点队列
	CVector3* normal;// 法向队列
	CFace* face;// 面队列
	CTriangle* triangle;// 图元(三角形)队列
	int nTotalVertex;// 顶点总数
	int nTotalTexture;// 纹理采样点总数
	int nTotalNormal;// 法向总数
	int nTotalFace;// 面总数
	int nTotalTriangle;// 三角形（图元）总数
	float maxY, maxX, maxZ, minY, minX, minZ;// 模型体包围盒
	float modelLength, modelWidth, modelHeight;// 模型的长宽高
	CP3 centerPoint;//模型中心点
	int xLeft, xRight, yTop, yBottom;// 投影后的屏幕包围盒
	int screenH, screenW;// 投影后屏幕包围盒尺寸
	// 模型属性
	CMaterial* material;// 物体材质
	CTexture* targetTexture;// 目标纹理
	// 模型相关项
	CZBuffer* zbuffer;// 目标深度缓冲区
	CProjection* camera;// 相机对象
	CLighting* scene;// 所在光照场景
};

