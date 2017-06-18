#include "stdafx.h"
#include "GameObject.h"
#include "Shader.h"


CGameObject::CGameObject(int nMeshes)
{
	m_xmf4x4World = Matrix4x4::Identity();
	m_nMeshes = nMeshes;
	m_ppMeshes = NULL;

	if (m_nMeshes > 0)
	{
		m_ppMeshes = new CMesh*[m_nMeshes];
		for (int i = 0; i < m_nMeshes; i++) m_ppMeshes[i] = NULL;
	}
}


CGameObject::~CGameObject()
{
	if (m_ppMeshes)
	{
		for (int i = 0; i < m_nMeshes; i++)
		{
			if (m_ppMeshes[i]) m_ppMeshes[i]->Release();
			m_ppMeshes[i] = NULL;
		}
		delete[] m_ppMeshes;
	}
	//if (m_pShader)
	//{
	//	m_pShader->ReleaseShaderVariables();
	//	m_pShader->Release();
	//}
}

void CGameObject::SetShader(CShader *pShader) { 

}

//void CGameObject::SetObject(float w, float h, float d)
//{
//	width = w * 0.5f;
//	height = h * 0.5f;
//	depth = d * 0.5f;
//
//	SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(width, height, depth), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
//}

void CGameObject::SetMesh(int nIndex, CMesh *pMesh) { 
	if (m_ppMeshes)
	{
		if (m_ppMeshes[nIndex]) 
			m_ppMeshes[nIndex]->Release();

		m_ppMeshes[nIndex] = pMesh;

		if (pMesh) 
			pMesh->AddRef();
	}
}



void CGameObject::ReleaseUploadBuffers() {
	//정점 버퍼를 위한 업로드 버퍼를 소멸시킨다. 
	if (m_ppMeshes)
	{
		for (int i = 0; i < m_nMeshes; i++)
		{
			if (m_ppMeshes[i]) 
				m_ppMeshes[i]->ReleaseUploadBuffers();
		}
	}
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
	if (m_ppMeshes)
	{
		for (int i = 0; i < m_nMeshes; i++)
		{
			if (m_ppMeshes[i])
				m_ppMeshes[i]->Render(pd3dCommandList);
		}
	}
}

