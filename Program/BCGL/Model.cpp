#include "pch.h"
#include "Model.h"
#define ROUND(d) int(floor(d+0.5))
#define CRGBtoRGB(c) RGB(c.red*255,c.green*255,c.blue*255)

CModel::CModel(void)
{
	fileName = _T("");

	vertex = NULL;
	textureCoord = NULL;
	normal = NULL;
	face = NULL;
	triangle = NULL;

	targetTexture = NULL;

	nTotalVertex = 0;
	nTotalTexture = 0;
	nTotalNormal = 0;
	nTotalFace = 0;
	nTotalTriangle = 0;

	maxY = -10000, maxX = -10000, maxZ = -10000, minY = 10000, minX = 10000, minZ = 10000;
}

CModel::CModel(int nTotalVertex, int nTotalTriangle)
{
	this->nTotalVertex = nTotalVertex;
	this->nTotalTriangle = nTotalTriangle;
	vertex = new CP3[this->nTotalVertex];
	triangle = new CTriangle[this->nTotalTriangle];
}

CModel::~CModel(void)
{
	if (vertex != NULL)
	{
		delete[] vertex;
		vertex = NULL;
	}
	if (textureCoord != NULL)
	{
		delete[] textureCoord;
		textureCoord = NULL;
	}
	if (normal != NULL)
	{
		delete[] normal;
		normal = NULL;
	}
	if (face != NULL)
	{
		delete[] face;
		face = NULL;
	}
	if (triangle != NULL)
	{
		delete[] triangle;
		triangle = NULL;
	}
}

void CModel::GatherStatistics(void)
{
	CStdioFile file;
	if (!file.Open(fileName, CFile::modeRead))
	{
		MessageBoxW(NULL, _T("δ�ҵ�ģ���ļ���"), _T("����"), MB_ICONEXCLAMATION);
		return;
	}
	CString strLine;
	// ���������Ϣ����
	while (file.ReadString(strLine))
	{
		if (strLine[0] == 'v' && strLine[1] == ' ')
		{
			nTotalVertex++;
		}
		if (strLine[0] == 'v' && strLine[1] == 't' && strLine[2] == ' ')
		{
			nTotalTexture++;
		}
		if (strLine[0] == 'v' && strLine[1] == 'n' && strLine[2] == ' ')
		{
			nTotalNormal++;
		}
		if (strLine[0] == 'f' && strLine[1] == ' ')
		{
			nTotalFace++;
		}
	}
	file.Close();
}

void CModel::ReadVertex(void)
{
	CStdioFile file;
	if (!file.Open(fileName, CFile::modeRead))
	{
		return;
	}
	vertex = new CP3[nTotalVertex];
	int index = 0;// ��������
	CString strLine;
	while (file.ReadString(strLine) && index < nTotalVertex)
	{
		if (strLine[0] == 'v' && strLine[1] == ' ')
		{
			if (strLine[2] == ' ')//�ж�v���Ƿ��������ո��
			{
				CString str[3];
				for (int i = 0; i < 3; i++)
				{
					AfxExtractSubString(str[i], strLine, i + 2, ' ');
				}
				vertex[index].x = _wtof(str[0]);
				vertex[index].y = _wtof(str[1]);
				vertex[index].z = _wtof(str[2]);
				// ����ģ�Ͱ�Χ�з�Χ
				if (vertex[index].x > maxX)	maxX = vertex[index].x;
				if (vertex[index].x < minX)	minX = vertex[index].x;
				if (vertex[index].y > maxY)	maxY = vertex[index].y;
				if (vertex[index].y < minY)	minY = vertex[index].y;
				if (vertex[index].z > maxZ)	maxZ = vertex[index].z;
				if (vertex[index].z < minZ)	minZ = vertex[index].z;
				index++;
			}
			if (strLine[2] != ' ')
			{
				CString str[3];
				for (int i = 0; i < 3; i++)
				{
					AfxExtractSubString(str[i], strLine, i + 1, ' ');
				}
				vertex[index].x = _wtof(str[0]);
				vertex[index].y = _wtof(str[1]);
				vertex[index].z = _wtof(str[2]);
				// ����ģ�Ͱ�Χ�з�Χ
				if (vertex[index].x > maxX)	maxX = vertex[index].x;
				if (vertex[index].x < minX)	minX = vertex[index].x;
				if (vertex[index].y > maxY)	maxY = vertex[index].y;
				if (vertex[index].y < minY)	minY = vertex[index].y;
				if (vertex[index].z > maxZ)	maxZ = vertex[index].z;
				if (vertex[index].z < minZ)	minZ = vertex[index].z;
				index++;
			}
		}
	}
	file.Close();
	// ģ�Ͱ�Χ���������
	modelLength = maxX - minX;
	modelWidth = maxZ - minZ;
	modelHeight = maxY - minY;
	centerPoint = CP3((maxX + minX) / 2, (maxY + minY) / 2, (maxZ + minZ) / 2);
}

