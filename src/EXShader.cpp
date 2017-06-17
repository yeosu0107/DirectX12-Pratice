#include "stdafx.h"
#include "EXShader.h"


CPaticlesShader::CPaticlesShader()
{
}

CPaticlesShader::~CPaticlesShader()
{
}
void CPaticlesShader::CreateShader(ID3D12Device * pd3dDevice, ID3D12RootSignature * pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];
	CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
}

void CPaticlesShader::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList)
{
	for (int j = 0; j < m_nObjects; j++)
	{
		m_pcbMappedGameObjects[j].m_xmcColor = m_ppObjects[j]->getColor();
		XMStoreFloat4x4(&m_pcbMappedGameObjects[j].m_xmf4x4Transform,
			XMMatrixTranspose(XMLoadFloat4x4(&m_ppObjects[j]->getMatrix())));
	}
}

void CPaticlesShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	m_nObjects = numOfPaticles;
	m_ppObjects = new CGameObject*[m_nObjects];

	std::default_random_engine dre;
	std::uniform_int_distribution<int> dir(-10, 10);
	std::uniform_int_distribution<int> rot(0, 1);

	Paticle* paticle = NULL;
	for (int i = 0; i < m_nObjects; ++i) {
		paticle = new Paticle();
		paticle->setPaticle(XMFLOAT3(dir(dre), dir(dre), dir(dre)),
			XMFLOAT3(rot(dre), rot(dre), rot(dre)), 5.0f, 0.5f);
		m_ppObjects[i] = paticle;
	}

	CCube* paticleMesh = new CCube(pd3dDevice, pd3dCommandList, 2.5f, 2.5f, 2.5f);
	m_ppObjects[0]->SetMesh(0, paticleMesh);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CPaticlesShader::Render(ID3D12GraphicsCommandList * pd3dCommandList, CCamera * pCamera)
{
	if (!run)
		return;

	ObjectShader::Render(pd3dCommandList, pCamera);

	//모든 게임 객체의 인스턴싱 데이터를 버퍼에 저장한다.
	UpdateShaderVariables(pd3dCommandList);

	//하나의 정점 데이터를 사용하여 모든 게임 객체(인스턴스)들을 렌더링한다.
	m_ppObjects[0]->Render(pd3dCommandList, pCamera, m_nObjects,
		m_d3dInstancingBufferView);
}

void CPaticlesShader::AnimateObjects(float fTimeElapsed)
{
	if (!run)
		return;

	if (scale < 0) {
		run = false;
		scale = 1.0f;

	}

	for (int j = 0; j < m_nObjects; j++) {
		m_ppObjects[j]->Scale(0.95f);
		m_ppObjects[j]->Animate(fTimeElapsed);
	}
	scale *= 0.95f;
}
void CPaticlesShader::setPosition(XMFLOAT3 pos)
{
	for (int i = 0; i < m_nObjects; ++i)
		m_ppObjects[i]->SetPosition(pos);
}

void CPaticlesShader::setRun(XMFLOAT4& color) { 
	run = true; 
	for (int i = 0; i < m_nObjects; ++i) {
		m_ppObjects[i]->setScale(1.0f);
		m_ppObjects[i]->setColor(color);
	}
}

CBulletShader::CBulletShader() {}
CBulletShader::~CBulletShader() {}

void CBulletShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature
	*pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];
	CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
}

void CBulletShader::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
	for (int j = 0; j < m_nObjects; j++)
	{
		if (!m_ppObjects[j]->getDie()) {
			m_pcbMappedGameObjects[j].m_xmcColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
			XMStoreFloat4x4(&m_pcbMappedGameObjects[j].m_xmf4x4Transform,
				XMMatrixTranspose(XMLoadFloat4x4(&m_ppObjects[j]->getMatrix())));
		}
	}
}

void CBulletShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	m_nObjects = maxBullet;
	m_ppObjects = new CGameObject*[m_nObjects];
	CCube* bulletMesh = new CCube(pd3dDevice, pd3dCommandList, 2.5f, 2.5f, 2.5f);

	CBullet* bullet = NULL;
	for (int i = 0; i < m_nObjects; ++i) {
		bullet = new CBullet();
		bullet->SetPosition(0.0f, 0.0f, 0.0f);
		//bullet->SetObject(2.5f, 2.5f, 2.5f);
		bullet->SetOOBB(bulletMesh->GetBoundingBox());
		bullet->setDie(true);
		m_ppObjects[i] = bullet;
	}

	
	m_ppObjects[0]->SetMesh(0, bulletMesh);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CBulletShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	CShader::Render(pd3dCommandList, pCamera);

	//모든 게임 객체의 인스턴싱 데이터를 버퍼에 저장한다.
	UpdateShaderVariables(pd3dCommandList);

	//하나의 정점 데이터를 사용하여 모든 게임 객체(인스턴스)들을 렌더링한다.
	m_ppObjects[0]->Render(pd3dCommandList, pCamera, m_nObjects,
		m_d3dInstancingBufferView);
}

void CBulletShader::AnimateObjects(float fTime)
{
	for (int j = 0; j < m_nObjects; j++) {
		if (!m_ppObjects[j]->getDie()) {
			m_ppObjects[j]->Animate(fTime);
		}
	}
}


void CBulletShader::Shoot(XMFLOAT3 pos, XMFLOAT3 look)
{
	if (nowtime < maxDelay) {
		nowtime += 1;
		return;
	}
	m_ppObjects[nowIndex]->SetPosition(pos);
	m_ppObjects[nowIndex]->setMovingDir(look);
	m_ppObjects[nowIndex]->setDie(false);
	nowIndex += 1;
	if (nowIndex >= m_nObjects)
		nowIndex = 0;
	nowtime = 0;
}

CMazeShader::CMazeShader()
{

}

CMazeShader::~CMazeShader()
{

}

void CMazeShader::CreateShader(ID3D12Device * pd3dDevice, ID3D12RootSignature * pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];
	CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
}

void CMazeShader::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList)
{
	for (int j = 0; j < m_nObjects; j++)
	{
		m_pcbMappedGameObjects[j].m_xmcColor = XMFLOAT4(1.0f, 0.0f, 1.0f, 0.0f);
		XMStoreFloat4x4(&m_pcbMappedGameObjects[j].m_xmf4x4Transform,
			XMMatrixTranspose(XMLoadFloat4x4(&m_ppObjects[j]->getMatrix())));
	}
}



void CMazeShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, void* pContext)
{
	int count = 0;

	for (int i = 0; i < 10; ++i)
		for (int j = 0; j < 10; ++j)
			for (int z = 0; z < 2; ++z)
				if (miro[i][j].layout[z] != 0)
					++count;

	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)pContext;

	float fHeight = 0.0f;

	float startXpos = 73.0f;
	float startZPos = 145.0f;

	float xPos = 0.0f;
	float zPos = 0.0f;
	
	float setX = 60.0f;
	float setY = 50.0f;
	float setZ = 30.0f;

	m_nObjects = count; //미로
	m_ppObjects = new CGameObject*[m_nObjects];
	int now = 0;
	CGameObject* wall = nullptr;
	CCube* pCube = new CCube(pd3dDevice, pd3dCommandList, setX, setY, setZ);
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			for (int z = 0; z < 2; ++z) {
				if (miro[i][j].layout[z] == 0)
					continue;
				
				wall = new CGameObject();
				if (miro[i][j].layout[z] == 1) {
					xPos = startXpos + setX * j + setZ - setZ/2;
					zPos = startZPos + 950 - setX * i;
					fHeight = pTerrain->GetHeight(xPos, zPos);
					wall->SetPosition(xPos, fHeight, zPos);
					
					wall->Rotate(0, 90, 0);
				}
				else if (miro[i][j].layout[z] == 2) {
					xPos = startXpos + setX * j + setZ;
					zPos = startZPos + 950 - setX * i + setZ/2;
					fHeight = pTerrain->GetHeight(xPos, zPos);
					wall->SetPosition(xPos, fHeight, zPos);
					
				}
				else if (miro[i][j].layout[z] == 3) {
					xPos = startXpos + setX * j + setZ + setZ/2;
					zPos = startZPos + 950 - setX * i;
					fHeight = pTerrain->GetHeight(xPos, zPos);
					wall->SetPosition(xPos, fHeight, zPos);
					
					wall->Rotate(0, 90, 0);
				}
				else if (miro[i][j].layout[z] == 4) {
					xPos = startXpos + setX * j + setZ;
					zPos = startZPos + 950 - setX * i - setZ/2;
					fHeight = pTerrain->GetHeight(xPos, zPos);
					wall->SetPosition(xPos, fHeight, zPos);
					
				}
				wall->SetOOBB(pCube->GetBoundingBox());
				m_ppObjects[now++] = wall;
				
			}
		}
	}
				
	
	m_ppObjects[0]->SetMesh(0, pCube);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CMazeShader::Render(ID3D12GraphicsCommandList * pd3dCommandList, CCamera * pCamera)
{
	ObjectShader::Render(pd3dCommandList, pCamera);

	//모든 게임 객체의 인스턴싱 데이터를 버퍼에 저장한다.
	UpdateShaderVariables(pd3dCommandList);

	//하나의 정점 데이터를 사용하여 모든 게임 객체(인스턴스)들을 렌더링한다.
	m_ppObjects[0]->Render(pd3dCommandList, pCamera, m_nObjects,
		m_d3dInstancingBufferView);
}

