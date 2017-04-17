#include "StdAfx.h"
#include "GameObject.h"
#include "Player.h"

CPolygon::CPolygon(int nVertices) 
{ 
	if (nVertices > 0)
	{
		m_nVertices = nVertices; 
		m_pVertices = new CVertex[nVertices]; 
	}
}

CPolygon::~CPolygon() 
{ 
	if (m_pVertices) delete [] m_pVertices; 
}

void CPolygon::SetVertex(int nIndex, CVertex& vertex)
{
	if ((0 <= nIndex) && (nIndex < m_nVertices) && m_pVertices)
	{
		m_pVertices[nIndex] = vertex;
	}
}

void CPolygon::Draw(HDC hDCFrameBuffer, XMFLOAT4X4& xm4x4Transform, CCamera *pCamera)
{
	CVertex vertex;
    XMFLOAT3 xmf3Previous, xmf3Current;
    for (int i = 0; i <= m_nVertices; i++) 
    {
		vertex = m_pVertices[i % m_nVertices];
//World/View/Projection Transformation(Perspective Divide)
//		xmf4Current = Vector3::Transform(vertex.m_xmf3Position, xm4x4Transform);
//		if (xmf4Current.z != 0.0f) { xmf4Current.x = xmf4Current.x / xmf4Current.w; xmf4Current.y = xmf4Current.y / xmf4Current.w; xmf4Current.z = xmf4Current.z / xmf4Current.w; }
		xmf3Current = Vector3::TransformCoord(vertex.m_xmf3Position, xm4x4Transform);
		//Screen Transformation
		xmf3Current.x = +xmf3Current.x * (pCamera->m_Viewport.m_nWidth * 0.5f) + pCamera->m_Viewport.m_xStart + (pCamera->m_Viewport.m_nWidth * 0.5f);
		xmf3Current.y = -xmf3Current.y * (pCamera->m_Viewport.m_nHeight * 0.5f) + pCamera->m_Viewport.m_yStart + (pCamera->m_Viewport.m_nHeight * 0.5f);

		if (i != 0)
		{
			if (((xmf3Previous.z >= 0.0f) && (xmf3Previous.z <= 1.0f) && (xmf3Previous.x >= 0) && (xmf3Previous.x < pCamera->m_Viewport.m_nWidth) && (xmf3Previous.y >= 0) && (xmf3Previous.y < pCamera->m_Viewport.m_nHeight)) || ((xmf3Current.z >= 0.0f) && (xmf3Current.z <= 1.0f) && (xmf3Current.x >= 0) && (xmf3Current.x < pCamera->m_Viewport.m_nWidth) && (xmf3Current.y >= 0) && (xmf3Current.y < pCamera->m_Viewport.m_nHeight)))
			{
				::MoveToEx(hDCFrameBuffer, (long)xmf3Previous.x, (long)xmf3Previous.y, NULL);
				::LineTo(hDCFrameBuffer, (long)xmf3Current.x, (long)xmf3Current.y);
				//::Rectangle(hDCFrameBuffer, (long)xmf3Previous.x, (long)xmf3Previous.y, (long)xmf3Current.x, (long)xmf3Current.y);
			}
		}
		xmf3Previous = xmf3Current;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CMesh::CMesh(int nPolygons)
{
	if (nPolygons > 0)
	{
		m_nPolygons = nPolygons;
		m_ppPolygons = new CPolygon*[nPolygons];
		m_nReferences = 0;
	}
}

CMesh::~CMesh(void)
{
	if (m_ppPolygons)
	{
		for (int i = 0; i < m_nPolygons; i++) if (m_ppPolygons[i]) delete m_ppPolygons[i];
		delete [] m_ppPolygons;
	}
}

void CMesh::SetPolygon(int nIndex, CPolygon *pPolygon)
{
	if ((0 <= nIndex) && (nIndex < m_nPolygons) && m_ppPolygons)
	{
		m_ppPolygons[nIndex] = pPolygon;
	}
}

void CMesh::Render(HDC hDCFrameBuffer, XMFLOAT4X4& xm4x4Transform, CCamera *pCamera)
{
    for (int j = 0; j < m_nPolygons; j++)
    {
        m_ppPolygons[j]->Draw(hDCFrameBuffer, xm4x4Transform, pCamera);
    } 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CLine::CLine() : CMesh(1){

}

CLine::~CLine() {

}

void CLine::setLength(XMFLOAT3 pos1, XMFLOAT3 pos2) {
	CPolygon *line = new CPolygon(2);
	line->SetVertex(0, CVertex(pos1.x, pos1.y, pos1.z));
	line->SetVertex(1, CVertex(pos2.x, pos2.y, pos2.z));
	SetPolygon(0, line);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CCubeMesh::CCubeMesh(float fWidth, float fHeight, float fDepth) : CMesh(6)
{
	float fHalfWidth = fWidth * 0.5f;
	float fHalfHeight = fHeight * 0.5f;
	float fHalfDepth = fDepth * 0.5f;

	CPolygon *pFrontFace = new CPolygon(4);
	pFrontFace->SetVertex(0, CVertex(-fHalfWidth, +fHalfHeight, -fHalfDepth));
	pFrontFace->SetVertex(1, CVertex(+fHalfWidth, +fHalfHeight, -fHalfDepth));
	pFrontFace->SetVertex(2, CVertex(+fHalfWidth, -fHalfHeight, -fHalfDepth));
	pFrontFace->SetVertex(3, CVertex(-fHalfWidth, -fHalfHeight, -fHalfDepth));
	SetPolygon(0, pFrontFace);

	CPolygon *pTopFace = new CPolygon(4);
	pTopFace->SetVertex(0, CVertex(-fHalfWidth, +fHalfHeight, +fHalfDepth));
	pTopFace->SetVertex(1, CVertex(+fHalfWidth, +fHalfHeight, +fHalfDepth));
	pTopFace->SetVertex(2, CVertex(+fHalfWidth, +fHalfHeight, -fHalfDepth));
	pTopFace->SetVertex(3, CVertex(-fHalfWidth, +fHalfHeight, -fHalfDepth));
	SetPolygon(1, pTopFace);

	CPolygon *pBackFace = new CPolygon(4);
	pBackFace->SetVertex(0, CVertex(-fHalfWidth, -fHalfHeight, +fHalfDepth));
	pBackFace->SetVertex(1, CVertex(+fHalfWidth, -fHalfHeight, +fHalfDepth));
	pBackFace->SetVertex(2, CVertex(+fHalfWidth, +fHalfHeight, +fHalfDepth));
	pBackFace->SetVertex(3, CVertex(-fHalfWidth, +fHalfHeight, +fHalfDepth));
	SetPolygon(2, pBackFace);

	CPolygon *pBottomFace = new CPolygon(4);
	pBottomFace->SetVertex(0, CVertex(-fHalfWidth, -fHalfHeight, -fHalfDepth));
	pBottomFace->SetVertex(1, CVertex(+fHalfWidth, -fHalfHeight, -fHalfDepth));
	pBottomFace->SetVertex(2, CVertex(+fHalfWidth, -fHalfHeight, +fHalfDepth));
	pBottomFace->SetVertex(3, CVertex(-fHalfWidth, -fHalfHeight, +fHalfDepth));
	SetPolygon(3, pBottomFace);

	CPolygon *pLeftFace = new CPolygon(4);
	pLeftFace->SetVertex(0, CVertex(-fHalfWidth, +fHalfHeight, +fHalfDepth));
	pLeftFace->SetVertex(1, CVertex(-fHalfWidth, +fHalfHeight, -fHalfDepth));
	pLeftFace->SetVertex(2, CVertex(-fHalfWidth, -fHalfHeight, -fHalfDepth));
	pLeftFace->SetVertex(3, CVertex(-fHalfWidth, -fHalfHeight, +fHalfDepth));
	SetPolygon(4, pLeftFace);

	CPolygon *pRightFace = new CPolygon(4);
	pRightFace->SetVertex(0, CVertex(+fHalfWidth, +fHalfHeight, -fHalfDepth));
	pRightFace->SetVertex(1, CVertex(+fHalfWidth, +fHalfHeight, +fHalfDepth));
	pRightFace->SetVertex(2, CVertex(+fHalfWidth, -fHalfHeight, +fHalfDepth));
	pRightFace->SetVertex(3, CVertex(+fHalfWidth, -fHalfHeight, -fHalfDepth));
	SetPolygon(5, pRightFace);
}

CCubeMesh::~CCubeMesh(void)
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CWallMesh::CWallMesh(float fWidth, float fHeight, float fDepth) : CMesh(32)
{
	float fHalfWidth = fWidth;
	float fHalfHeight = fHeight * 0.5f;
	float fHalfDepth = fDepth;

	float numOfSide = 1;
	float numOfBotton = 2;
	float difference = -3;

	int index = 0;

	CPolygon *pLeftFace;
	for (float i = numOfBotton; i>0; --i) {
		for (float j = numOfSide; j>0; --j) {
			pLeftFace = new CPolygon(4);
			pLeftFace->SetVertex(0, CVertex(-fHalfWidth, +fHalfHeight * (i / numOfBotton),       +fHalfDepth * (j / numOfSide) + i * difference));
			pLeftFace->SetVertex(1, CVertex(-fHalfWidth, +fHalfHeight * (i / numOfBotton),       +fHalfDepth * ((j - 1) / numOfSide) + i * difference));
			pLeftFace->SetVertex(2, CVertex(-fHalfWidth, +fHalfHeight * ((i - 1) / numOfBotton), +fHalfDepth * ((j - 1) / numOfSide) + i * difference));
			pLeftFace->SetVertex(3, CVertex(-fHalfWidth, +fHalfHeight * ((i - 1) / numOfBotton), +fHalfDepth * (j / numOfSide) + i * difference));
			SetPolygon(index++, pLeftFace);

			pLeftFace = new CPolygon(4);
			pLeftFace->SetVertex(0, CVertex(-fHalfWidth, +fHalfHeight * (i / numOfBotton), 0 * (j / numOfSide) + i * difference));
			pLeftFace->SetVertex(1, CVertex(-fHalfWidth, +fHalfHeight * (i / numOfBotton), 0 * ((j - 1) / numOfSide) + i * difference));
			pLeftFace->SetVertex(2, CVertex(-fHalfWidth, +fHalfHeight * ((i - 1) / numOfBotton), 0 * ((j - 1) / numOfSide) + i * difference));
			pLeftFace->SetVertex(3, CVertex(-fHalfWidth, +fHalfHeight * ((i - 1) / numOfBotton), 0 * (j / numOfSide) + i * difference));
			SetPolygon(index++, pLeftFace);

			pLeftFace = new CPolygon(4);
			pLeftFace->SetVertex(0, CVertex(-fHalfWidth, -fHalfHeight * (i / numOfBotton), +fHalfDepth * (j / numOfSide) + i * difference));
			pLeftFace->SetVertex(1, CVertex(-fHalfWidth, -fHalfHeight * (i / numOfBotton), +fHalfDepth * ((j - 1) / numOfSide) + i * difference));
			pLeftFace->SetVertex(2, CVertex(-fHalfWidth, -fHalfHeight * ((i - 1) / numOfBotton), +fHalfDepth * ((j - 1) / numOfSide) + i * difference));
			pLeftFace->SetVertex(3, CVertex(-fHalfWidth, -fHalfHeight * ((i - 1) / numOfBotton), +fHalfDepth * (j / numOfSide) + i * difference));
			SetPolygon(index++, pLeftFace);

			pLeftFace = new CPolygon(4);
			pLeftFace->SetVertex(0, CVertex(-fHalfWidth, -fHalfHeight * (i / numOfBotton),       0 * (j / numOfSide) + i * difference));
			pLeftFace->SetVertex(1, CVertex(-fHalfWidth, -fHalfHeight * (i / numOfBotton),       0 * ((j - 1) / numOfSide) + i * difference));
			pLeftFace->SetVertex(2, CVertex(-fHalfWidth, -fHalfHeight * ((i - 1) / numOfBotton), 0 * ((j - 1) / numOfSide) + i * difference));
			pLeftFace->SetVertex(3, CVertex(-fHalfWidth, -fHalfHeight * ((i - 1) / numOfBotton), 0 * (j / numOfSide) + i * difference));
			SetPolygon(index++, pLeftFace);
		}
	}

	CPolygon *pRightFace;
	for (float i = numOfBotton; i>0; --i) {
		for (float j = numOfSide; j>0; --j) {
			pRightFace = new CPolygon(4);
			pRightFace->SetVertex(0, CVertex(+fHalfWidth, +fHalfHeight * (i / numOfBotton), +fHalfDepth * (j / numOfSide) + i * difference));
			pRightFace->SetVertex(1, CVertex(+fHalfWidth, +fHalfHeight * (i / numOfBotton), +fHalfDepth * ((j - 1) / numOfSide) + i * difference));
			pRightFace->SetVertex(2, CVertex(+fHalfWidth, +fHalfHeight * ((i - 1) / numOfBotton), +fHalfDepth * ((j - 1) / numOfSide) + i * difference));
			pRightFace->SetVertex(3, CVertex(+fHalfWidth, +fHalfHeight * ((i - 1) / numOfBotton), +fHalfDepth * (j / numOfSide) + i * difference));
			SetPolygon(index++, pRightFace);

			pRightFace = new CPolygon(4);
			pRightFace->SetVertex(0, CVertex(+fHalfWidth, -fHalfHeight * (i / numOfBotton), +fHalfDepth * (j / numOfSide) + i * difference));
			pRightFace->SetVertex(1, CVertex(+fHalfWidth, -fHalfHeight * (i / numOfBotton), +fHalfDepth * ((j - 1) / numOfSide) + i * difference));
			pRightFace->SetVertex(2, CVertex(+fHalfWidth, -fHalfHeight * ((i - 1) / numOfBotton), +fHalfDepth * ((j - 1) / numOfSide) + i * difference));
			pRightFace->SetVertex(3, CVertex(+fHalfWidth, -fHalfHeight * ((i - 1) / numOfBotton), +fHalfDepth * (j / numOfSide) + i * difference));
			SetPolygon(index++, pRightFace);

			pRightFace = new CPolygon(4);
			pRightFace->SetVertex(0, CVertex(+fHalfWidth, +fHalfHeight * (i / numOfBotton), 0 * (j / numOfSide) + i * difference));
			pRightFace->SetVertex(1, CVertex(+fHalfWidth, +fHalfHeight * (i / numOfBotton), 0 * ((j - 1) / numOfSide) + i * difference));
			pRightFace->SetVertex(2, CVertex(+fHalfWidth, +fHalfHeight * ((i - 1) / numOfBotton), 0 * ((j - 1) / numOfSide) + i * difference));
			pRightFace->SetVertex(3, CVertex(+fHalfWidth, +fHalfHeight * ((i - 1) / numOfBotton), 0 * (j / numOfSide) + i * difference));
			SetPolygon(index++, pRightFace);

			pRightFace = new CPolygon(4);
			pRightFace->SetVertex(0, CVertex(+fHalfWidth, -fHalfHeight * (i / numOfBotton), 0 * (j / numOfSide) + i * difference));
			pRightFace->SetVertex(1, CVertex(+fHalfWidth, -fHalfHeight * (i / numOfBotton), 0 * ((j - 1) / numOfSide) + i * difference));
			pRightFace->SetVertex(2, CVertex(+fHalfWidth, -fHalfHeight * ((i - 1) / numOfBotton), 0 * ((j - 1) / numOfSide) + i * difference));
			pRightFace->SetVertex(3, CVertex(+fHalfWidth, -fHalfHeight * ((i - 1) / numOfBotton), 0 * (j / numOfSide) + i * difference));
			SetPolygon(index++, pRightFace);
		}
	}
	
	CPolygon *pUpFace;
	for (float i = numOfBotton; i>0; --i) {
		for (float j = numOfSide; j>0; --j) {
			pUpFace = new CPolygon(4);
			pUpFace->SetVertex(0, CVertex(+fHalfWidth * (i / numOfBotton), +fHalfHeight , +fHalfDepth * (j / numOfSide) + i * difference));
			pUpFace->SetVertex(1, CVertex(+fHalfWidth * (i / numOfBotton), +fHalfHeight , +fHalfDepth * ((j - 1) / numOfSide) + i * difference));
			pUpFace->SetVertex(2, CVertex(+fHalfWidth * ((i - 1) / numOfBotton), +fHalfHeight , +fHalfDepth * ((j - 1) / numOfSide) + i * difference));
			pUpFace->SetVertex(3, CVertex(+fHalfWidth * ((i - 1) / numOfBotton), +fHalfHeight , +fHalfDepth * (j / numOfSide) + i * difference));
			SetPolygon(index++, pUpFace);

			pUpFace = new CPolygon(4);
			pUpFace->SetVertex(0, CVertex(+fHalfWidth * (i / numOfBotton), +fHalfHeight , 0 * (j / numOfSide) + i * difference));
			pUpFace->SetVertex(1, CVertex(+fHalfWidth * (i / numOfBotton), +fHalfHeight , 0 * ((j - 1) / numOfSide) + i * difference));
			pUpFace->SetVertex(2, CVertex(+fHalfWidth * ((i - 1) / numOfBotton), +fHalfHeight , 0 * ((j - 1) / numOfSide) + i * difference));
			pUpFace->SetVertex(3, CVertex(+fHalfWidth * ((i - 1) / numOfBotton), +fHalfHeight , 0 * (j / numOfSide) + i * difference));
			SetPolygon(index++, pUpFace);

			pUpFace = new CPolygon(4);
			pUpFace->SetVertex(0, CVertex(-fHalfWidth * (i / numOfBotton), +fHalfHeight, +fHalfDepth * (j / numOfSide) + i * difference));
			pUpFace->SetVertex(1, CVertex(-fHalfWidth * (i / numOfBotton), +fHalfHeight, +fHalfDepth * ((j - 1) / numOfSide) + i * difference));
			pUpFace->SetVertex(2, CVertex(-fHalfWidth * ((i - 1) / numOfBotton), +fHalfHeight, +fHalfDepth * ((j - 1) / numOfSide) + i * difference));
			pUpFace->SetVertex(3, CVertex(-fHalfWidth * ((i - 1) / numOfBotton), +fHalfHeight, +fHalfDepth * (j / numOfSide) + i * difference));
			SetPolygon(index++, pUpFace);

			pUpFace = new CPolygon(4);
			pUpFace->SetVertex(0, CVertex(-fHalfWidth * (i / numOfBotton), +fHalfHeight, 0 * (j / numOfSide) + i * difference));
			pUpFace->SetVertex(1, CVertex(-fHalfWidth * (i / numOfBotton), +fHalfHeight, 0 * ((j - 1) / numOfSide) + i * difference));
			pUpFace->SetVertex(2, CVertex(-fHalfWidth * ((i - 1) / numOfBotton), +fHalfHeight, 0 * ((j - 1) / numOfSide) + i * difference));
			pUpFace->SetVertex(3, CVertex(-fHalfWidth * ((i - 1) / numOfBotton), +fHalfHeight, 0 * (j / numOfSide) + i * difference));
			SetPolygon(index++, pUpFace);
		}
	}

	CPolygon *pBottonFace;
	for (float i = numOfBotton; i>0; --i) {
		for (float j = numOfSide; j>0; --j) {
			pBottonFace = new CPolygon(4);
			pBottonFace->SetVertex(0, CVertex(+fHalfWidth * (i / numOfBotton), -fHalfHeight, +fHalfDepth * (j / numOfSide) + i * difference));
			pBottonFace->SetVertex(1, CVertex(+fHalfWidth * (i / numOfBotton), -fHalfHeight, +fHalfDepth * ((j - 1) / numOfSide) + i * difference));
			pBottonFace->SetVertex(2, CVertex(+fHalfWidth * ((i - 1) / numOfBotton), -fHalfHeight, +fHalfDepth * ((j - 1) / numOfSide) + i * difference));
			pBottonFace->SetVertex(3, CVertex(+fHalfWidth * ((i - 1) / numOfBotton), -fHalfHeight, +fHalfDepth * (j / numOfSide) + i * difference));
			SetPolygon(index++, pBottonFace);

			pBottonFace = new CPolygon(4);
			pBottonFace->SetVertex(0, CVertex(+fHalfWidth * (i / numOfBotton), -fHalfHeight, 0 * (j / numOfSide) + i * difference));
			pBottonFace->SetVertex(1, CVertex(+fHalfWidth * (i / numOfBotton), -fHalfHeight, 0 * ((j - 1) / numOfSide) + i * difference));
			pBottonFace->SetVertex(2, CVertex(+fHalfWidth * ((i - 1) / numOfBotton), -fHalfHeight, 0 * ((j - 1) / numOfSide) + i * difference));
			pBottonFace->SetVertex(3, CVertex(+fHalfWidth * ((i - 1) / numOfBotton), -fHalfHeight, 0 * (j / numOfSide) + i * difference));
			SetPolygon(index++, pBottonFace);

			pBottonFace = new CPolygon(4);
			pBottonFace->SetVertex(0, CVertex(-fHalfWidth * (i / numOfBotton), -fHalfHeight, +fHalfDepth * (j / numOfSide) + i * difference));
			pBottonFace->SetVertex(1, CVertex(-fHalfWidth * (i / numOfBotton), -fHalfHeight, +fHalfDepth * ((j - 1) / numOfSide) + i * difference));
			pBottonFace->SetVertex(2, CVertex(-fHalfWidth * ((i - 1) / numOfBotton), -fHalfHeight, +fHalfDepth * ((j - 1) / numOfSide) + i * difference));
			pBottonFace->SetVertex(3, CVertex(-fHalfWidth * ((i - 1) / numOfBotton), -fHalfHeight, +fHalfDepth * (j / numOfSide) + i * difference));
			SetPolygon(index++, pBottonFace);

			pBottonFace = new CPolygon(4);
			pBottonFace->SetVertex(0, CVertex(-fHalfWidth * (i / numOfBotton), -fHalfHeight, 0 * (j / numOfSide) + i * difference));
			pBottonFace->SetVertex(1, CVertex(-fHalfWidth * (i / numOfBotton), -fHalfHeight, 0 * ((j - 1) / numOfSide) + i * difference));
			pBottonFace->SetVertex(2, CVertex(-fHalfWidth * ((i - 1) / numOfBotton), -fHalfHeight, 0 * ((j - 1) / numOfSide) + i * difference));
			pBottonFace->SetVertex(3, CVertex(-fHalfWidth * ((i - 1) / numOfBotton), -fHalfHeight, 0 * (j / numOfSide) + i * difference));
			SetPolygon(index++, pBottonFace);
		}
	}
}

CWallMesh::~CWallMesh(void)
{
}

CWallTileMesh::CWallTileMesh(float fWidth, float fHeight, float fDepth) :CMesh(1) {
	float fHalfWidth = fWidth * 0.5f;
	float fHalfHeight = fHeight * 0.5f;
	float fHalfDepth = fDepth * 0.5f;

	CPolygon* tile = new CPolygon(4);
	tile->SetVertex(0, CVertex(-fHalfWidth, 0, -fHalfDepth));
	tile->SetVertex(1, CVertex(-fHalfWidth, 0, +fHalfDepth));
	tile->SetVertex(2, CVertex(+fHalfWidth, 0, +fHalfDepth));
	tile->SetVertex(3, CVertex(+fHalfWidth, 0, -fHalfDepth));
	SetPolygon(0, tile);
}

CWallTileMesh::~CWallTileMesh() {

}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CAirplaneMesh::CAirplaneMesh(float fWidth, float fHeight, float fDepth) : CMesh(24)
{
	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fDepth*0.5f;

	float x1 = fx * 0.2f, y1 = fy * 0.2f, x2 = fx * 0.1f, y3 = fy * 0.3f, y2 = ((y1 - (fy - y3)) / x1)*x2 + (fy - y3);
	int i = 0;
	CPolygon *pFace;

	//Upper Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(1, CVertex(+x1, -y1, -fz));
	pFace->SetVertex(2, CVertex(0.0f, 0.0f, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(1, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(2, CVertex(-x1, -y1, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x2, +y2, -fz));
	pFace->SetVertex(1, CVertex(+fx, -y3, -fz));
	pFace->SetVertex(2, CVertex(+x1, -y1, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x2, +y2, -fz));
	pFace->SetVertex(1, CVertex(-x1, -y1, -fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, -fz));
	SetPolygon(i++, pFace);

	//Lower Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, CVertex(0.0f, 0.0f, +fz));
	pFace->SetVertex(2, CVertex(+x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, CVertex(-x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(0.0f, 0.0f, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x2, +y2, +fz));
	pFace->SetVertex(1, CVertex(+x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(+fx, -y3, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x2, +y2, +fz));
	pFace->SetVertex(1, CVertex(-fx, -y3, +fz));
	pFace->SetVertex(2, CVertex(-x1, -y1, +fz));
	SetPolygon(i++, pFace);

	//Right Plane
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(1, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(2, CVertex(+x2, +y2, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x2, +y2, -fz));
	pFace->SetVertex(1, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(2, CVertex(+x2, +y2, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x2, +y2, -fz));
	pFace->SetVertex(1, CVertex(+x2, +y2, +fz));
	pFace->SetVertex(2, CVertex(+fx, -y3, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+fx, -y3, -fz));
	pFace->SetVertex(1, CVertex(+x2, +y2, +fz));
	pFace->SetVertex(2, CVertex(+fx, -y3, +fz));
	SetPolygon(i++, pFace);

	//Back/Right Plane
	pFace->SetVertex(0, CVertex(+x1, -y1, -fz));
	pFace->SetVertex(1, CVertex(+fx, -y3, -fz));
	pFace->SetVertex(2, CVertex(+fx, -y3, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x1, -y1, -fz));
	pFace->SetVertex(1, CVertex(+fx, -y3, +fz));
	pFace->SetVertex(2, CVertex(+x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, CVertex(+x1, -y1, -fz));
	pFace->SetVertex(2, CVertex(+x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace->SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, CVertex(+x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(0.0f, 0.0f, +fz));
	SetPolygon(i++, pFace);

	//Left Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, CVertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(2, CVertex(-x2, +y2, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, CVertex(-x2, +y2, -fz));
	pFace->SetVertex(2, CVertex(-x2, +y2, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x2, +y2, +fz));
	pFace->SetVertex(1, CVertex(-x2, +y2, -fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x2, +y2, +fz));
	pFace->SetVertex(1, CVertex(-fx, -y3, -fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, +fz));
	SetPolygon(i++, pFace);

	//Back/Left Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, CVertex(0.0f, 0.0f, +fz));
	pFace->SetVertex(2, CVertex(-x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, CVertex(-x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(-x1, -y1, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x1, -y1, -fz));
	pFace->SetVertex(1, CVertex(-x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x1, -y1, -fz));
	pFace->SetVertex(1, CVertex(-fx, -y3, +fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, -fz));
	SetPolygon(i++, pFace);
}

CAirplaneMesh::~CAirplaneMesh()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CGameObject::CGameObject() 
{ 
	m_pMesh = NULL; 
	m_xmf4x4World = Matrix4x4::Identity();

	m_dwColor = RGB(0, 0, 0);

	m_xmf3MovingDirection = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_fMovingSpeed = 0.0f;
	m_fMovingRange = 0.0f;

	m_xmf3RotationAxis = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_fRotationSpeed = 0.05f;
}

CGameObject::CGameObject(CMesh *pMesh) : CGameObject()
{
	m_pMesh = pMesh; 
}

CGameObject::~CGameObject(void)
{
	if (m_pMesh) m_pMesh->Release();
}

void CGameObject::SetPosition(float x, float y, float z) 
{
	m_xmf4x4World._41 = x; 
	m_xmf4x4World._42 = y; 
	m_xmf4x4World._43 = z; 
}

void CGameObject::SetPosition(XMFLOAT3& xmf3Position) 
{ 
	m_xmf4x4World._41 = xmf3Position.x; 
	m_xmf4x4World._42 = xmf3Position.y; 
	m_xmf4x4World._43 = xmf3Position.z; 
}

XMFLOAT3 CGameObject::GetPosition() 
{ 
	return(XMFLOAT3(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43)); 
}

XMFLOAT3 CGameObject::GetLook() 
{ 	
	XMFLOAT3 xmf3LookAt(m_xmf4x4World._31, m_xmf4x4World._32, m_xmf4x4World._33);
	xmf3LookAt = Vector3::Normalize(xmf3LookAt);
	return(xmf3LookAt);
}

XMFLOAT3 CGameObject::GetUp() 
{ 	
	XMFLOAT3 xmf3Up(m_xmf4x4World._21, m_xmf4x4World._22, m_xmf4x4World._23);
	xmf3Up = Vector3::Normalize(xmf3Up);
	return(xmf3Up);
}

XMFLOAT3 CGameObject::GetRight()
{ 	
	XMFLOAT3 xmf3Right(m_xmf4x4World._11, m_xmf4x4World._12, m_xmf4x4World._13);
	xmf3Right = Vector3::Normalize(xmf3Right);
	return(xmf3Right);
}

void CGameObject::MoveStrafe(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Right = GetRight();
	xmf3Position = Vector3::Add(xmf3Position, Vector3::ScalarProduct(xmf3Right, fDistance));
	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::MoveUp(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Up = GetUp();
	xmf3Position = Vector3::Add(xmf3Position, Vector3::ScalarProduct(xmf3Up, fDistance));
	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::MoveForward(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3LookAt = GetLook();
	xmf3Position = Vector3::Add(xmf3Position, Vector3::ScalarProduct(xmf3LookAt, fDistance));
	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::Rotate(float fPitch, float fYaw, float fRoll)
{
	XMFLOAT4X4 mtxRotate = Matrix4x4::RotationYawPitchRoll(fYaw, fPitch, fRoll);
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
}

void CGameObject::Rotate(XMFLOAT3& xmf3RotationAxis, float fAngle)
{
	XMFLOAT4X4 mtxRotate = Matrix4x4::RotationAxis(xmf3RotationAxis, fAngle);
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
}

void CGameObject::Move(XMFLOAT3& vDirection, float fSpeed)
{
	SetPosition(m_xmf4x4World._41 + vDirection.x * fSpeed, m_xmf4x4World._42 + vDirection.y * fSpeed, m_xmf4x4World._43 + vDirection.z * fSpeed);
}

void CGameObject::Animate()
{
	if (m_fRotationSpeed != 0.0f) Rotate(m_xmf3RotationAxis, m_fRotationSpeed);
	if (m_fMovingSpeed != 0.0f) Move(m_xmf3MovingDirection, m_fMovingSpeed);
	
	m_xmOOBBTransformed = m_xmOOBB;

	m_xmOOBBTransformed.Transform(m_xmOOBBTransformed, XMLoadFloat4x4(&m_xmf4x4World));
	XMStoreFloat4(&m_xmOOBBTransformed.Orientation, XMQuaternionNormalize(XMLoadFloat4(&m_xmOOBBTransformed.Orientation)));
}

void CGameObject::Render(HDC hDCFrameBuffer, CCamera *pCamera)
{
	XMFLOAT4X4 xm4x4Transform = Matrix4x4::Multiply(m_xmf4x4World, pCamera->m_xmf4x4ViewProject);
	HPEN hPen = ::CreatePen(PS_SOLID, 0, m_dwColor);
	HPEN hOldPen = (HPEN)::SelectObject(hDCFrameBuffer, hPen);
	if (m_pMesh) m_pMesh->Render(hDCFrameBuffer, xm4x4Transform, pCamera);
	::SelectObject(hDCFrameBuffer, hOldPen);
	::DeleteObject(hPen);
}



/////////////////////////////////////////////////////////////////////////////////////////////////////
//

void Paticle::Animate() {
	if (m_fRotationSpeed != 0.0f)
		Rotate(m_xmf3RotationAxis, m_fRotationSpeed);
	if (m_fMovingSpeed != 0.0f)
		Move(m_xmf3MovingDirection, m_fMovingSpeed);
}