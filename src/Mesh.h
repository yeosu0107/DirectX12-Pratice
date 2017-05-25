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

public:
	CMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual ~CMesh();

	void AddRef() { m_nReferences++; } 
	void Release() { if (--m_nReferences <= 0) delete this; }
	void ReleaseUploadBuffers();

	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList);


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
	XMFLOAT3 m_vNormal[6] = {
		{ 1.0f, 0.0f, 0.0f }, //오른쪽
		{ -1.0f, 0.0f, 0.0f }, //왼쪽
		{ 0.0f, 1.0f, 0.0f }, //위
		{ 0.0f, -1.0f, 0.0f }, //아래
		{ 0.0f, 0.0f, -1.0f }, //앞
		{ 0.0f, 0.0f, 1.0f }, //뒤
	};
	float m_fScalar[6];
public:
	CCube(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, 
		float width, float height, float depth);
	virtual ~CCube() { }
};