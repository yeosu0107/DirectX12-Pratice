#include "stdafx.h"
#include "Shader.h"
#include "Player.h"


CShader::CShader()
{
}


CShader::~CShader()
{
	if (m_ppd3dPipelineStates) { 
		for (int i = 0; i < m_nPipelineStates; i++)
			if (m_ppd3dPipelineStates[i]) 
				m_ppd3dPipelineStates[i]->Release(); 
		delete[] m_ppd3dPipelineStates; 
	}
}

D3D12_INPUT_LAYOUT_DESC CShader::CreateInputLayout() {
	UINT nInputElementDescs = 2; 
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = 
		new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	//정점은 위치 벡터(POSITION)와 색상(COLOR)을 가진다. 
	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	
	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc; 
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs; 
	d3dInputLayoutDesc.NumElements = nInputElementDescs;
	return d3dInputLayoutDesc;
}

D3D12_RASTERIZER_DESC CShader::CreateRasterizerState()
{
	D3D12_RASTERIZER_DESC d3dRasterizerDesc; 
	::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID; 
	d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_BACK; 
	d3dRasterizerDesc.FrontCounterClockwise = FALSE; 
	d3dRasterizerDesc.DepthBias = 0; 
	d3dRasterizerDesc.DepthBiasClamp = 0.0f; 
	d3dRasterizerDesc.SlopeScaledDepthBias = 0.0f; 
	d3dRasterizerDesc.DepthClipEnable = TRUE; 
	d3dRasterizerDesc.MultisampleEnable = FALSE; 
	d3dRasterizerDesc.AntialiasedLineEnable = FALSE; 
	d3dRasterizerDesc.ForcedSampleCount = 0; 
	d3dRasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	return d3dRasterizerDesc;
}

D3D12_DEPTH_STENCIL_DESC CShader::CreateDepthStencilState() {
	D3D12_DEPTH_STENCIL_DESC d3dDepthStencilDesc; 
	::ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D12_DEPTH_STENCIL_DESC)); 
	d3dDepthStencilDesc.DepthEnable = TRUE;
	d3dDepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL; 
	d3dDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS; 
	d3dDepthStencilDesc.StencilEnable = FALSE; 
	d3dDepthStencilDesc.StencilReadMask = 0x00; 
	d3dDepthStencilDesc.StencilWriteMask = 0x00; 
	d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP; 
	d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP; 
	d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP; 
	d3dDepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER; 
	d3dDepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP; 
	d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP; 
	d3dDepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP; 
	d3dDepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;

	return d3dDepthStencilDesc;
}

D3D12_BLEND_DESC CShader::CreateBlendState() {
	D3D12_BLEND_DESC d3dBlendDesc; 
	::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC)); 
	d3dBlendDesc.AlphaToCoverageEnable = FALSE; 
	d3dBlendDesc.IndependentBlendEnable = FALSE; 
	d3dBlendDesc.RenderTarget[0].BlendEnable = FALSE; 
	d3dBlendDesc.RenderTarget[0].LogicOpEnable = FALSE; 
	d3dBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE; 
	d3dBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO; 
	d3dBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD; 
	d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE; 
	d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO; 
	d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD; 
	d3dBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP; 
	d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	return(d3dBlendDesc);
}

D3D12_SHADER_BYTECODE CShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob) { 
	//return(CompileShaderFromFile(L"Shaders.hlsl", "VSMain", "vs_5_1", ppd3dShaderBlob));
	D3D12_SHADER_BYTECODE d3dShaderByteCode; 
	d3dShaderByteCode.BytecodeLength = 0; 
	d3dShaderByteCode.pShaderBytecode = NULL;

	return(d3dShaderByteCode);

}

D3D12_SHADER_BYTECODE CShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob) { 
	//return(CompileShaderFromFile(L"Shaders.hlsl", "PSMain", "ps_5_1", ppd3dShaderBlob));
	D3D12_SHADER_BYTECODE d3dShaderByteCode; 
	d3dShaderByteCode.BytecodeLength = 0; 
	d3dShaderByteCode.pShaderBytecode = NULL;

	return(d3dShaderByteCode);
}

