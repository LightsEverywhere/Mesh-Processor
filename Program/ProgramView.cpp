
// ProgramView.cpp: CProgramView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Program.h"
#endif

#include "ProgramDoc.h"
#include "ProgramView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CProgramView

IMPLEMENT_DYNCREATE(CProgramView, CView)

BEGIN_MESSAGE_MAP(CProgramView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
//	ON_WM_LBUTTONDOWN()
//	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_32771, &CProgramView::OnImportObj)
	ON_COMMAND(ID_32772, &CProgramView::OnLightCtrl_1)
	ON_COMMAND(ID_32773, &CProgramView::OnLightCtrl_2)
	ON_COMMAND(ID_32779, &CProgramView::OnLightCtrl_3)
	ON_COMMAND(ID_32775, &CProgramView::OnLightCtrl_4)
	ON_COMMAND(ID_32776, &CProgramView::OnAllLightsOn)
	ON_COMMAND(ID_32778, &CProgramView::OnAllLightsOff)
	ON_COMMAND(ID_32780, &CProgramView::OnPointCloudMode)
	ON_COMMAND(ID_32781, &CProgramView::OnWireFrameMode)
	ON_COMMAND(ID_32782, &CProgramView::OnPhongMode)
	ON_COMMAND(ID_32783, &CProgramView::OnLoopSubdivide)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CProgramView 构造/析构

CProgramView::CProgramView() noexcept
{
	// TODO: 在此处添加构造代码
	mainCamera.SetEye(66.4, 51.6);// 设置主相机
	gui.BindCamera(&mainCamera);
	isRButtonDown = FALSE;// 鼠标右键默认弹起
	isMouseWheelMove = FALSE;// 默认鼠标滚轮未滚动
	model = NULL;
	subdivTool = NULL;
	divModel = NULL;
	drawingMode = wireFrame;// 默认线框绘制模式
	isDivided = FALSE;
}

CProgramView::~CProgramView()
{
	if (model != NULL)// 释放模型对象
	{
		delete model;
		model = NULL;
	}
	if (subdivTool != NULL)
	{
		delete subdivTool;
		subdivTool = NULL;
	}
	//if (divModel != NULL)
	//{
	//	delete divModel;
	//	divModel = NULL;
	//}
}

BOOL CProgramView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CProgramView 绘图

void CProgramView::OnDraw(CDC* pDC)
{
	CProgramDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	OffsetClientRect(pDC);
	DoubleBuffer(pDC);
}

void CProgramView::OffsetClientRect(CDC* pDC)
{
	GetClientRect(&rect);
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(rect.Width(), rect.Height());
	pDC->SetViewportExt(rect.Width(), -rect.Height());
	pDC->SetViewportOrg(rect.Width() / 2, rect.Height() / 2);
}

void CProgramView::DoubleBuffer(CDC* pDC)
{
	CDC memDC;// 声明内存DC
	memDC.CreateCompatibleDC(pDC);// 创建一个与显示DC兼容的内存DC
	CBitmap NewBitmap, * pOldBitmap;
	NewBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());// 创建兼容内存位图
	pOldBitmap = memDC.SelectObject(&NewBitmap);// 将兼容位图选入内存DC
	//memDC.FillSolidRect(rect, pDC->GetBkColor());// 设置客户区背景色
	memDC.FillSolidRect(rect, RGB(52, 52, 52));// 深灰
	//memDC.FillSolidRect(rect, RGB(0, 0, 0));
	rect.OffsetRect(-rect.Width() / 2, -rect.Height() / 2);
	memDC.SetMapMode(MM_ANISOTROPIC);// 内存DC自定义坐标系
	memDC.SetWindowExt(rect.Width(), rect.Height());
	memDC.SetViewportExt(rect.Width(), -rect.Height());
	memDC.SetViewportOrg(rect.Width() / 2, rect.Height() / 2);

	DrawObject(&memDC);// 绘制图形

	pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(),
		&memDC, -rect.Width() / 2, -rect.Height() / 2, SRCCOPY);// 将内存DC中的位图拷贝到显示DC

	memDC.SelectObject(pOldBitmap);
	NewBitmap.DeleteObject();
	memDC.DeleteDC();
}

