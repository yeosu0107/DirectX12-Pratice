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
	//±×·¡ÇÈ ·çÆ® ½Ã±×³ÊÃÄ¸¦ »ý¼ºÇÑ´Ù. 
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);


	//¼ÎÀÌ´õ »ý¼º
	m_nShaders = 2;
	m_ppShaders = new CShader*[m_nShaders];

	//¸Ê ¼ÎÀÌ´õ
	CMapShader* mapShader = new CMapShader();
	mapShader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	mapShader->BuildObjects(pd3dDevice, pd3dCommandList);
	
	m_ppShaders[0] = mapShader;

	m_nWall = mapShader->getObjectsNum();
	m_pWall = mapShader->GetObjects();
	
	float width = m_pWall[0]->getWidth();
	float height = m_pWall[0]->getHeight();

	m_wallPlanes[0] = XMFLOAT4(+1.0f, 0.0f, 0.0f, width);
	m_wallPlanes[1] = XMFLOAT4(-1.0f, 0.0f, 0.0f, width);
	m_wallPlanes[2] = XMFLOAT4(0.0f, +1.0f, 0.0f, height);
	m_wallPlanes[3] = XMFLOAT4(0.0f, -1.0f, 0.0f, height);

	//Àû ¼ÎÀÌ´õ
	CInstancingShader* EnemyShader = new CInstancingShader();
	EnemyShader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	EnemyShader->BuildObjects(pd3dDevice, pd3dCommandList);
	
	m_nEnemy = EnemyShader->getObjectsNum();
	m_pEnemy = EnemyShader->GetObjects();

	m_ppShaders[1] = EnemyShader;


	//ÃÑ¾Ë °´Ã¼ ¼ÎÀÌ´õ »ý¼º
	CBulletShader* bulletShader = new CBulletShader();
	bulletShader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	bulletShader->BuildObjects(pd3dDevice, pd3dCommandList);

	m_nBullet = bulletShader->getObjectsNum();
	m_pBullet = bulletShader->GetObjects();

	m_BulletShader = bulletShader;

	//ÆÄÆ¼Å¬ ¼ÎÀÌ´õ »ý¼º
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
		
	for (int i = 0; i < m_nEnemy; ++i) {
		//Àû °´Ã¼ - º®
		int nindex = -1;
		for (int index = 0; index < m_nWall; ++index) {
			ContainmentType containType = m_pWall[index]->getOOBB()->Contains(*m_pEnemy[i]->getOOBB());
			switch (containType) {
			case CONTAINS:
				break;
			case DISJOINT:
				break;
			case INTERSECTING: {
				int nPlaneIndex = -1;
				nindex = index;
				for (int j = 0; j < 4; j++)
				{
					PlaneIntersectionType intersectType = m_pEnemy[i]->getOOBB()->Intersects(XMLoadFloat4(&m_wallPlanes[j]));
					if (intersectType == INTERSECTING)
					{
						nPlaneIndex = j;
						break;
					}
				}
				if (nPlaneIndex != -1)
				{
					XMVECTOR xmvNormal = XMVectorSet(m_wallPlanes[nPlaneIndex].x, m_wallPlanes[nPlaneIndex].y, m_wallPlanes[nPlaneIndex].z, 0.0f);
					XMVECTOR xmvReflect = XMVector3Reflect(XMLoadFloat3(&m_pEnemy[i]->getMovingDir()), xmvNormal);
					XMStoreFloat3(&m_pEnemy[i]->getMovingDir(), xmvReflect);
				}
			}
			break;
			}
			if (nindex != -1)
				break;
		}

		for (int bi = 0; bi < m_nBullet; ++bi) {
			if (m_pBullet[bi]->getDie())
				continue;
			if (m_pEnemy[i]->getOOBB()->Intersects(*m_pBullet[bi]->getOOBB())) {
				m_PaticleShaders[nowPaticle]->setPosition(m_pEnemy[i]->GetPosition());
				m_PaticleShaders[nowPaticle]->setRun(m_pEnemy[i]->getColor());
				m_pEnemy[i]->Reset(player.Center);
				//m_pBullet[bi]->setDie(true);
				
				nowPaticle += 1;
				if (nowPaticle > m_nPaticleShaders-1)
					nowPaticle = 1;
			}
		}

		//Àû °´Ã¼ - Àû °´Ã¼
		for (int j = (i + 1); j < m_nEnemy; ++j) {
			if (m_pEnemy[i]->getOOBB()->Intersects(*m_pEnemy[j]->getOOBB())) {
				XMFLOAT3 tDir = m_pEnemy[i]->getMovingDir();
				m_pEnemy[i]->setMovingDir(m_pEnemy[j]->getMovingDir());
				m_pEnemy[j]->setMovingDir(tDir);
			}
		}
	}
	if (playerDeath)
		return true;

	//ÇÃ·¹ÀÌ¾î - Àû °´Ã¼
	for (int index = 0; index < m_nEnemy; ++index) {
		ContainmentType containType = m_pEnemy[index]->getOOBB()->Contains(player);
		switch (containType) {
		case CONTAINS:
			break;
		case DISJOINT:
			break;
		case INTERSECTING:
			m_PaticleShaders[0]->setPosition(player.Center);
			m_PaticleShaders[0]->setRun(XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f));
			return true;
		}
	}
	for (int index = 0; index < m_nWall; ++index) {
		//ÇÃ·¹ÀÌ¾î - º®
		ContainmentType containType = m_pWall[index]->getOOBB()->Contains(player);
		switch (containType) {
		case CONTAINS:
			break;
		case DISJOINT:
			break;
		case INTERSECTING:
			for (int i = 0; i < 4; ++i) {
				PlaneIntersectionType intersectType = player.Intersects(XMLoadFloat4(&m_wallPlanes[i]));
				if (intersectType == BACK || intersectType == INTERSECTING) {
					m_PaticleShaders[0]->setPosition(player.Center);
					m_PaticleShaders[0]->setRun(XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f));
					return true;
				}
			}
			
			break;
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

	//·çÆ® ÆÄ¶ó¹ÌÅÍ ¼³Á¤
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
