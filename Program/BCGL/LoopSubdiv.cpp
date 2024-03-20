#include "pch.h"
#include "LoopSubdiv.h"

CLoopSubdiv::CLoopSubdiv(int nTotalTriangle, int nTotalVertex, const int* vertexIndices, const CP3* P, int nLevels) :resultModel(NULL)
{
	this->nLevels = nLevels;
	// Allocate LoopSubdiv vertices and faces
	SDVertex* verts = new SDVertex[nTotalVertex];// 注意内存分配！！！！！！！！！！！
	vertsMemory.push_back(verts);// 将指针存入顶点组内存池
	for (int i = 0; i < nTotalVertex; i++)
	{
		verts[i] = SDVertex(P[i]);
		vertices.push_back(&verts[i]);
	}
	SDFace* fs = new SDFace[nTotalTriangle];// 注意内存分配！！！！！！！！！！！
	facesMemory.push_back(fs);// 将指针存入面片组内存池
	for (int i = 0; i < nTotalTriangle; i++)
	{
		faces.push_back(&fs[i]);
	}

	// Set face to vertex pointers
	const int* vp = vertexIndices;
	for (int i = 0; i < nTotalTriangle; i++)
	{
		SDFace* f = faces[i];
		for (int j = 0; j < 3; j++)
		{
			SDVertex* v = vertices[vp[j]];
			f->v[j] = v;
			v->startFace = f;
		}
		vp += 3;
	}

	//Set neighbor pointers in faces
	std::set<SDEdge> edges;
	for (int i = 0; i < nTotalTriangle; i++)
	{
		SDFace* f = faces[i];
		for (int edgeNum = 0; edgeNum < 3; edgeNum++)
		{
			// Update neighbor pointer for edgeNum
			int v0 = edgeNum, v1 = NEXT(edgeNum);
			SDEdge e(f->v[v0], f->v[v1]);
			if (edges.find(e) == edges.end())
			{
				// Handle new edge
				e.f[0] = f;
				e.f0EdgeNum = edgeNum;
				edges.insert(e);
			}
			else
			{
				// Handle previously seen edge
				e = *edges.find(e);// 获取了集合中“兄弟边”的信息
				e.f[0]->f[e.f0EdgeNum] = f;
				f->f[edgeNum] = e.f[0];
				edges.erase(e);
			}
		}
	}

	// Finish vertex initialization
	for (int i = 0; i < nTotalVertex; i++)
	{
		SDVertex* v = vertices[i];
		SDFace* f = v->startFace;
		do
		{
			f = f->nextFace(v);
		} while (f && f != v->startFace);
		v->boundary = (f == NULL);
		if (!v->boundary && v->valence() == 6)
			v->regular = TRUE;
		else if (v->boundary && v->valence() == 4)
			v->regular = TRUE;
		else
			v->regular = FALSE;
	}
}

CLoopSubdiv::~CLoopSubdiv(void)
{
	delete[] vertsMemory[0];
	vertsMemory[0] = NULL;
	delete[] facesMemory[0];
	facesMemory[0] = NULL;
	for (int i = 0; i < vertMemory.size(); i++)
	{
		delete vertMemory[i];
		vertMemory[i] = NULL;
	}
	for (int i = 0; i < faceMemory.size(); i++)
	{
		delete faceMemory[i];
		faceMemory[i] = NULL;
	}
	if (resultModel != NULL)
	{
		delete resultModel;
		resultModel = NULL;
	}
}