D3D12_SHADER_BYTECODE CShader::CompileShaderFromFile(WCHAR *pszFileName, 
	LPCSTR pszShaderName, LPCSTR pszShaderProfile, ID3DBlob **ppd3dShaderBlob)
{

	UINT nCompileFlags = 0;
#if defined(_DEBUG) 
	nCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION; 
#endif
	::D3DCompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderProfile, nCompileFlags, 0, ppd3dShaderBlob, NULL);
	D3D12_SHADER_BYTECODE d3dShaderByteCode; 
	d3dShaderByteCode.BytecodeLength = (*ppd3dShaderBlob)->GetBufferSize(); 
	d3dShaderByteCode.pShaderBytecode = (*ppd3dShaderBlob)->GetBufferPointer();
	
	return(d3dShaderByteCode);
}

void CShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature) {
	//그래픽스 파이프라인 상태 객체 배열을 생성한다. 
	//m_nPipelineStates = 1; 
	//m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];
	ID3DBlob *pd3dVertexShaderBlob = NULL, *pd3dPixelShaderBlob = NULL;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc; 
	::ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC)); 
	d3dPipelineStateDesc.pRootSignature = pd3dGraphicsRootSignature;
	d3dPipelineStateDesc.VS = CreateVertexShader(&pd3dVertexShaderBlob); 
	d3dPipelineStateDesc.PS = CreatePixelShader(&pd3dPixelShaderBlob); 
	d3dPipelineStateDesc.RasterizerState = CreateRasterizerState(); 
	d3dPipelineStateDesc.BlendState = CreateBlendState(); 
	d3dPipelineStateDesc.DepthStencilState = CreateDepthStencilState(); 
	d3dPipelineStateDesc.InputLayout = CreateInputLayout(); 
	d3dPipelineStateDesc.SampleMask = UINT_MAX; 
	d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; 
	d3dPipelineStateDesc.NumRenderTargets = 1; 
	d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; 
	d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT; 
	d3dPipelineStateDesc.SampleDesc.Count = 1; 
	d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

	pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc, 
		__uuidof(ID3D12PipelineState), (void **)&m_ppd3dPipelineStates[0]);
	
	if (pd3dVertexShaderBlob) 
		pd3dVertexShaderBlob->Release(); 
	if (pd3dPixelShaderBlob) 
		pd3dPixelShaderBlob->Release();
	if (d3dPipelineStateDesc.InputLayout.pInputElementDescs) 
		delete[] d3dPipelineStateDesc.InputLayout.pInputElementDescs;
}

void CShader::CreateShaderVariables(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
}

void CShader::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList)
{
}

void CShader::UpdateShaderVariable(ID3D12GraphicsCommandList * pd3dCommandList, XMFLOAT4X4 * pxmf4x4World)
{
	XMFLOAT4X4 xmf4x4World; 
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(pxmf4x4World))); 
	pd3dCommandList->SetGraphicsRoot32BitConstants(0, 16, &xmf4x4World, 0);

}

void CShader::ReleaseShaderVariables()
{
}

void CShader::ReleaseUploadBuffers() { 
	if (m_ppObjects) { 
		for (int j = 0; j < m_nObjects; j++) 
			if (m_ppObjects[j]) 
				m_ppObjects[j]->ReleaseUploadBuffers(); 
	} 
}


void CShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, 
	void *pContext, CGameObject** ppObjects) {
	m_nObjects = 1;
	m_ppObjects = ppObjects;
}

void CShader::ReleaseObjects() {
	if (m_ppObjects) {
		for (int j = 0; j < m_nObjects; j++)
			if (m_ppObjects[j])
				delete m_ppObjects[j];
		delete[] m_ppObjects;
	}
}

void CShader::AnimateObjects(float fTimeElapsed) {
	for (int j = 0; j < m_nObjects; j++) {
		m_ppObjects[j]->Animate(fTimeElapsed);
	}
}

void CShader::OnPrepareRender(ID3D12GraphicsCommandList *pd3dCommandList) { 
	//파이프라인에 그래픽스 상태 객체를 설정한다. 
	pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[0]); 
}

void CShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera) {
	OnPrepareRender(pd3dCommandList);
	
}

CPlayerShader::CPlayerShader() { 
}

CPlayerShader::~CPlayerShader() { 
}

D3D12_INPUT_LAYOUT_DESC CPlayerShader::CreateInputLayout() {
	UINT nInputElementDescs = 2; 
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = 
		new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];
	pd3dInputElementDescs[0] = { 
		"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, 
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 
	}; 
	
	pd3dInputElementDescs[1] = { 
		"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, 
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 
	};

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc; 
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs; 
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CPlayerShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob) { 
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSDiffused", "vs_5_1", ppd3dShaderBlob)); 
}

D3D12_SHADER_BYTECODE CPlayerShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob) {
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSDiffused", "ps_5_1", ppd3dShaderBlob));
}

void CPlayerShader::CreateShader(ID3D12Device *pd3dDevice, 
	ID3D12RootSignature *pd3dGraphicsRootSignature) {
	m_nPipelineStates = 1; 
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];
	CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
}

void CPlayerShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList
	*pd3dCommandList, void *pContext)
{
	m_nObjects = 1;
	m_ppObjects = new CGameObject*[m_nObjects];
	
	CTerrainPlayer* player = new CTerrainPlayer(pd3dDevice, pd3dCommandList, pContext, 1);

	m_ppObjects[0] = player;
}
void CPlayerShader::AnimateObjects(float fTimeElapsed)
{
	CShader::AnimateObjects(fTimeElapsed);
}
void CPlayerShader::ReleaseObjects()
{
	CShader::ReleaseObjects();
}
void CPlayerShader::ReleaseUploadBuffers()
{
	CShader::ReleaseUploadBuffers();
}
void CPlayerShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	CShader::Render(pd3dCommandList, pCamera);
	for (int j = 0; j < m_nObjects; j++) {
		if (m_ppObjects[j]) {
			m_ppObjects[j]->Render(pd3dCommandList, pCamera);
		}
	}
}

ObjectShader::ObjectShader() {}
ObjectShader::~ObjectShader() {}

void ObjectShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList
	*pd3dCommandList) {
	

}
void ObjectShader::AnimateObjects(float fTimeElapsed) {
	CShader::AnimateObjects(fTimeElapsed);
	/*for (int j = 0; j < m_nObjects; j++) {
		
		m_ppObjects[j]->Animate(fTimeElapsed);
		
	}*/
}
void ObjectShader::ReleaseObjects() {
	CShader::ReleaseObjects();
}

D3D12_INPUT_LAYOUT_DESC ObjectShader::CreateInputLayout() {
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new
		D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	pd3dInputElementDescs[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;
	return(d3dInputLayoutDesc);
}
D3D12_SHADER_BYTECODE ObjectShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob) {
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSDiffused", "vs_5_1",
		ppd3dShaderBlob));
}
D3D12_SHADER_BYTECODE ObjectShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob) {
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSDiffused", "ps_5_1",
		ppd3dShaderBlob));
}
void ObjectShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature
	*pd3dGraphicsRootSignature) {

	m_nPipelineStates = 1;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];
	CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
}
void ObjectShader::ReleaseUploadBuffers() {
	CShader::ReleaseUploadBuffers();
}
void ObjectShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera) {
	CShader::Render(pd3dCommandList, pCamera);
	for (int j = 0; j < m_nObjects; j++) {
		if (m_ppObjects[j]) {
			m_ppObjects[j]->Render(pd3dCommandList, pCamera);
		}
	}
}

CTerrainShader::CTerrainShader()
{
}
CTerrainShader::~CTerrainShader()
{
}

D3D12_INPUT_LAYOUT_DESC CTerrainShader::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];
	
	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	
	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;
	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CTerrainShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSDiffused", "vs_5_1",
		ppd3dShaderBlob));
}
D3D12_SHADER_BYTECODE CTerrainShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSDiffused", "ps_5_1",
		ppd3dShaderBlob));
}
void CTerrainShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature
	*pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];
	CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
}

