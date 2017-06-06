#include "stdafx.h"
#include "Scene.h"


CScene::CScene()
{
	
}


CScene::~CScene()
{
}

void CScene::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	//그래픽 루트 시그너쳐를 생성한다. 
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);


	//쉐이더 생성
	m_nShaders = 2;
	m_ppShaders = new CShader*[m_nShaders];

	CMapShader* mapShader = new CMapShader();
	mapShader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	mapShader->BuildObjects(pd3dDevice, pd3dCommandList);
	
	m_ppShaders[0] = mapShader;

	m_nWall = mapShader->getObjectsNum();
	m_pWall = (CWallObject**)mapShader->GetObjects();
	
	float width = m_pWall[0]->getWidth();
	float height = m_pWall[0]->getHeight();

	m_wallPlanes[0] = XMFLOAT4(+1.0f, 0.0f, 0.0f, width);
	m_wallPlanes[1] = XMFLOAT4(-1.0f, 0.0f, 0.0f, width);
	m_wallPlanes[2] = XMFLOAT4(0.0f, +1.0f, 0.0f, height);
	m_wallPlanes[3] = XMFLOAT4(0.0f, -1.0f, 0.0f, height);

	EnemyShader* enemyShader = new EnemyShader();
	enemyShader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	enemyShader->BuildObjects(pd3dDevice, pd3dCommandList);
	enemyShader->BuildObjects(pd3dDevice, pd3dCommandList);

	m_ppShaders[1] = enemyShader;
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




void CScene::AnimateObjects(float fTimeElapsed, XMFLOAT3 player)
{
	for (int i = 0; i < m_nShaders; i++) { 
		m_ppShaders[i]->updatePlayerPos(player);
		m_ppShaders[i]->AnimateObjects(fTimeElapsed);
	}

}

bool CScene::CrashObjects(BoundingOrientedBox& player)
{
	//printf("%f %f\n", player.Center.y, player.Extents.y);
	for (int index = 0; index < m_nWall; ++index) {
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
					//printf("%d %d 충돌\n", index, i);
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
	
	for (int i = 0; i < m_nShaders; i++) { 
		m_ppShaders[i]->Render(pd3dCommandList, pCamera);
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

	//루트 파라미터 설정
	D3D12_ROOT_PARAMETER pd3dRootParameters[2]; 

	pd3dRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS; 
	pd3dRootParameters[0].Constants.Num32BitValues = 16; 
	pd3dRootParameters[0].Constants.ShaderRegister = 0; 
	pd3dRootParameters[0].Constants.RegisterSpace = 0; 
	pd3dRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; 
	
	pd3dRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS; 
	pd3dRootParameters[1].Constants.Num32BitValues = 32; 
	pd3dRootParameters[1].Constants.ShaderRegister = 1; 
	pd3dRootParameters[1].Constants.RegisterSpace = 0; 
	pd3dRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;


	//시그너쳐 플래그 설정
	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = 
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | 
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | 
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | 
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS | 
		D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;


	//매개변수가 없는 루트 시그너쳐를 생성한다. 
	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc; 
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC)); 

	d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters); 
	d3dRootSignatureDesc.pParameters = pd3dRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers = 0; 
	d3dRootSignatureDesc.pStaticSamplers = NULL; 
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;

	ID3DBlob *pd3dSignatureBlob = NULL; 
	ID3DBlob *pd3dErrorBlob = NULL;

	::D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, 
		&pd3dSignatureBlob, &pd3dErrorBlob); 
	
	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), 
		pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), 
		(void **)&pd3dGraphicsRootSignature);

	if (pd3dSignatureBlob)
		pd3dSignatureBlob->Release(); 
	if (pd3dErrorBlob) 
		pd3dErrorBlob->Release();

	return(pd3dGraphicsRootSignature);
}