void CModel::ReadTexture(void)
{
	CStdioFile file;
	if (!file.Open(fileName, CFile::modeRead))
	{
		return;
	}
	textureCoord = new CT2[nTotalTexture];
	CString strLine;
	int index=0;
	while (file.ReadString(strLine) && index < nTotalTexture)
	{
		if (strLine[0] == 'v' && strLine[1] == 't' && strLine[2] == ' ')
		{
			CString str[2];
			for (int i = 0; i < 2; i++)
			{
				AfxExtractSubString(str[i], strLine, i + 1, ' ');
			}
			textureCoord[index].u = _wtof(str[0]);
			textureCoord[index].v = _wtof(str[1]);
			index++;
		}
	}
	file.Close();
}

void CModel::ReadNormal(void)
{
	CStdioFile file;
	if (!file.Open(fileName, CFile::modeRead))
	{
		return;
	}
	normal = new CVector3[nTotalNormal];
	CString strLine;
	int index = 0;// ��������
	while (file.ReadString(strLine) && index < nTotalNormal)
	{
		if (strLine[0] == 'v' && strLine[1] == 'n' && strLine[2] == ' ')
		{
			CString str[3];
			for (int i = 0; i < 3; i++)
			{
				AfxExtractSubString(str[i], strLine, i + 1, ' ');
			}
			normal[index].x = _wtof(str[0]);
			normal[index].y = _wtof(str[1]);
			normal[index].z = _wtof(str[2]);
			index++;
		}
	}
	file.Close();
}

void CModel::ReadFace(void)
{
	CStdioFile file;
	if (!file.Open(fileName, CFile::modeRead))
	{
		return;
	}
	face = new CFace[nTotalFace];
	CString strLine;
	int index = 0;// ��������
	while (file.ReadString(strLine) && index < nTotalFace)
	{
		if (strLine[0] == 'f' && strLine[1] == ' ')
		{
			// �����涥������
			int count = 0;// '/'������
			for (int i = 0; i < strLine.GetLength(); i++)
			{
				if (strLine[i] == '/')
					count++;
			}
			face[index].vertexNumber = count / 2;// �涥����
			face[index].InitializeQueue();// ��ʼ�������������

			CString* str = new CString[face[index].vertexNumber];

			for (int j = 0; j < face[index].vertexNumber; j++)
			{
				AfxExtractSubString(str[j], strLine, j + 1, ' ');
				CString strs[3];
				for (int k = 0; k < 3; k++)
				{
					AfxExtractSubString(strs[k], str[j], k, '/');
				}
				face[index].vertexIndex[j] = _wtof(strs[0])-1;
				face[index].textureIndex[j] = _wtof(strs[1])-1;
				face[index].normalIndex[j] = _wtof(strs[2])-1;
			}

			delete[] str;
			index++;
		}
	}
	file.Close();
}