void CTerrainShader::BuildObjects(ID3D12Device *pd3dDevice, 
	ID3D12GraphicsCommandList *pd3dCommandList) {

	m_nObjects = 1;
	m_ppObjects = new CGameObject*[m_nObjects];

	CHeightMapTerrain *m_pTerrain = NULL;

	//지형을 확대할 스케일 벡터이다. x-축과 z-축은 8배, y-축은 2배 확대한다. 
	XMFLOAT3 xmf3Scale(16.0f, 2.0f, 16.0f);
	XMFLOAT4 xmf4Color(0.0f, 0.2f, 0.0f, 0.0f);

	//지형을 높이 맵 이미지 파일(HeightMap.raw)을 사용하여 생성한다. 높이 맵의 크기는 가로x세로(257x257)이다. 
#ifdef _WITH_TERRAIN_PARTITION
	/*하나의 격자 메쉬의 크기는 가로x세로(17x17)이다. 지형 전체는 가로 방향으로 16개, 세로 방향으로 16의 격자 메
	쉬를 가진다. 지형을 구성하는 격자 메쉬의 개수는 총 256(16x16)개가 된다.*/
	m_pTerrain = new CHeightMapTerrain(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature,
		_T("../Assets/Image/Terrain/HeightMap.raw"), 257, 257, 17, 17, xmf3Scale, xmf4Color);
#else

	//지형을 하나의 격자 메쉬(257x257)로 생성한다. 
	m_pTerrain = new CHeightMapTerrain(pd3dDevice, pd3dCommandList,
		_T("HeightMap.raw"), 257, 257, 257, 257, xmf3Scale, xmf4Color);
#endif

	m_ppObjects[0] = m_pTerrain;
}

void CTerrainShader::ReleaseUploadBuffers()
{
	CShader::ReleaseUploadBuffers();
}

void CTerrainShader::Render(ID3D12GraphicsCommandList * pd3dCommandList, CCamera * pCamera)
{
	if (m_ppObjects) {
		CShader::Render(pd3dCommandList, pCamera);
		for (int j = 0; j < m_nObjects; j++) {
			if (m_ppObjects[j]) {
				m_ppObjects[j]->Render(pd3dCommandList, pCamera);
			}
		}
	}
}

CInstancingShader::CInstancingShader()
{
}
CInstancingShader::~CInstancingShader()
{
}

D3D12_INPUT_LAYOUT_DESC CInstancingShader::CreateInputLayout()
{
	UINT nInputElementDescs = 7;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new
		D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];
	//정점 정보를 위한 입력 원소이다. 
	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	};
	pd3dInputElementDescs[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	//인스턴싱 정보를 위한 입력 원소이다. 
	pd3dInputElementDescs[2] = { "WORLDMATRIX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0,
	D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 };
	pd3dInputElementDescs[3] = { "WORLDMATRIX", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
	D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 };
	pd3dInputElementDescs[4] = { "WORLDMATRIX", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32,
	D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 };
	pd3dInputElementDescs[5] = { "WORLDMATRIX", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48,
	D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 };
	pd3dInputElementDescs[6] = { "INSTANCECOLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,
	64, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 };
	
	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;
	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CInstancingShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSInstancing", "vs_5_1",
		ppd3dShaderBlob));
}
D3D12_SHADER_BYTECODE CInstancingShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSInstancing", "ps_5_1",
		ppd3dShaderBlob));
}
void CInstancingShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature
	*pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];
	CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
}

void CInstancingShader::CreateShaderVariables(ID3D12Device *pd3dDevice,
	ID3D12GraphicsCommandList *pd3dCommandList)
{
	//인스턴스 정보를 저장할 정점 버퍼를 업로드 힙 유형으로 생성한다. 
	m_pd3dcbGameObjects = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL,
		sizeof(VS_VB_INSTANCE) * m_nObjects, D3D12_HEAP_TYPE_UPLOAD,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);
	
	//정점 버퍼(업로드 힙)에 대한 포인터를 저장한다. 
	m_pd3dcbGameObjects->Map(0, NULL, (void **)&m_pcbMappedGameObjects);
	
	//정점 버퍼에 대한 뷰를 생성한다. 
	m_d3dInstancingBufferView.BufferLocation =
		m_pd3dcbGameObjects->GetGPUVirtualAddress();
	m_d3dInstancingBufferView.StrideInBytes = sizeof(VS_VB_INSTANCE);
	m_d3dInstancingBufferView.SizeInBytes = sizeof(VS_VB_INSTANCE) * m_nObjects;
}
void CInstancingShader::ReleaseShaderVariables()
{
	if (m_pd3dcbGameObjects) 
		m_pd3dcbGameObjects->Unmap(0, NULL);
	if (m_pd3dcbGameObjects)
		m_pd3dcbGameObjects->Release();
}

