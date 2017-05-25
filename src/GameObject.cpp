#include "stdafx.h"
#include "GameObject.h"
#include "Shader.h"

CGameObject::CGameObject()
{
	XMStoreFloat4x4(&m_xmf4x4World, XMMatrixIdentity());
}


CGameObject::~CGameObject()
{
	if (m_pMesh) m_pMesh->Release(); 
	if (m_pShader) { 
		m_pShader->ReleaseShaderVariables(); 
		m_pShader->Release(); 
	}
}

void CGameObject::SetShader(CShader *pShader) { 
	if (m_pShader) 
		m_pShader->Release(); 

	m_pShader = pShader; 

	if (m_pShader) 
		m_pShader->AddRef(); 
}

void CGameObject::SetMesh(CMesh *pMesh) { 
	if (m_pMesh) 
		m_pMesh->Release();

	m_pMesh = pMesh; 

	if (m_pMesh) 
		m_pMesh->AddRef(); 
}

void CGameObject::Rotate(XMFLOAT3 * pxmf3Axis, float fAngle)
{
	XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(pxmf3Axis), 
		XMConvertToRadians(fAngle));
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
}

void CGameObject::ReleaseUploadBuffers() {
	//정점 버퍼를 위한 업로드 버퍼를 소멸시킨다. 
	if (m_pMesh) 
		m_pMesh->ReleaseUploadBuffers(); 
}

void CGameObject::Animate(float fTimeElapsed) { 
}

void CGameObject::OnPrepareRender() { 
}

void CGameObject::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera) {
	OnPrepareRender();
	
	//m_pShader->UpdateShaderVariable(pd3dCommandList, &m_xmf4x4World);
	//게임 객체에 메쉬가 연결되어 있으면 메쉬를 렌더링한다. 
	if (m_pMesh) {
		m_pMesh->Render(pd3dCommandList);
	}
}


CRotatingObject::CRotatingObject() { 
	m_xmf3RotationAxis = XMFLOAT3(1.0f, 5.0f, -5.0f); 
	m_fRotationSpeed = 90.0f; 
}

CRotatingObject::~CRotatingObject() { 
}

void CRotatingObject::Animate(float fTimeElapsed) { 
	CGameObject::Rotate(&m_xmf3RotationAxis, m_fRotationSpeed * fTimeElapsed); 
}
