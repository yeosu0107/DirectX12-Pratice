#include "stdafx.h"
#include "GameObject.h"
#include "Shader.h"
#include <random>

CGameObject::CGameObject()
{
	XMStoreFloat4x4(&m_xmf4x4World, XMMatrixIdentity());
}


CGameObject::~CGameObject()
{
	if (m_pMesh) m_pMesh->Release(); 
}

void CGameObject::SetShader(CShader *pShader) { 
	//if (m_pShader) 
	//	m_pShader->Release(); 

	//m_pShader = pShader; 

	//if (m_pShader) 
	//	m_pShader->AddRef(); 
}

void CGameObject::SetObject(float w, float h, float d)
{
	width = w * 0.5f;
	height = h * 0.5f;
	depth = d * 0.5f;

	SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(width, height, depth), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
}

void CGameObject::SetMesh(CMesh *pMesh) { 
	if (m_pMesh) 
		m_pMesh->Release();

	m_pMesh = pMesh; 

	if (m_pMesh) 
		m_pMesh->AddRef(); 
}



void CGameObject::ReleaseUploadBuffers() {
	//정점 버퍼를 위한 업로드 버퍼를 소멸시킨다. 
	if (m_pMesh) 
		m_pMesh->ReleaseUploadBuffers(); 
}

void CGameObject::Animate(float fTimeElapsed) { 
	//충돌영역
	m_xmOOBBTransformed = m_xmOOBB;
	m_xmOOBBTransformed.Transform(m_xmOOBBTransformed, XMLoadFloat4x4(&m_xmf4x4World));
	XMStoreFloat4(&m_xmOOBBTransformed.Orientation, XMQuaternionNormalize(XMLoadFloat4(&m_xmOOBBTransformed.Orientation)));
}

void CGameObject::OnPrepareRender() { 
}

void CGameObject::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera) {
	OnPrepareRender();
	
	UpdateShaderVariables(pd3dCommandList);
	//게임 객체에 메쉬가 연결되어 있으면 메쉬를 렌더링한다. 
	if (m_pMesh) {
		m_pMesh->Render(pd3dCommandList);
	}
}

void CGameObject::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera,
	UINT nInstances, D3D12_VERTEX_BUFFER_VIEW d3dInstancingBufferView)
{
	OnPrepareRender();
	if (m_pMesh) m_pMesh->Render(pd3dCommandList, nInstances, d3dInstancingBufferView);
}

void CGameObject::CreateShaderVariables(ID3D12Device *pd3dDevice,
	ID3D12GraphicsCommandList *pd3dCommandList)
{
}
void CGameObject::ReleaseShaderVariables()
{
}

void CGameObject::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4World)));
	//객체의 월드 변환 행렬을 루트 상수(32-비트 값)를 통하여 셰이더 변수(상수 버퍼)로 복사한다. 
	pd3dCommandList->SetGraphicsRoot32BitConstants(0, 16, &xmf4x4World, 0);
}

void CGameObject::SetPosition(float x, float y, float z)
{
	m_xmf4x4World._41 = x;
	m_xmf4x4World._42 = y;
	m_xmf4x4World._43 = z;
}
void CGameObject::SetPosition(XMFLOAT3 xmf3Position)
{
	SetPosition(xmf3Position.x, xmf3Position.y, xmf3Position.z);
}
XMFLOAT3 CGameObject::GetPosition()
{
	return(XMFLOAT3(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43));
}


XMFLOAT3 CGameObject::GetLook()
{
	return(Vector3::Normalize(XMFLOAT3(m_xmf4x4World._31, m_xmf4x4World._32,
		m_xmf4x4World._33)));
}

XMFLOAT3 CGameObject::GetUp()
{
	return(Vector3::Normalize(XMFLOAT3(m_xmf4x4World._21, m_xmf4x4World._22,
		m_xmf4x4World._23)));
}

XMFLOAT3 CGameObject::GetRight()
{
	return(Vector3::Normalize(XMFLOAT3(m_xmf4x4World._11, m_xmf4x4World._12,
		m_xmf4x4World._13)));
}

//x축이동
void CGameObject::MoveStrafe(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Right = GetRight();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Right, fDistance);
	CGameObject::SetPosition(xmf3Position);
}