//인스턴싱 정보(객체의 월드 변환 행렬과 색상)를 정점 버퍼에 복사한다.
void CInstancingShader::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
	for (int j = 0; j < m_nObjects; j++)
	{
		if (!m_ppObjects[j]->getDie()) {
			/*m_pcbMappedGameObjects[j].m_xmcColor = (j % 2) ? XMFLOAT4(0.5f, 0.0f, 0.0f, 0.0f) :
				XMFLOAT4(0.0f, 0.0f, 0.5f, 0.0f);*/
			m_pcbMappedGameObjects[j].m_xmcColor = m_ppObjects[j]->getColor();
			XMStoreFloat4x4(&m_pcbMappedGameObjects[j].m_xmf4x4Transform,
				XMMatrixTranspose(XMLoadFloat4x4(&m_ppObjects[j]->getMatrix())));
		}
	}
}

void CInstancingShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{  
	m_nObjects = 1;
	m_ppObjects = new CGameObject*[m_nObjects];

	std::default_random_engine dre(1000);
	std::uniform_int_distribution<int> xPos(-mapWidth / 2 + 13.0f, mapWidth / 2 - 13.0f);
	std::uniform_int_distribution<int> yPos(-mapHeight / 2 + 13.0f, mapHeight / 2 - 13.0f);
	std::uniform_int_distribution<int> zPos(0 + 50.0f, mapDepth*3);
	std::uniform_int_distribution<int> type(0, 3);

	CRotatingObject *pRotatingObject = NULL;
	for (int i = 0; i < m_nObjects; ++i) {
		pRotatingObject = new CRotatingObject();
		//각 정육면체 객체의 위치를 설정한다. 
		pRotatingObject->SetPosition(xPos(dre), yPos(dre), zPos(dre));
		pRotatingObject->setType(type(dre));
		//pRotatingObject->SetRotationAxis(XMFLOAT3(1.0f, 3.0f, 1.0f));
		//pRotatingObject->setMovingDir(XMFLOAT3(0.5f, 0.5f, 0.5f));
		pRotatingObject->SetRotationSpeed(90.0f);
		pRotatingObject->SetObject(12.0f, 12.0f, 12.0f);
		m_ppObjects[i] = pRotatingObject;
	}

	CCube *pCubeMesh = new CCube(pd3dDevice, pd3dCommandList, 12.0f, 12.0f, 12.0f);
	m_ppObjects[0]->SetMesh(0, pCubeMesh);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}
void CInstancingShader::ReleaseObjects()
{
	ObjectShader::ReleaseObjects();
}
void CInstancingShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera
	*pCamera)
{
	ObjectShader::Render(pd3dCommandList, pCamera);

	//모든 게임 객체의 인스턴싱 데이터를 버퍼에 저장한다.
	UpdateShaderVariables(pd3dCommandList);
	
	//하나의 정점 데이터를 사용하여 모든 게임 객체(인스턴스)들을 렌더링한다.
	m_ppObjects[0]->Render(pd3dCommandList, pCamera, m_nObjects,
		m_d3dInstancingBufferView);
}

void CInstancingShader::AnimateObjects(float fTimeElapsed)
{
	for (int j = 0; j < m_nObjects; j++) {
		if (m_ppObjects[j]->GetPosition().z+100.0f < playerPos.z)
			m_ppObjects[j]->Reset(playerPos);

		m_ppObjects[j]->Animate(fTimeElapsed);
	}
}