void CGameObject::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera,
	UINT nInstances, D3D12_VERTEX_BUFFER_VIEW d3dInstancingBufferView)
{
	OnPrepareRender();
	if (m_ppMeshes)
	{
		for (int i = 0; i < m_nMeshes; i++)
		{
			if (m_ppMeshes[i])
				m_ppMeshes[i]->Render(pd3dCommandList, nInstances, d3dInstancingBufferView);
		}
	}
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

void CGameObject::Scale(float num)
{
	m_xmf4x4World._11 *= num;
	m_xmf4x4World._22 *= num;
	m_xmf4x4World._33 *= num;

}

void CGameObject::setScale(float num)
{
	m_xmf4x4World._11 = num;
	m_xmf4x4World._22 = num;
	m_xmf4x4World._33 = num;
}


void CGameObject::GenerateRayForPicking(XMFLOAT3& xmf3PickPosition, XMFLOAT4X4&
	xmf4x4View, XMFLOAT3 *pxmf3PickRayOrigin, XMFLOAT3 *pxmf3PickRayDirection)
{
	XMFLOAT4X4 xmf4x4WorldView = Matrix4x4::Multiply(m_xmf4x4World, xmf4x4View);
	XMFLOAT4X4 xmf4x4Inverse = Matrix4x4::Inverse(xmf4x4WorldView);
	XMFLOAT3 xmf3CameraOrigin(0.0f, 0.0f, 0.0f);
	//카메라 좌표계의 원점을 모델 좌표계로 변환한다. 
	*pxmf3PickRayOrigin = Vector3::TransformCoord(xmf3CameraOrigin, xmf4x4Inverse);
	//카메라 좌표계의 점(마우스 좌표를 역변환하여 구한 점)을 모델 좌표계로 변환한다. 
	*pxmf3PickRayDirection= Vector3::TransformCoord(xmf3PickPosition, xmf4x4Inverse);
	//광선의 방향 벡터를 구한다. 
	*pxmf3PickRayDirection = Vector3::Normalize(
		Vector3::Subtract(*pxmf3PickRayDirection, *pxmf3PickRayOrigin));
}

int CGameObject::PickObjectByRayIntersection(XMFLOAT3& xmf3PickPosition, XMFLOAT4X4&
	xmf4x4View, float *pfHitDistance)
{
	/*if (die)
		return 0;*/
	int nIntersected = 0;
	if (m_ppMeshes)
	{
		XMFLOAT3 xmf3PickRayOrigin, xmf3PickRayDirection;
		//모델 좌표계의 광선을 생성한다. 
		GenerateRayForPicking(xmf3PickPosition, 
			xmf4x4View, &xmf3PickRayOrigin,&xmf3PickRayDirection);
		//모델 좌표계의 광선과 메쉬의 교차를 검사한다. 
		nIntersected = m_ppMeshes[0]->CheckRayIntersection(xmf3PickRayOrigin,
		xmf3PickRayDirection, pfHitDistance);
	}
	return(nIntersected);
}

bool CGameObject::IsVisible(CCamera *pCamera)
{
	OnPrepareRender();
	bool bIsVisible = false;
	BoundingOrientedBox xmBoundingBox = m_ppMeshes[0]->GetBoundingBox();
	//모델 좌표계의 바운딩 박스를 월드 좌표계로 변환한다. 
	xmBoundingBox.Transform(xmBoundingBox, XMLoadFloat4x4(&m_xmf4x4World));
	if (pCamera) 
		bIsVisible = pCamera->IsInFrustum(xmBoundingBox);
	return(bIsVisible);
}

CRotatingObject::CRotatingObject(int nMeshes) {
	//m_xmf3RotationAxis = XMFLOAT3(1.0f, 5.0f, -5.0f);
	//m_fRotationSpeed = 90.0f;
	
}

CRotatingObject::~CRotatingObject() {
}

void CRotatingObject::StatusWork() {
	distance = 0.0f;
	switch (status)
	{
	case enemyStatus::normal:
		
		break;
	case enemyStatus::move:
		distance = 1.5f;
		break;
	case enemyStatus::attack:
		break;
	case enemyStatus::death:
		return;
	}
}

void CRotatingObject::Animate(float fTimeElapsed) {

	StatusWork();
	CGameObject::Rotate(&m_xmf3RotationAxis, m_fRotationSpeed * fTimeElapsed);
	CGameObject::Move(distance);
	
	CGameObject::Animate(fTimeElapsed);
}

std::default_random_engine dre(1000);
std::uniform_int_distribution<int> xPos(-mapWidth / 2 + 13.0f, mapWidth / 2 - 13.0f);
std::uniform_int_distribution<int> yPos(-mapHeight / 2 + 13.0f, mapHeight / 2 - 13.0f);
std::uniform_int_distribution<int> zPos(0 + 50.0f, mapDepth);
std::uniform_int_distribution<int> type(0, 3);
void CRotatingObject::Reset(XMFLOAT3 pos)
{
	SetPosition(xPos(dre), yPos(dre), pos.z + 800.0f + zPos(dre));
	setType(type(dre));
	SetRotationSpeed(90.0f);
}

void CRotatingObject::setType(int type)
{
	float xRot = 0.0f, yRot = 0.0f, zRot = 0.0f;
	float xDir = 0.0f, yDir = 0.0f, zDir = 0.0f;
	switch (type) {
	case 0:
		xDir = 1.0f;
		xRot = 1.0f;
		color = XMFLOAT4(0.9f, 0.9f, 0.0f, 0.0f);
		break;
	case 1:
		yDir = 1.0f;
		yRot = 1.0f;
		color = XMFLOAT4(0.3f, 0.0f, 0.9f, 0.0f);
		break;
	case 2:
		zDir = -1.0f;
		zRot = 1.0f;
		color = XMFLOAT4(0.0f, 0.9f, 0.0f, 0.0f);
		break;
	case 3:
		xDir = 0.5f; yDir = 0.5f; zDir = -1.0f;
		xRot = 1.0f; yRot = 3.0f; zRot = 1.0f;
		color = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
		break;
	}
	SetRotationAxis(XMFLOAT3(xRot, yRot, zRot));
	setMovingDir(XMFLOAT3(xDir, yDir, zDir));
}


CHeightMapTerrain::CHeightMapTerrain(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList
	*pd3dCommandList, LPCTSTR pFileName, int nWidth, int nLength, 
	int nBlockWidth, int nBlockLength, XMFLOAT3 xmf3Scale, 
	XMFLOAT4 xmf4Color) : CGameObject(0)
{
	m_nWidth = nWidth;
	m_nLength = nLength;
	/*지형 객체는 격자 메쉬들의 배열로 만들 것이다. 
	nBlockWidth, nBlockLength는 격자 메쉬 하나의 가로, 세로 크기이다. 
	cxQuadsPerBlock, czQuadsPerBlock은 격자 메쉬의 가로 방향과 세로 방향 사각형의 개수이다.*/
	int cxQuadsPerBlock = nBlockWidth - 1;
	int czQuadsPerBlock = nBlockLength - 1;
	m_xmf3Scale = xmf3Scale;

	//지형에 사용할 높이 맵을 생성한다. 
	m_pHeightMapImage = new CHeightMapImage(pFileName, nWidth, nLength, xmf3Scale);
	//지형에서 가로 방향, 세로 방향으로 격자 메쉬가 몇 개가 있는 가를 나타낸다. 
	long cxBlocks = (m_nWidth - 1) / cxQuadsPerBlock;
	long czBlocks = (m_nLength - 1) / czQuadsPerBlock;
	//지형 전체를 표현하기 위한 격자 메쉬의 개수이다. 
	m_nMeshes = cxBlocks * czBlocks;
	//지형 전체를 표현하기 위한 격자 메쉬에 대한 포인터 배열을 생성한다.

	m_ppMeshes = new CMesh*[m_nMeshes];
	for (int i = 0; i < m_nMeshes; i++)m_ppMeshes[i] = NULL;
	CHeightMapGridMesh *pHeightMapGridMesh = NULL;
	for (int z = 0, zStart = 0; z < czBlocks; z++)
	{
		for (int x = 0, xStart = 0; x < cxBlocks; x++)
		{
			//지형의 일부분을 나타내는 격자 메쉬의 시작 위치(좌표)이다.
			xStart = x * (nBlockWidth - 1);
			zStart = z * (nBlockLength - 1);
			//지형의 일부분을 나타내는 격자 메쉬를 생성하여 지형 메쉬에 저장한다.
			pHeightMapGridMesh = new CHeightMapGridMesh(pd3dDevice, pd3dCommandList, 
				xStart, zStart, nBlockWidth, nBlockLength, xmf3Scale, xmf4Color, m_pHeightMapImage);
			SetMesh(x + (z*cxBlocks), pHeightMapGridMesh);
		}
	}
	//지형을 렌더링하기 위한 셰이더를 생성한다. 
	//CTerrainShader *pShader = new CTerrainShader();
	//pShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	//SetShader(pShader);
}
CHeightMapTerrain::~CHeightMapTerrain(void)
{
	if (m_pHeightMapImage) delete m_pHeightMapImage;
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
	if (movingSpeed != 0.0f)
		MoveVector(dir, movingSpeed * fTimeElapsed);
}


CBullet::CBullet()
{

}
CBullet::~CBullet()
{

}

void CBullet::Animate(float fTimeElapsed)
{
	Move(5.0f);
	CGameObject::Animate(fTimeElapsed);
	/*runtime += 1.0f;
	if (runtime >= maxtime) {
		runtime = 0;
		setDie(true);
	}*/
}