void CProgramView::DrawObject(CDC* pDC)
{
	//gui.DrawCoordinateGrid(pDC);
	if (model != NULL)
	{
		switch (drawingMode)
		{
		case CProgramView::pointCloud:
			model->DrawPointCloud(pDC);
			break;
		case CProgramView::wireFrame:
			if (isDivided)
			{
				divModel->DrawTriWireframe(pDC);
			}
			else
			{
				model->DrawWireframe(pDC);
			}
			break;
		case CProgramView::phongShading:
		{
			if (isRButtonDown)
			{
				model->DrawWireframe(pDC);
			}
			else
			{
				model->GetScreenBox();// 获取模型屏幕包围盒尺寸
				CZBuffer* pZBuffer = new CZBuffer;//申请内存
				int bufferW, bufferH;
				bufferH = min(model->screenH, rect.Height());
				bufferW = min(model->screenW, rect.Width());
				pZBuffer->SetZBufferSize(bufferW, bufferH);
				pZBuffer->InitializeZBuffer();//初始化深度缓冲器
				model->SetMaterial(&material);
				model->SetScene(&scene);
				model->SetZBuffer(pZBuffer);
				model->BlinnPhongShader(pDC);
				delete pZBuffer;
			}
			break;
		}
		default:
			break;
		}
	}
}


// CProgramView 打印

BOOL CProgramView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CProgramView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CProgramView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CProgramView 诊断

#ifdef _DEBUG
void CProgramView::AssertValid() const
{
	CView::AssertValid();
}

void CProgramView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CProgramDoc* CProgramView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CProgramDoc)));
	return (CProgramDoc*)m_pDocument;
}
#endif //_DEBUG


// CProgramView 消息处理程序


void CProgramView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	isRButtonDown = TRUE;
	Invalidate(FALSE);
	CView::OnRButtonDown(nFlags, point);
}


void CProgramView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	isRButtonDown = FALSE;
	Invalidate(FALSE);
	CView::OnRButtonUp(nFlags, point);
}



void CProgramView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (isRButtonDown)
	{
		//SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
		double speed = 1800;
		double angle = 360;
		mainCamera.Phi -= (point.y - lastMousePoint.y) / speed * angle;
		mainCamera.Theta -= (point.x - lastMousePoint.x) / speed * angle;
		if (mainCamera.Phi > 180) mainCamera.Phi = 180;
		if (mainCamera.Phi < 0) mainCamera.Phi = 0;
		lastMousePoint = point;
		mainCamera.InitialParameter();
		Invalidate(FALSE);
	}
	else
	{
		lastMousePoint = point;
	}
	CView::OnMouseMove(nFlags, point);
}


BOOL CProgramView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	mainCamera.R -= zDelta ;
	if (mainCamera.R < 425) mainCamera.R = 425;
	mainCamera.InitialParameter();
	Invalidate(FALSE);
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CProgramView::OnImportObj()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog pFileDlg(TRUE, _T(""), _T(""), OFN_HIDEREADONLY, _T("OBJ Files(*.obj)|*.obj|"), this);
	if (pFileDlg.DoModal() == IDOK)
	{
		if (model != NULL) { delete model; model = NULL; }// 若添加另一个模型时首先释放第一个模型
		if (divModel != NULL) { divModel = NULL; }// 若细分模型指针不为空，先置空
		if (subdivTool != NULL) { delete subdivTool; subdivTool = NULL; }// 若存在细分工具对象，先清除
		model = new CModel();
		model->fileName = pFileDlg.GetPathName();
		model->GatherStatistics();
		model->ReadVertex();
		model->ReadTexture();
		model->ReadNormal();
		model->ReadFace();
		model->ReadTriangle();
		transform.SetMatrix(model->vertex, model->nTotalVertex);
		transform.Translate(-model->centerPoint.x, -model->centerPoint.y, -model->centerPoint.z);//将模型移至屏幕中心点
		double scaleX = 160 / model->modelLength;//计算合适的缩放比例
		double scaleY = 160 / model->modelHeight;
		double nScale = min(scaleX, scaleY);
		transform.Scale(nScale, nScale, nScale);
		model->camera = &mainCamera;// 将模型绑定到主相机
		isDivided = FALSE;
	}
	Invalidate(FALSE);
}


void CProgramView::OnLightCtrl_1()
{
	// TODO: 在此添加命令处理程序代码
	scene.lightSource[0].lightOn = !scene.lightSource[0].lightOn;
	Invalidate(FALSE);
}


