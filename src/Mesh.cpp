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
	//정점 버퍼를 위한 업로드 버퍼를 소멸 
	if (m_pd3dVertexUploadBuffer) 
		m_pd3dVertexUploadBuffer->Release(); 
	if (m_pd3dIndexUploadBuffer)
		m_pd3dIndexUploadBuffer->Release();
	m_pd3dVertexUploadBuffer = NULL;
	m_pd3dIndexUploadBuffer = NULL;
};

void CMesh::Render(ID3D12GraphicsCommandList *pd3dCommandList) { 
	//메쉬의 프리미티브 유형을 설정 
	pd3dCommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology); 
	//메쉬의 정점 버퍼 뷰를 설정 
	pd3dCommandList->IASetVertexBuffers(m_nSlot, 1, &m_d3dVertexBufferView); 
	if (m_pd3dIndexBuffer) {
		pd3dCommandList->IASetIndexBuffer(&m_d3dIndexBufferView);
		pd3dCommandList->DrawIndexedInstanced(m_nIndices, 1, 0, 0, 0);
	}
	else {
		pd3dCommandList->DrawInstanced(m_nVertices, 1, m_nOffset, 0);
	}
	//메쉬의 정점 버퍼 뷰를 렌더링한다(파이프라인(입력 조립기)을 작동하게 한다). 
}

CTriangleMesh::CTriangleMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList) : CMesh(pd3dDevice, pd3dCommandList) {
	//삼각형 메쉬를 정의한다. 
	m_nVertices = 3; 
	m_nStride = sizeof(CDiffusedVertex); 
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	CDiffusedVertex pVertices[3]; 
	pVertices[0] = CDiffusedVertex(XMFLOAT3(0.0f, 0.1f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
	pVertices[1] = CDiffusedVertex(XMFLOAT3(0.1f, -0.1f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)); 
	pVertices[2] = CDiffusedVertex(XMFLOAT3(-0.1f, -0.1f, 0.0f), XMFLOAT4(Colors::Blue));
	
	//삼각형 메쉬를 리소스(정점 버퍼)로 생성한다. 
	m_pd3dVertexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, 
		pVertices, m_nStride * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, 
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dVertexUploadBuffer);

	//정점 버퍼 뷰를 생성한다. 
	m_d3dVertexBufferView.BufferLocation = m_pd3dVertexBuffer->GetGPUVirtualAddress();
	m_d3dVertexBufferView.StrideInBytes = m_nStride; 
	m_d3dVertexBufferView.SizeInBytes = m_nStride * m_nVertices; 
}


CCube::CCube(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, 
	float width, float height, float depth) : CMesh(pd3dDevice, pd3dCommandList)
{

	//큐브 매쉬 선언
	m_nVertices = 8;
	m_nStride = sizeof(CDiffusedVertex);
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	

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

	//메쉬를 리소스(정점 버퍼)로 생성한다. 

	m_pd3dVertexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, 
		pVertices, m_nStride * m_nVertices, D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dVertexUploadBuffer);
	//정점 버퍼 뷰를 생성한다. 
	m_d3dVertexBufferView.BufferLocation = m_pd3dVertexBuffer->GetGPUVirtualAddress();
	m_d3dVertexBufferView.StrideInBytes = m_nStride;
	m_d3dVertexBufferView.SizeInBytes = m_nStride * m_nVertices;

	m_nIndices = 36;
	UINT pnIndices[36];
	//ⓐ 앞면(Front) 사각형의 위쪽 삼각형
	pnIndices[0] = 3; pnIndices[1] = 1; pnIndices[2] = 0;
	//ⓑ 앞면(Front) 사각형의 아래쪽 삼각형
	pnIndices[3] = 2; pnIndices[4] = 1; pnIndices[5] = 3;
	//ⓒ 윗면(Top) 사각형의 위쪽 삼각형
	pnIndices[6] = 0; pnIndices[7] = 5; pnIndices[8] = 4;
	//ⓓ 윗면(Top) 사각형의 아래쪽 삼각형
	pnIndices[9] = 1; pnIndices[10] = 5; pnIndices[11] = 0;
	//ⓔ 뒷면(Back) 사각형의 위쪽 삼각형
	pnIndices[12] = 3; pnIndices[13] = 4; pnIndices[14] = 7;
	//ⓕ 뒷면(Back) 사각형의 아래쪽 삼각형
	pnIndices[15] = 0; pnIndices[16] = 4; pnIndices[17] = 3;
	//ⓖ 아래면(Bottom) 사각형의 위쪽 삼각형
	pnIndices[18] = 1; pnIndices[19] = 6; pnIndices[20] = 5;
	//ⓗ 아래면(Bottom) 사각형의 아래쪽 삼각형
	pnIndices[21] = 2; pnIndices[22] = 6; pnIndices[23] = 1;
	//ⓘ 옆면(Left) 사각형의 위쪽 삼각형
	pnIndices[24] = 2; pnIndices[25] = 7; pnIndices[26] = 6;
	//ⓙ 옆면(Left) 사각형의 아래쪽 삼각형
	pnIndices[27] = 3; pnIndices[28] = 7; pnIndices[29] = 2;
	//ⓚ 옆면(Right) 사각형의 위쪽 삼각형
	pnIndices[30] = 6; pnIndices[31] = 4; pnIndices[32] = 5;
	//ⓛ 옆면(Right) 사각형의 아래쪽 삼각형
	pnIndices[33] = 7; pnIndices[34] = 4; pnIndices[35] = 6;
	////메쉬를 리소스(정점 버퍼)로 생성한다. 
	m_pd3dIndexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, pnIndices, sizeof(UINT)*m_nIndices, D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_INDEX_BUFFER, &m_pd3dIndexUploadBuffer);

	m_d3dIndexBufferView.BufferLocation = m_pd3dIndexBuffer->GetGPUVirtualAddress();
	m_d3dIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	m_d3dIndexBufferView.SizeInBytes = sizeof(UINT)*m_nIndices;
	

	
}