void CLoopSubdiv::Subdivide() const
{
	std::vector<SDVertex*> v = vertices;
	std::vector<SDFace*> f = faces;
	for (int i = 0; i < nLevels; i++)
	{
		// Update f and v for next level of subdivision
		std::vector<SDFace*> newFaces;
		std::vector<SDVertex*> newVertices;
		// - Allocate next level of children in mesh tree
		for (uint32_t j = 0; j < v.size(); j++)
		{
			v[j]->child = new SDVertex();// 注意内存分配！！！！！！！！！！！
			v[j]->child->regular = v[j]->regular;
			v[j]->child->boundary = v[j]->boundary;
			newVertices.push_back(v[j]->child);

			vertMemory.push_back(v[j]->child);// 将指针存入顶点内存池
		}
		for (uint32_t j = 0; j < f.size(); j++)
		{
			for (int nChild = 0; nChild < 4; nChild++)
			{
				f[j]->childern[nChild] = new SDFace();// 注意内存分配！！！！！！！！！！！
				newFaces.push_back(f[j]->childern[nChild]);

				faceMemory.push_back(f[j]->childern[nChild]);// 将指针存入面片内存池
			}
		}

		// - Update vertex positions and create new edge vertices
		// -- Update vertex positions for even vertices (更新偶顶点(原有顶点)位置)
		for (uint32_t j = 0; j < v.size(); j++)
		{
			if (!v[j]->boundary)
			{
				// Apply one-ring rule for even vertex
				if (v[j]->regular)
				{
					v[j]->child->P = WeightOneRing(v[j], 1.f / 16.f);
				}
				else
				{
					v[j]->child->P = WeightOneRing(v[j], beta(v[j]->valence()));
				}
			}
			else
			{
				// Apply boundary rule for even vertex
				v[j]->child->P = WeightOneRing(v[j], 1.f / 8.f);
			}
		}
		// -- Compute new odd edge vertices
		std::map<SDEdge, SDVertex*> edgeVerts;
		for (uint32_t j = 0; j < f.size(); j++)
		{
			SDFace* face = f[j];
			for (int k = 0; k < 3; k++)
			{
				// Compute odd vertex on kth edge
				SDEdge edge(face->v[k], face->v[NEXT(k)]);
				SDVertex* vert = edgeVerts[edge];
				if (vert==NULL)
				{
					// Create and initialize new odd vertex
					vert = new SDVertex();// 注意内存分配！！！！！！！！！！！
					newVertices.push_back(vert);
					vertMemory.push_back(vert);
					vert->regular = TRUE;
					vert->boundary = (face->f[k] == NULL);
					vert->startFace = face->childern[3];
					// Apply edge rules to compute new vertex position
					if (vert->boundary)
					{
						CP3 ap0 = 0.5f * edge.v[0]->P;
						CP3 ap1 = 0.5f * edge.v[1]->P;
						vert->P = ap0 + ap1;
						// 有问题的写法 += 不执行
						//vert->P = 0.5f * edge.v[0]->P;
						//vert->P += 0.5f * edge.v[1]->P;
					}
					else
					{
						CP3 ap0 = (3.f / 8.f) * edge.v[0]->P;
						CP3 ap1 = (3.f / 8.f) * edge.v[1]->P;
						CP3 ap2 = (1.f / 8.f) * face->OtherVertex(edge.v[0], edge.v[1])->P;
						CP3 ap3 = (1.f / 8.f) * face->f[k]->OtherVertex(edge.v[0], edge.v[1])->P;
						vert->P = ap0 + ap1 + ap2 + ap3;


						//vert->P = 3.f / 8.f * edge.v[0]->P
						//	+ 3.f / 8.f * edge.v[1]->P
						//	+ 1.f / 8.f * face->OtherVertex(edge.v[0], edge.v[1])->P
						//	+ 1.f / 8.f * face->f[k]->OtherVertex(edge.v[0], edge.v[1])->P;
						// 同样是有问题的写法 +=不执行
						//CP3 resultP = 3.f / 8.f * edge.v[0]->P;
						//resultP += 3.f / 8.f * edge.v[1]->P;
						//resultP += 1.f / 8.f * face->OtherVertex(edge.v[0], edge.v[1])->P;
						//resultP += 1.f / 8.f * face->f[k]->OtherVertex(edge.v[0], edge.v[1])->P;
						//vert->P = resultP;
						// 有问题的写法 += 不执行
						//vert->P = 3.f / 8.f * edge.v[0]->P;
						//vert->P += 3.f / 8.f * edge.v[1]->P;
						//vert->P += 1.f / 8.f * face->OtherVertex(edge.v[0], edge.v[1])->P;
						//vert->P += 1.f / 8.f * face->f[k]->OtherVertex(edge.v[0], edge.v[1])->P;
					}
					edgeVerts[edge] = vert;
				}
			}
		}
		// - Update new mesh topology
		// -- Update even vertex face pointers
		for (uint32_t j = 0; j < v.size(); j++)
		{
			SDVertex* vert = v[j];
			int vertNum = vert->startFace->vnum(vert);
			vert->child->startFace = vert->startFace->childern[vertNum];
		}
		// -- Update face neighbor pointers
		for (uint32_t j = 0; j < f.size(); j++)
		{
			SDFace* face = f[j];
			for (int k = 0; k < 3; k++)
			{
				// Update children f pointers for siblings
				face->childern[3]->f[k] = face->childern[NEXT(k)];
				face->childern[k]->f[NEXT(k)] = face->childern[3];
				// Update childern f pointers for neighbor childern
				SDFace* f2 = face->f[k];
				face->childern[k]->f[k] = f2 ? f2->childern[f2->vnum(face->v[k])] : NULL;
				f2 = face->f[PREV(k)];
				face->childern[k]->f[PREV(k)] = f2 ? f2->childern[f2->vnum(face->v[k])] : NULL;
			}
		}
		// -- Update face vertex pointers
		for (uint32_t j = 0; j < f.size(); j++)
		{
			SDFace* face = f[j];
			for (int k = 0; k < 3; k++)
			{
				// Update child vertex_pointer to new even vertex
				face->childern[k]->v[k] = face->v[k]->child;
				// Update child vertex_pointer to new odd vertex
				SDVertex* vert = edgeVerts[SDEdge(face->v[k], face->v[NEXT(k)])];
				face->childern[k]->v[NEXT(k)] = vert;
				face->childern[NEXT(k)]->v[k] = vert;
				face->childern[3]->v[k] = vert;
			}
		}
		// - Prepare for next level of subdivision
		f = newFaces;
		v = newVertices;
	}

	outVertices = v;
	outFaces = f;
}

