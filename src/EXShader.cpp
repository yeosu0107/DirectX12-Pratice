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
		m_pcbMappedGameObjects[j].m_xmcColor = XMFLOAT4(0.9f, 0.0f, 0.0f, 0.0f);
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
	m_ppObjects[0]->SetMesh(paticleMesh);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CPaticlesShader::Render(ID3D12GraphicsCommandList * pd3dCommandList, CCamera * pCamera)
{
	if (!run)
		return;

	ObjectShader::Render(pd3dCommandList, pCamera);

	//��� ���� ��ü�� �ν��Ͻ� �����͸� ���ۿ� �����Ѵ�.
	UpdateShaderVariables(pd3dCommandList);

	//�ϳ��� ���� �����͸� ����Ͽ� ��� ���� ��ü(�ν��Ͻ�)���� �������Ѵ�.
	m_ppObjects[0]->Render(pd3dCommandList, pCamera, m_nObjects,
		m_d3dInstancingBufferView);
}

void CPaticlesShader::AnimateObjects(float fTimeElapsed)
{
	if (!run)
		return;

	if (scale < 0.3f) {
		run = false;
		for (int i = 0; i < m_nObjects; ++i)
			m_ppObjects[i]->setScale(1.0f);
	}

	for (int j = 0; j < m_nObjects; j++) {
		m_ppObjects[j]->Scale(0.95f);
		m_ppObjects[j]->Animate(fTimeElapsed);
	}
}
void CPaticlesShader::setPosition(XMFLOAT3 pos)
{
	for (int i = 0; i < m_nObjects; ++i)
		m_ppObjects[i]->SetPosition(pos);
}