void CModel::ReadTriangle(void)
{
	// ����ƬԪ����
	for (int nFace = 0; nFace < nTotalFace; nFace++)
	{
		if (face[nFace].vertexNumber == 4)
		{
			nTotalTriangle += 2;
		}
		else
		{
			nTotalTriangle += 1;
		}
	}
	// ��ȡƬԪ����
	triangle = new CTriangle[nTotalTriangle];
	int index = 0;// ƬԪ��������
	for (int nFace = 0; nFace < nTotalFace; nFace++)
	{
		if (face[nFace].vertexNumber == 4)
		{
			// ������
			for (int i = 0; i < 3; i++)
			{
				triangle[index].vertexIndex[i] = face[nFace].vertexIndex[i];
				triangle[index].textureIndex[i] = face[nFace].textureIndex[i];
				triangle[index].normalIndex[i] = face[nFace].normalIndex[i];
			}
			index++;
			for (int i = 0; i < 3; i++)
			{
				if (i == 0)
				{
					triangle[index].vertexIndex[i] = face[nFace].vertexIndex[i];
					triangle[index].textureIndex[i] = face[nFace].textureIndex[i];
					triangle[index].normalIndex[i] = face[nFace].normalIndex[i];
				}
				else
				{
					triangle[index].vertexIndex[i] = face[nFace].vertexIndex[i + 1];
					triangle[index].textureIndex[i] = face[nFace].textureIndex[i + 1];
					triangle[index].normalIndex[i] = face[nFace].normalIndex[i + 1];
				}
			}
			index++;
		}
		if (face[nFace].vertexNumber == 3)
		{
			for (int i = 0; i < 3; i++)
			{
				triangle[index].vertexIndex[i] = face[nFace].vertexIndex[i];
				triangle[index].textureIndex[i] = face[nFace].textureIndex[i];
				triangle[index].normalIndex[i] = face[nFace].normalIndex[i];
			}
			index++;
		}
	}
}

void CModel::GetScreenBox(void)
{
	xLeft = 10000, xRight = -10000, yTop = -10000, yBottom = 10000;// ��ʼ����Χ�з�Χ
	CP3 screenP;
	for (int nVertex = 0; nVertex < nTotalVertex; nVertex++)
	{
		screenP = camera->PerspectiveProjection(vertex[nVertex]);
		//������Ļ��Χ��
		if (screenP.x < xLeft) xLeft = int(floor(screenP.x));
		if (screenP.x > xRight) xRight = int(ceil(screenP.x));
		if (screenP.y < yBottom) yBottom = int(floor(screenP.y));
		if (screenP.y > yTop) yTop = int(ceil(screenP.y));
	}
	screenH = max(abs(yTop), abs(yBottom))*2;
	screenW = max(abs(xRight), abs(xLeft))*2;
}

void CModel::BindTexture(CTexture* targetTexture)
{
	this->targetTexture = targetTexture;
}

void CModel::SetMaterial(CMaterial* material)
{
	this->material = material;
}

void CModel::SetScene(CLighting* scene)
{
	this->scene = scene;
}

void CModel::SetZBuffer(CZBuffer* zbuffer)
{
	this->zbuffer = zbuffer;
}

bool CModel::InsideTriangle(float x, float y, const CP3* point)
{
	CVector3 v[3];
	v[0] = CVector3(point[0], point[1]).Normalized();
	v[1] = CVector3(point[1], point[2]).Normalized();
	v[2] = CVector3(point[2], point[0]).Normalized();
	CVector3 f[3];
	for (int i = 0; i < 3; i++)
	{
		f[i] = CVector3(point[i], CP2(x, y)).Normalized();
	}
	CVector3 n[3];
	n[0] = CrossProduct(v[0], f[0]);
	n[1] = CrossProduct(v[1], f[1]);
	n[2] = CrossProduct(v[2], f[2]);

	if (n[0].z*n[1].z>=-1e-10&& n[1].z * n[2].z >= -1e-10 && n[2].z * n[0].z >= -1e-10)
		return true;
	else
		return false;
}

