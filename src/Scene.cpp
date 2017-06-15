#include "stdafx.h"
#include "Scene.h"
#include "EXShader.h"


CScene::CScene()
{
	
}


CScene::~CScene()
{
}

void CScene::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	//�׷��� ��Ʈ �ñ׳��ĸ� �����Ѵ�. 
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);

	

	//���̴� ����
	m_nShaders = 3;
	m_ppShaders = new CShader*[m_nShaders];

	//�� ���̴�
	CTerrainShader* mapShader = new CTerrainShader();
	mapShader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	mapShader->BuildObjects(pd3dDevice, pd3dCommandList);
	
	m_pTerrain = (CHeightMapTerrain*)mapShader->GetObjects()[0];

	m_ppShaders[0] = mapShader;


	//�� ���̴�
	CInstancingShader* EnemyShader = new CInstancingShader();
	EnemyShader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	EnemyShader->BuildObjects(pd3dDevice, pd3dCommandList);
	
	m_nEnemy = EnemyShader->getObjectsNum();
	m_pEnemy = EnemyShader->GetObjects();

	m_ppShaders[1] = EnemyShader;

	//�̷μ��̴�
	CMazeShader* MazeShader = new CMazeShader();
	MazeShader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	MazeShader->BuildObjects(pd3dDevice, pd3dCommandList, m_pTerrain);

	m_nWall = MazeShader->getObjectsNum();
	m_pWall = MazeShader->GetObjects();

	m_ppShaders[2] = MazeShader;

	//�Ѿ� ��ü ���̴� ����
	CBulletShader* bulletShader = new CBulletShader();
	bulletShader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	bulletShader->BuildObjects(pd3dDevice, pd3dCommandList);

	m_nBullet = bulletShader->getObjectsNum();
	m_pBullet = bulletShader->GetObjects();

	m_BulletShader = bulletShader;

	//��ƼŬ ���̴� ����
	m_nPaticleShaders = 1 + m_nEnemy;
	m_PaticleShaders = new CPaticlesShader*[m_nPaticleShaders];

	CPaticlesShader* paticleShader = NULL;
	for (int i = 0; i < m_nPaticleShaders; ++i) {
		paticleShader = new CPaticlesShader();
		paticleShader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
		paticleShader->BuildObjects(pd3dDevice, pd3dCommandList);

		m_PaticleShaders[i] = paticleShader;
	}
	
	
}

void CScene::ReleaseObjects()
{
	if (m_pd3dGraphicsRootSignature) 
		m_pd3dGraphicsRootSignature->Release();

	if (m_ppShaders){
		for (int i = 0; i < m_nShaders; i++) { 
			m_ppShaders[i]->ReleaseShaderVariables(); 
			m_ppShaders[i]->ReleaseObjects();
		} 
		delete[] m_ppShaders;
	}

}

bool CScene::ProcessInput(UCHAR *pKeysBuffer)
{
	
	return false;
}

void CScene::playerUpdate(XMFLOAT3 pos, XMFLOAT3 look)
{
	m_BulletShader->Shoot(pos, look);
}


void CScene::AnimateObjects(float fTimeElapsed, XMFLOAT3 player)
{
	m_BulletShader->AnimateObjects(fTimeElapsed);

	for (int i = 0; i < m_nShaders; ++i) { 
		m_ppShaders[i]->updatePlayerPos(player);
		m_ppShaders[i]->AnimateObjects(fTimeElapsed);
	}

	for (int i = 0; i < m_nPaticleShaders; ++i) {
		if (m_PaticleShaders[i]->getRun())
			m_PaticleShaders[i]->AnimateObjects(fTimeElapsed);
	}
}

bool CScene::CrashObjects(BoundingOrientedBox& player, bool playerDeath)
{
	for (int i = 0; i < m_nWall; ++i) {
		ContainmentType containType = m_pWall[i]->getOOBB()->Contains(player);
		switch (containType) {
		case CONTAINS:
			break;
		case DISJOINT:
			break;
		case INTERSECTING:
			printf("crash\n");
			//return true;
		}
	}
	return false;
}

void CScene::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera) 
{
	pCamera->SetViewportsAndScissorRects(pd3dCommandList); 
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);

	if (pCamera) 
		pCamera->UpdateShaderVariables(pd3dCommandList);
	
	for (int i = 0; i < m_nShaders; ++i) { 
		m_ppShaders[i]->Render(pd3dCommandList, pCamera);
	} 
	
	m_BulletShader->Render(pd3dCommandList, pCamera);

	for (int i = 0; i < m_nPaticleShaders; ++i) {
		if (m_PaticleShaders[i]->getRun())
			m_PaticleShaders[i]->Render(pd3dCommandList, pCamera);
	}
}


bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

bool CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

void CScene::ReleaseUploadBuffers() { 
	if (m_ppShaders) { 
		for (int j = 0; j < m_nShaders; j++) 
				m_ppShaders[j]->ReleaseUploadBuffers();
	} 
}

ID3D12RootSignature *CScene::GetGraphicsRootSignature() { 
	return m_pd3dGraphicsRootSignature; 
}

ID3D12RootSignature *CScene::CreateGraphicsRootSignature(ID3D12Device *pd3dDevice) {
	ID3D12RootSignature *pd3dGraphicsRootSignature = NULL;

	//��Ʈ �Ķ���� ����
	D3D12_ROOT_PARAMETER pd3dRootParameters[3];
	
	pd3dRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pd3dRootParameters[0].Constants.Num32BitValues = 16;
	pd3dRootParameters[0].Constants.ShaderRegister = 0; //b0: Player
	pd3dRootParameters[0].Constants.RegisterSpace = 0;
	pd3dRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	
	pd3dRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pd3dRootParameters[1].Constants.Num32BitValues = 32;
	pd3dRootParameters[1].Constants.ShaderRegister = 1; //b1: Camera
	pd3dRootParameters[1].Constants.RegisterSpace = 0;
	pd3dRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	
	pd3dRootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
	pd3dRootParameters[2].Descriptor.ShaderRegister = 0; //t0
	pd3dRootParameters[2].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	
	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;
	
	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
	d3dRootSignatureDesc.pParameters = pd3dRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers = 0;
	d3dRootSignatureDesc.pStaticSamplers = NULL;
	
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;
	
	ID3DBlob *pd3dSignatureBlob = NULL;
	ID3DBlob *pd3dErrorBlob = NULL;
	
	D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		&pd3dSignatureBlob, &pd3dErrorBlob);
	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(),
		pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void
			**)&pd3dGraphicsRootSignature);
	
	if (pd3dSignatureBlob) 
		pd3dSignatureBlob->Release();
	
	if (pd3dErrorBlob)
		pd3dErrorBlob->Release();
	return(pd3dGraphicsRootSignature);
}

CGameObject *CScene::PickObjectPointedByCursor(int xClient, int yClient, CCamera
	*pCamera)
{
	if (!pCamera) return(NULL);
	XMFLOAT4X4 xmf4x4View = pCamera->GetViewMatrix();
	XMFLOAT4X4 xmf4x4Projection = pCamera->GetProjectionMatrix();
	D3D12_VIEWPORT d3dViewport = pCamera->GetViewport();
	XMFLOAT3 xmf3PickPosition;

	/*ȭ�� ��ǥ���� �� (xClient, yClient)�� ȭ�� ��ǥ ��ȯ�� 
	����ȯ�� ���� ��ȯ�� ����ȯ�� �Ѵ�. �� ����� ī�޶�
	��ǥ���� ���̴�. ���� ����� ī�޶󿡼� z-������ �Ÿ��� 1�̹Ƿ� 
	z-��ǥ�� 1�� �����Ѵ�.*/

	xmf3PickPosition.x = (((2.0f * xClient) / d3dViewport.Width) - 1) /
		xmf4x4Projection._11;

	xmf3PickPosition.y = -(((2.0f * yClient) / d3dViewport.Height) - 1) /
		xmf4x4Projection._22;

	xmf3PickPosition.z = 1.0f;

	int nIntersected = 0;
	float fHitDistance = FLT_MAX, fNearestHitDistance = FLT_MAX;
	CGameObject *pIntersectedObject = NULL, *pNearestObject = NULL;
	//���̴��� ��� ���� ��ü�鿡 ���� ���콺 ��ŷ�� �����Ͽ� ī�޶�� ���� ����� ���� ��ü�� ���Ѵ�. 
	for (int i = 0; i < 2; i++)
	{
		pIntersectedObject = m_ppShaders[i]->PickObjectByRayIntersection(xmf3PickPosition,
			xmf4x4View, &fHitDistance);
		if (pIntersectedObject && (fHitDistance < fNearestHitDistance))
		{
			fNearestHitDistance = fHitDistance;
			pNearestObject = pIntersectedObject;
		}
	}
	return(pNearestObject);
}