CModel* CLoopSubdiv::OutputResModel()
{
	InitResultModel();
	for (int i = 0; i < resultModel->nTotalVertex; i++)
	{
		resultModel->vertex[i] = outVertices[i]->P;
	}
	for (int i = 0; i < resultModel->nTotalTriangle; i++)
	{
		for (int k = 0; k < 3; k++)
		{
			resultModel->triangle[i].vertexIndex[k] = GetVertIndex(outFaces[i]->v[k]);
		}
	}
	return resultModel;
}

float CLoopSubdiv::beta(int valence)
{
	if (valence == 3) return 3.f / 16.f;
	else return 3.f / (8.f * valence);
}

CP3 CLoopSubdiv::WeightOneRing(SDVertex* vert, float beta) const
{
	// Put vert one-ring in Pring
	int valence = vert->valence();
	CP3* Pring = new CP3[valence];
	vert->OneRing(Pring);

	CP3 P = (1 - valence * beta) * vert->P;
	for (int i = 0; i < valence; i++)
	{
		CP3 addP = beta * Pring[i];
		P += addP;
		// 有问题的写法 += 不执行
		//P += beta * Pring[i];
	}
	delete[] Pring;
	Pring = NULL;

	return P;
}

void CLoopSubdiv::InitResultModel()
{
	if (resultModel != NULL)
	{
		delete resultModel;
		resultModel = NULL;
	}
	resultModel = new CModel(outVertices.size(), outFaces.size());
}

int CLoopSubdiv::GetVertIndex(SDVertex* vert)
{
	std::vector<SDVertex*>::iterator it = find(outVertices.begin(), outVertices.end(), vert);
	if(it != outVertices.end())
		return 	it - outVertices.begin();
	return -1;
}