void CModel::DrawPointCloud(CDC* pDC)
{
	CP3 screenP;
	for (int nVertex = 0; nVertex < nTotalVertex; nVertex++)
	{
		screenP = camera->PerspectiveProjection(vertex[nVertex]);
		pDC->SetPixelV(ROUND(screenP.x), ROUND(screenP.y), RGB(255, 255, 255));
	}
}

void CModel::DrawWireframe(CDC* pDC)
{
	CP3 screenP;
	CPen newPen, * pOldPen;
	newPen.CreatePen(BS_SOLID, 1, RGB(230, 230, 230));
	pDC->SelectObject(&newPen);
	for (int nFace = 0; nFace < nTotalFace; nFace++)// ��ѭ��
	{
		screenP = camera->PerspectiveProjection(vertex[face[nFace].vertexIndex[0]]);
		pDC->MoveTo(ROUND(screenP.x), ROUND(screenP.y));
		for (int nPoint = 1; nPoint < face[nFace].vertexNumber; nPoint++)
		{
			screenP = camera->PerspectiveProjection(vertex[face[nFace].vertexIndex[nPoint]]);
			pDC->LineTo(ROUND(screenP.x), ROUND(screenP.y));
		}
		screenP = camera->PerspectiveProjection(vertex[face[nFace].vertexIndex[0]]);
		pDC->LineTo(ROUND(screenP.x), ROUND(screenP.y));
	}
}

void CModel::DrawTriWireframe(CDC* pDC)
{
	CP3 screenP;
	CPen newPen, * pOldPen;
	newPen.CreatePen(BS_SOLID, 1, RGB(230, 230, 230));
	pDC->SelectObject(&newPen);
	for (int nTriangle = 0; nTriangle < nTotalTriangle; nTriangle++)// ��ѭ��
	{
		screenP = camera->PerspectiveProjection(vertex[triangle[nTriangle].vertexIndex[0]]);
		pDC->MoveTo(ROUND(screenP.x), ROUND(screenP.y));
		for (int nPoint = 1; nPoint < 3; nPoint++)
		{
			screenP = camera->PerspectiveProjection(vertex[triangle[nTriangle].vertexIndex[nPoint]]);
			pDC->LineTo(ROUND(screenP.x), ROUND(screenP.y));
		}
		screenP = camera->PerspectiveProjection(vertex[triangle[nTriangle].vertexIndex[0]]);
		pDC->LineTo(ROUND(screenP.x), ROUND(screenP.y));
	}
}

