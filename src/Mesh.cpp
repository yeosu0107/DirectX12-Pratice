#include "stdafx.h"
#include "Mesh.h"


CMesh::CMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
}


CMesh::~CMesh()
{
	if (m_pd3dVertexBuffer) 
		m_pd3dVertexBuffer->Release(); 
	if (m_pd3dVertexUploadBuffer) 
		m_pd3dVertexUploadBuffer->Release();
}

void CMesh::ReleaseUploadBuffers() { 
	//���� ���۸� ���� ���ε� ���۸� �Ҹ� 
	if (m_pd3dVertexUploadBuffer) 
		m_pd3dVertexUploadBuffer->Release(); 
	if (m_pd3dIndexUploadBuffer)
		m_pd3dIndexUploadBuffer->Release();
};

void CMesh::Render(ID3D12GraphicsCommandList *pd3dCommandList) { 
	//�޽��� ������Ƽ�� ������ ���� 
	pd3dCommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology); 
	//�޽��� ���� ���� �並 ���� 
	pd3dCommandList->IASetVertexBuffers(m_nSlot, 1, &m_d3dVertexBufferView); 
	if (m_pd3dIndexBuffer) {
		pd3dCommandList->IASetIndexBuffer(&m_d3dIndexBufferView);
		pd3dCommandList->DrawIndexedInstanced(m_nIndices, 1, 0, 0, 0);
	}
	else {
		pd3dCommandList->DrawInstanced(m_nVertices, 1, m_nOffset, 0);
	}
	//�޽��� ���� ���� �並 �������Ѵ�(����������(�Է� ������)�� �۵��ϰ� �Ѵ�). 
}

CTriangleMesh::CTriangleMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList) : CMesh(pd3dDevice, pd3dCommandList) {
	//�ﰢ�� �޽��� �����Ѵ�. 
	m_nVertices = 3; 
	m_nStride = sizeof(CDiffusedVertex); 
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	CDiffusedVertex pVertices[3]; 
	pVertices[0] = CDiffusedVertex(XMFLOAT3(0.0f, 0.1f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
	pVertices[1] = CDiffusedVertex(XMFLOAT3(0.1f, -0.1f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)); 
	pVertices[2] = CDiffusedVertex(XMFLOAT3(-0.1f, -0.1f, 0.0f), XMFLOAT4(Colors::Blue));
	
	//�ﰢ�� �޽��� ���ҽ�(���� ����)�� �����Ѵ�. 
	m_pd3dVertexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, 
		pVertices, m_nStride * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, 
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dVertexUploadBuffer);

	//���� ���� �並 �����Ѵ�. 
	m_d3dVertexBufferView.BufferLocation = m_pd3dVertexBuffer->GetGPUVirtualAddress();
	m_d3dVertexBufferView.StrideInBytes = m_nStride; 
	m_d3dVertexBufferView.SizeInBytes = m_nStride * m_nVertices; 
}


CCube::CCube(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, 
	float width, float height, float depth) : CMesh(pd3dDevice, pd3dCommandList)
{

	//ť�� �Ž� ����
	m_nVertices = 8;
	m_nStride = sizeof(CDiffusedVertex);
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	

	float fx = width*0.5f, fy = height*0.5f, fz = depth*0.5f;
	m_fScalar[0] = fx;
	m_fScalar[1] = fx;
	m_fScalar[2] = fy;
	m_fScalar[3] = fy;
	m_fScalar[4] = fz;
	m_fScalar[5] = fz;

	CDiffusedVertex pVertices[8];
	pVertices[0] = CDiffusedVertex(XMFLOAT3(-fx, +fy, -fz), RANDOM_COLOR);
	pVertices[1] = CDiffusedVertex(XMFLOAT3(+fx, +fy, -fz), RANDOM_COLOR);
	pVertices[2] = CDiffusedVertex(XMFLOAT3(+fx, +fy, +fz), RANDOM_COLOR);
	pVertices[3] = CDiffusedVertex(XMFLOAT3(-fx, +fy, +fz), RANDOM_COLOR);
	pVertices[4] = CDiffusedVertex(XMFLOAT3(-fx, -fy, -fz), RANDOM_COLOR);
	pVertices[5] = CDiffusedVertex(XMFLOAT3(+fx, -fy, -fz), RANDOM_COLOR);
	pVertices[6] = CDiffusedVertex(XMFLOAT3(+fx, -fy, +fz), RANDOM_COLOR);
	pVertices[7] = CDiffusedVertex(XMFLOAT3(-fx, -fy, +fz), RANDOM_COLOR);

	//�޽��� ���ҽ�(���� ����)�� �����Ѵ�. 

	m_pd3dVertexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, 
		pVertices, m_nStride * m_nVertices, D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dVertexUploadBuffer);
	//���� ���� �並 �����Ѵ�. 
	m_d3dVertexBufferView.BufferLocation = m_pd3dVertexBuffer->GetGPUVirtualAddress();
	m_d3dVertexBufferView.StrideInBytes = m_nStride;
	m_d3dVertexBufferView.SizeInBytes = m_nStride * m_nVertices;

	UINT index[18];
	index[0] = 5; index[1] = 6; index[2] = 4;
	index[3] = 7; index[4] = 0; index[5] = 3;
	index[6] = 1; index[7] = 2; index[8] = 2;
	index[9] = 3; index[10] = 3; index[11] = 7;

	index[12] = 2; index[13] = 6; index[14] = 1;
	index[15] = 5; index[16] = 0; index[17] = 4;

	m_nIndices = 18;
	////�޽��� ���ҽ�(���� ����)�� �����Ѵ�. 
	m_pd3dIndexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, index, sizeof(UINT)*m_nIndices, D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_INDEX_BUFFER, &m_pd3dIndexUploadBuffer);

	m_d3dIndexBufferView.BufferLocation = m_pd3dIndexBuffer->GetGPUVirtualAddress();
	m_d3dIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	m_d3dIndexBufferView.SizeInBytes = sizeof(UINT)*m_nIndices;
	

	
}