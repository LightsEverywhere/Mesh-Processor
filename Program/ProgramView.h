
// ProgramView.h: CProgramView 类的接口
//

#pragma once
#include"BCGL/GUI.h"
#include"BCGL/Model.h"
#include"BCGL/Transform3.h"
#include"BCGL/LoopSubdiv.h"

class CProgramView : public CView
{
protected: // 仅从序列化创建
	CProgramView() noexcept;
	DECLARE_DYNCREATE(CProgramView)

// 特性
public:
	CProgramDoc* GetDocument() const;

// 操作
public:
	void OffsetClientRect(CDC* pDC);
	void DoubleBuffer(CDC* pDC);// 双缓冲
	void DrawObject(CDC* pDC);
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CProgramView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CRect rect;
	CProjection mainCamera;// 主相机
	CGUI gui;
	CModel* model;// 外部导入模型对象
	CLoopSubdiv* subdivTool;// 细分工具
	CModel* divModel;// 细分结果对象
	bool isDivided;
	enum drawingMode {// 绘制模式
		pointCloud,
		wireFrame,
		phongShading
	} drawingMode;
	CTransform3 transform;// 变换对象
	CMaterial material;// (BRDF)名称待定
	CLighting scene;// 光照场景
	CZBuffer* zbuffer;// z缓冲区
	BOOL isRButtonDown;
	BOOL isMouseWheelMove;
	CPoint lastMousePoint;
// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnImportObj();// obj文件导入
	afx_msg void OnLightCtrl_1();// 光源开关控制
	afx_msg void OnLightCtrl_2();
	afx_msg void OnLightCtrl_3();
	afx_msg void OnLightCtrl_4();
	afx_msg void OnAllLightsOn();
	afx_msg void OnAllLightsOff();
	afx_msg void OnPointCloudMode();
	afx_msg void OnWireFrameMode();
	afx_msg void OnPhongMode();
	afx_msg void OnLoopSubdivide();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // ProgramView.cpp 中的调试版本
inline CProgramDoc* CProgramView::GetDocument() const
   { return reinterpret_cast<CProgramDoc*>(m_pDocument); }
#endif