void CProgramView::OnLightCtrl_2()
{
	// TODO: 在此添加命令处理程序代码
	scene.lightSource[1].lightOn = !scene.lightSource[1].lightOn;
	Invalidate(FALSE);
}


void CProgramView::OnLightCtrl_3()
{
	// TODO: 在此添加命令处理程序代码
	scene.lightSource[2].lightOn = !scene.lightSource[2].lightOn;
	Invalidate(FALSE);
}


void CProgramView::OnLightCtrl_4()
{
	// TODO: 在此添加命令处理程序代码
	scene.lightSource[3].lightOn = !scene.lightSource[3].lightOn;
	Invalidate(FALSE);
}


void CProgramView::OnAllLightsOn()
{
	// TODO: 在此添加命令处理程序代码
	for (int nLight = 0; nLight < 4; nLight++)
	{
		scene.lightSource[nLight].lightOn = TRUE;
	}
	Invalidate(FALSE);
}


void CProgramView::OnAllLightsOff()
{
	// TODO: 在此添加命令处理程序代码
	for (int nLight = 0; nLight < 4; nLight++)
	{
		scene.lightSource[nLight].lightOn = FALSE;
	}
	Invalidate(FALSE);
}

void CProgramView::OnPointCloudMode()
{
	// TODO: 在此添加命令处理程序代码
	drawingMode = pointCloud;
	Invalidate(FALSE);
}


void CProgramView::OnWireFrameMode()
{
	// TODO: 在此添加命令处理程序代码
	drawingMode = wireFrame;
	Invalidate(FALSE);
}


void CProgramView::OnPhongMode()
{
	// TODO: 在此添加命令处理程序代码
	drawingMode = phongShading;
	Invalidate(FALSE);
}


void CProgramView::OnLoopSubdivide()
{
	// TODO: 在此添加命令处理程序代码
	isDivided = TRUE;

	if (divModel != NULL) { divModel = NULL; }// 若细分模型指针不为空，先置空
	if (subdivTool != NULL) { delete subdivTool; subdivTool = NULL; }// 若存在细分工具对象，先清除

	int* vertexIndices;// 长度为“三角形数x3” 存放的是model中三角形对应点在vertex队列中的位置
	int indexLength = model->nTotalTriangle * 3;
	vertexIndices = new int[indexLength];
	for (int i = 0; i < model->nTotalTriangle; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			vertexIndices[i * 3 + j] = model->triangle[i].vertexIndex[j];
		}
	}

	subdivTool = new CLoopSubdiv(model->nTotalTriangle, model->nTotalVertex, vertexIndices, model->vertex, 1);
	subdivTool->Subdivide();
	divModel = subdivTool->OutputResModel();
	divModel->camera = &mainCamera;// 将模型绑定到主相机


	delete vertexIndices;
	vertexIndices = NULL;

	Invalidate(FALSE);
}


void CProgramView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (isDivided == TRUE)
	{
		int nLevel = -1;
		switch (nChar)
		{
		case 97:
			nLevel = 1;
			break;
		case 98:
			nLevel = 2;
			break;
		case 99:
			nLevel = 3;
			break;
		case 100:
			nLevel = 4;
			break;
		case 101:
			nLevel = 5;
			break;
		default:
			break;
		}
		if (nLevel != -1)
		{
			if (divModel != NULL) { divModel = NULL; }// 若细分模型指针不为空，先置空
			if (subdivTool != NULL) { delete subdivTool; subdivTool = NULL; }// 若存在细分工具对象，先清除

			int* vertexIndices;// 长度为“三角形数x3” 存放的是model中三角形对应点在vertex队列中的位置
			int indexLength = model->nTotalTriangle * 3;
			vertexIndices = new int[indexLength];
			for (int i = 0; i < model->nTotalTriangle; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					vertexIndices[i * 3 + j] = model->triangle[i].vertexIndex[j];
				}
			}

			subdivTool = new CLoopSubdiv(model->nTotalTriangle, model->nTotalVertex, vertexIndices, model->vertex, nLevel);
			subdivTool->Subdivide();
			divModel = subdivTool->OutputResModel();
			divModel->camera = &mainCamera;// 将模型绑定到主相机

			delete vertexIndices;
			vertexIndices = NULL;
		}
		Invalidate(FALSE);
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