//y축이동
void CGameObject::MoveUp(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Up = GetUp();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Up, fDistance);
	CGameObject::SetPosition(xmf3Position);
}

//z축이동
void CGameObject::MoveForward(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Look = GetLook();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Look, fDistance);
	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::MoveVector(XMFLOAT3 dir, float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	xmf3Position = Vector3::Add(xmf3Position, dir, fDistance);
	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::Move(float fDist) {
	XMFLOAT3 pos = GetPosition();
	pos = Vector3::Add(pos, movingDir, fDist);
	CGameObject::SetPosition(pos);
}

void CGameObject::Rotate(XMFLOAT3 * pxmf3Axis, float fAngle)
{
	XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(pxmf3Axis),
		XMConvertToRadians(fAngle));
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
}

void CGameObject::Rotate(float fPitch, float fYaw, float fRoll)
{
	XMMATRIX mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fPitch),
		XMConvertToRadians(fYaw), XMConvertToRadians(fRoll));
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
}

CRotatingObject::CRotatingObject() {
	//m_xmf3RotationAxis = XMFLOAT3(1.0f, 5.0f, -5.0f);
	//m_fRotationSpeed = 90.0f;
}

CRotatingObject::~CRotatingObject() {
}



void CRotatingObject::Animate(float fTimeElapsed) {
	CGameObject::Rotate(&m_xmf3RotationAxis, m_fRotationSpeed * fTimeElapsed);
	CGameObject::Move(1.0f);
	CGameObject::Animate(fTimeElapsed);
}

CWallObject::CWallObject()
{
}

CWallObject::~CWallObject()
{
}



void CWallObject::Animate(float fTimeElapsed)
{
	CGameObject::Animate(fTimeElapsed);
}

void Paticle::setPaticle(XMFLOAT3 tdir, XMFLOAT3 taxis, float mov, float rot)
{
	dir = tdir;
	axis = taxis;
	movingSpeed = mov;
	rotSpeed = rot;
}

void Paticle::Animate(float fTimeElapsed)
{
	/*if (rotSpeed != 0.0f)
		Rotate(&axis, rotSpeed * fTimeElapsed);*/
	if (movingSpeed != 0.0f)
		MoveVector(dir, movingSpeed * fTimeElapsed);
}

CPaticles::CPaticles(int num, ID3D12Device *pd3dDevice, 
	ID3D12GraphicsCommandList *pd3dCommandList)
{
	numOfPaticle = num;
	paticle = new Paticle[num];

	std::default_random_engine dre;
	std::uniform_int_distribution<> dir(-10, 10);
	std::uniform_int_distribution<> rot(0, 1);

	CCube* paticleMesh = new CCube(pd3dDevice, pd3dCommandList, 2.5f, 2.5f, 2.5f);

	for (int i = 0; i < numOfPaticle; ++i) {
		paticle[i].setPaticle(XMFLOAT3(dir(dre), dir(dre), dir(dre)),
			XMFLOAT3(rot(dre), rot(dre), rot(dre)), 5.0f, 0.5f);
		paticle[i].SetMesh(paticleMesh);
		//paticle[i].SetPosition(pos);
	}
}

CPaticles::~CPaticles()
{
	delete[] paticle;
}

void CPaticles::Render(ID3D12GraphicsCommandList * pd3dCommandList, CCamera * pCamera)
{
	if (!run)
		return;
	for (int i = 0; i < numOfPaticle; ++i)
		paticle[i].Render(pd3dCommandList, pCamera);
}

void CPaticles::Animate(float fTimeElapsed)
{
	if (!run)
		return;
	if (runtime > maxtime) {
		runtime = 0.0f;
		run = false;
	}
	for (int i = 0; i < numOfPaticle; ++i)
		paticle[i].Animate(fTimeElapsed);
	//printf("%f\n", runtime);
	runtime += fTimeElapsed * 50.0f;
}

void CPaticles::setPositions(XMFLOAT3 pos)
{
	for (int i = 0; i < numOfPaticle; ++i)
		paticle[i].SetPosition(pos);
}

void CPaticles::setRun() {
	if (!run) {
		runtime = 0.0f;
		run = true;
	}
}