void CMazeShader::AnimateObjects(float fTimeElapsed)
{
	for (int j = 0; j < m_nObjects; j++) {
		m_ppObjects[j]->Animate(fTimeElapsed);
	}
}


CEnemyShader::CEnemyShader() {}
CEnemyShader::~CEnemyShader() {}

void CEnemyShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];
	CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
}
void CEnemyShader::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
	for (int j = 0; j < m_nObjects; j++)
	{
		m_pcbMappedGameObjects[j].m_xmcColor = m_ppObjects[j]->getColor();
		XMStoreFloat4x4(&m_pcbMappedGameObjects[j].m_xmf4x4Transform,
			XMMatrixTranspose(XMLoadFloat4x4(&m_ppObjects[j]->getMatrix())));
	}
}

void CEnemyShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void* pContext)
{
	m_nObjects = 15;
	m_ppObjects = new CGameObject*[m_nObjects];

	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)pContext;
	//fHeight = pTerrain->GetHeight(xPos, zPos);
	float fHeight = 0.0f;
	CSphereMeshDiffused *pSphereMesh = new CSphereMeshDiffused(pd3dDevice,
		pd3dCommandList, 6.0f, 20, 20);
	std::default_random_engine dre(102);
	std::uniform_int_distribution<int> type(0, 3);
	CRotatingObject *pEnemy = NULL;
	for (int i = 0; i < m_nObjects; ++i) {
		fHeight= pTerrain->GetHeight(EnemyPos[i].xPos, EnemyPos[i].zPos);
		pEnemy = new CRotatingObject();
		pEnemy->SetPosition(EnemyPos[i].xPos, fHeight + 10.0f, EnemyPos[i].zPos);
		pEnemy->setType(type(dre));
		pEnemy->setMovingDir(XMFLOAT3(0, 0, 0));
		pEnemy->SetRotationSpeed(90.0f);
		m_ppObjects[i] = pEnemy;
		m_ppObjects[i]->SetMesh(0, pSphereMesh);
		m_ppObjects[i]->SetOOBB(pSphereMesh->GetBoundingBox());
	}


	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}
void CEnemyShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	ObjectShader::Render(pd3dCommandList, pCamera);

	//모든 게임 객체의 인스턴싱 데이터를 버퍼에 저장한다.
	UpdateShaderVariables(pd3dCommandList);

	//하나의 정점 데이터를 사용하여 모든 게임 객체(인스턴스)들을 렌더링한다.
	m_ppObjects[1]->Render(pd3dCommandList, pCamera, m_nObjects,
		m_d3dInstancingBufferView);
}
void CEnemyShader::AnimateObjects(float fTime)
{
	for (int i = 0; i < m_nObjects; ++i)
		m_ppObjects[i]->Animate(fTime);
}