void CModel::BlinnPhongShader(CDC* pDC)
{
	for (int nTriangle = 0; nTriangle < nTotalTriangle; nTriangle++)
	{
		// ������
		CVector3 sideVector01 = CVector3(vertex[triangle[nTriangle].vertexIndex[0]], vertex[triangle[nTriangle].vertexIndex[1]]).Normalized();
		CVector3 sideVector02 = CVector3(vertex[triangle[nTriangle].vertexIndex[0]], vertex[triangle[nTriangle].vertexIndex[2]]).Normalized();
		CVector3 triangleNormal = CrossProduct(sideVector01, sideVector02).Normalized();// ͼԪ���棩����
		CVector3 viewVector = CVector3(vertex[triangle[nTriangle].vertexIndex[0]], camera->GetEye()).Normalized();// ͼԪ������

		if (DotProduct(viewVector, triangleNormal) >= 0)// �����޳�
		{
			CP3 point[3];
			for (int j = 0; j < 3; j++)
			{
				point[j] = camera->PerspectiveProjection(vertex[triangle[nTriangle].vertexIndex[j]]);
			}
			// ��Χ��
			int Xmax = max(ceil(point[0].x), max(ceil(point[1].x), ceil(point[2].x)));
			int Xmin = min(floor(point[0].x), min(floor(point[1].x), floor(point[2].x)));
			int Ymax = max(ceil(point[0].y), max(ceil(point[1].y), ceil(point[2].y)));
			int Ymin = min(floor(point[0].y), min(floor(point[1].y), floor(point[2].y)));

			for (int x = Xmin; x < Xmax; x++)
			{
				for (int y = Ymin; y < Ymax; y++)
				{
					if ((x + zbuffer->nWidth / 2) < zbuffer->nWidth && (y + zbuffer->nHeight / 2) < zbuffer->nHeight
						&& (x + zbuffer->nWidth / 2) >= 0 && (y + zbuffer->nHeight / 2) >= 0)// zbuffer��Ӧ����
					{
						if (InsideTriangle((float)x + 0.5, (float)y + 0.5, point))
						{
							double Area = point[0].x * (point[1].y - point[2].y) + point[1].x * (point[2].y - point[0].y) + point[2].x * (point[0].y - point[1].y);
							double Area0 = x * (point[1].y - point[2].y) + point[1].x * (point[2].y - y) + point[2].x * (y - point[1].y);
							double Area1 = point[0].x * (y - point[2].y) + x * (point[2].y - point[0].y) + point[2].x * (point[0].y - y);
							double Area2 = point[0].x * (point[1].y - y) + point[1].x * (y - point[0].y) + x * (point[0].y - point[1].y);
							double alpha, beta, gamma;// ��������
							alpha = Area0 / Area, beta = Area1 / Area, gamma = Area2 / Area;
							// ��ǰ��α���
							double depth = point[0].z * alpha + point[1].z * beta + point[2].z * gamma;
							if (depth <= zbuffer->depthBuffer[x + zbuffer->nWidth / 2][y + zbuffer->nHeight / 2])
							{
								zbuffer->depthBuffer[x + zbuffer->nWidth / 2][y + zbuffer->nHeight / 2] = depth;

								// ͸�ӽ���
								double correctAlpha, correctBeta, correctGamma;// ��������
								correctAlpha = alpha / camera->ViewTransform(vertex[triangle[nTriangle].vertexIndex[0]]).z;
								correctBeta = beta / camera->ViewTransform(vertex[triangle[nTriangle].vertexIndex[1]]).z;
								correctGamma = gamma / camera->ViewTransform(vertex[triangle[nTriangle].vertexIndex[2]]).z;
								// ��ǰ�������ռ�����
								CP3 currentWorldPoint;
								currentWorldPoint = (
									correctAlpha * vertex[triangle[nTriangle].vertexIndex[0]]
									+ correctBeta * vertex[triangle[nTriangle].vertexIndex[1]]
									+ correctGamma * vertex[triangle[nTriangle].vertexIndex[2]]
									) / (correctAlpha + correctBeta + correctGamma);
								// ��ǰ������ռ䷨��
								CVector3 currentNormal;
								currentNormal = ((
									correctAlpha * normal[triangle[nTriangle].normalIndex[0]]
									+ correctBeta * normal[triangle[nTriangle].normalIndex[1]]
									+ correctGamma * normal[triangle[nTriangle].normalIndex[2]]
									) / (correctAlpha + correctBeta + correctGamma)).Normalized();

								if (targetTexture != NULL && targetTexture->isImageExist)// ���������
								{
									// ��ǰ�������ռ���������
									CT2 t = (
										correctAlpha * textureCoord[triangle[nTriangle].textureIndex[0]]
										+ correctBeta * textureCoord[triangle[nTriangle].textureIndex[1]]
										+ correctGamma * textureCoord[triangle[nTriangle].textureIndex[2]]
										) / (correctAlpha + correctBeta + correctGamma);

									CRGB diffuseColor = targetTexture->SampleTexture(t);
									material->SetDiffuseRef(diffuseColor);
								}
								CRGB I = scene->SimpleIlluminate(currentWorldPoint, camera->GetEye(), currentNormal, material);
								I.Normalize();
								pDC->SetPixelV(x, y, CRGBtoRGB(I));
							}
						}
					}
				}
			}
		}

	}
}
