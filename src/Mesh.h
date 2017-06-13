#pragma once

class CVertex
{
protected:
	XMFLOAT3 m_xmf3Position;
public: 
	CVertex() { 
	m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f); 
	} 
	CVertex(XMFLOAT3 xmf3Position) { 
		m_xmf3Position = xmf3Position; 
	} 
	~CVertex() { }
};

class CDiffusedVertex : public CVertex {
protected:  
	XMFLOAT4 m_xmf4Diffuse;//정점의 색상  
public: 
	CDiffusedVertex() { 
		m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f); 
		m_xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f); 
	} 
	CDiffusedVertex(float x, float y, float z, XMFLOAT4 xmf4Diffuse) { 
		m_xmf3Position = XMFLOAT3(x, y, z); 
		m_xmf4Diffuse = xmf4Diffuse; 
	} 
	CDiffusedVertex(XMFLOAT3 xmf3Position, XMFLOAT4 xmf4Diffuse) { 
		m_xmf3Position = xmf3Position; 
		m_xmf4Diffuse = xmf4Diffuse; 
	}

	~CDiffusedVertex() { }
};

class CMesh
{
private: 
	int m_nReferences = 0;
protected: 
	ID3D12Resource *m_pd3dVertexBuffer = NULL; 
	ID3D12Resource *m_pd3dVertexUploadBuffer = NULL;
	ID3D12Resource *m_pd3dIndexBuffer = NULL;
	ID3D12Resource *m_pd3dIndexUploadBuffer = NULL;

	D3D12_VERTEX_BUFFER_VIEW m_d3dVertexBufferView;
	D3D12_INDEX_BUFFER_VIEW m_d3dIndexBufferView;

	D3D12_PRIMITIVE_TOPOLOGY m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST; 
	UINT m_nSlot = 0; 
	UINT m_nVertices = 0; 
	UINT m_nStride = 0; 
	UINT m_nOffset = 0;

	UINT m_nIndices = 0;
	UINT m_nStartIndex = 0;
	int m_nBaseVertex = 0;

	BoundingOrientedBox m_xmBoundingBox;

	//정점을 픽킹을 위하여 저장한다(정점 버퍼를 Map()하여 읽지 않아도 되도록).
	CDiffusedVertex *m_pVertices = NULL;
	//메쉬의 인덱스를 저장한다(인덱스 버퍼를 Map()하여 읽지 않아도 되도록).
	UINT *m_pnIndices = NULL;
public:
	CMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual ~CMesh();

	void AddRef() { m_nReferences++; } 
	void Release() { if (--m_nReferences <= 0) delete this; }
	void ReleaseUploadBuffers();

	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, UINT nInstances);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, UINT nInstances,
		D3D12_VERTEX_BUFFER_VIEW d3dInstancingBufferView);

public:
	BoundingOrientedBox GetBoundingBox() { return(m_xmBoundingBox); }

	//광선과 메쉬의 교차를 검사하고 교차하는 횟수와 거리를 반환하는 함수이다.
	int CheckRayIntersection(XMFLOAT3& xmRayPosition, 
		XMFLOAT3& xmRayDirection, float* pfNearHitDistance);
};

class CTriangleMesh : public CMesh 
{ 
public:    
	CTriangleMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual ~CTriangleMesh() { } 
};

class CCube : public CMesh
{
private:

public:
	CCube(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, 
		float width, float height, float depth);
	virtual ~CCube() { }
};


class CAirplane : public CMesh 
{
public:
	CAirplane(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, 
		float fWidth = 20.0f, float fHeight = 20.0f, float fDepth = 4.0f,
		XMFLOAT4 xmf4Color = XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f));
	virtual ~CAirplane();
};

//높이맵 표현
class CHeightMapImage
{
private:
	//높이 맵 이미지 픽셀(8-비트)들의 이차원 배열이다. 
	//각 픽셀은 0~255의 값을 갖는다. 
	BYTE *m_pHeightMapPixels;

	int m_nWidth;
	int m_nLength;
	//높이 맵 이미지를 실제로 몇 배 확대하여 사용할 것인가
	XMFLOAT3 m_xmf3Scale;
public:
	CHeightMapImage(LPCTSTR pFileName, int nWidth, int nLength, XMFLOAT3 xmf3Scale);
	~CHeightMapImage(void);
	
	//높이 맵 이미지에서 (x, z) 위치의 법선 벡터를 반환한다.
	XMFLOAT3 GetHeightMapNormal(int x, int z);

	float GetHeight(float x, float z);
	XMFLOAT3 GetScale() { return(m_xmf3Scale); }
	BYTE *GetHeightMapPixels() { return(m_pHeightMapPixels); }
	int GetHeightMapWidth() { return(m_nWidth); }
	int GetHeightMapLength() { return(m_nLength); }
};

//지형매쉬 표현
class CHeightMapGridMesh : public CMesh
{
protected:

	int m_nWidth;
	int m_nLength;

	XMFLOAT3 m_xmf3Scale;
public:
	CHeightMapGridMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList
		*pd3dCommandList, int xStart, int zStart, int nWidth, int nLength, XMFLOAT3 xmf3Scale =
		XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4 xmf4Color = XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f), void
		*pContext = NULL);
	virtual ~CHeightMapGridMesh();
	XMFLOAT3 GetScale() { return(m_xmf3Scale); }
	int GetWidth() { return(m_nWidth); }
	int GetLength() { return(m_nLength); }

	virtual float OnGetHeight(int x, int z, void *pContext);
	virtual XMFLOAT4 OnGetColor(int x, int z, void *pContext);
};

class CSphereMeshDiffused : public CMesh
{
public:
	CSphereMeshDiffused(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList
		*pd3dCommandList, float fRadius = 2.0f, int nSlices = 20, int nStacks = 20);
	virtual ~CSphereMeshDiffused();
};