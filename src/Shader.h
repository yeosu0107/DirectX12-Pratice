#pragma once
#include "GameObject.h"
#include "Camera.h"

struct CB_GAMEOBJECT_INFO 
{ 
	XMFLOAT4X4 m_xmf4x4World; 
};

class CShader
{
private: int 
	m_nReferences = 0;

protected: 
	//���̴��� �����ϴ� ���� ��ü���� ����Ʈ(�迭)�̴�. 
	CGameObject **m_ppObjects = NULL; 
	int m_nObjects = 0;

	//���������� ���� ��ü���� ����Ʈ(�迭)�̴�. 
	ID3D12PipelineState **m_ppd3dPipelineStates = NULL; 
	int m_nPipelineStates = 0;

public:
	CShader();
	virtual ~CShader();

	void AddRef() { m_nReferences++; } 
	void Release() { if (--m_nReferences <= 0) delete this; }

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout(); 
	virtual D3D12_RASTERIZER_DESC CreateRasterizerState(); 
	virtual D3D12_BLEND_DESC CreateBlendState(); 
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();

	//�������̴� ����Ʈ�ڵ� ����(������)
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob); 
	//�ʼ����̴� ����Ʈ�ڵ� ����(������
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);

	//���̴� �ҽ��ڵ� ������ & ����Ʈ �ڵ� ����ü�� ��ȯ
	D3D12_SHADER_BYTECODE CompileShaderFromFile(WCHAR *pszFileName, 
		LPCSTR pszShaderName, LPCSTR pszShaderProfile, ID3DBlob **ppd3dShaderBlob);


	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dRootSignature);
	
	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList, XMFLOAT4X4 *pxmf4x4World);
	
	virtual void ReleaseUploadBuffers();
	virtual void BuildObjects(ID3D12Device *pd3dDevice, 
		ID3D12GraphicsCommandList *pd3dCommandList, void *pContext, CGameObject** pObject); 
	virtual void AnimateObjects(float fTimeElapsed); 
	virtual void ReleaseObjects();
	
	virtual void OnPrepareRender(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);

};

class CPlayerShader : public CShader 
{
public: 
	CPlayerShader(); 
	virtual ~CPlayerShader();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob); 
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);
	virtual void CreateShader(ID3D12Device *pd3dDevice, 
		ID3D12RootSignature *pd3dGraphicsRootSignature);
};

class EnemyShader : public CShader
{
protected:

public:
	EnemyShader();
	virtual ~EnemyShader();

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList
		*pd3dCommandList);
	virtual void AnimateObjects(float fTimeElapsed);
	virtual void ReleaseObjects();
	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);
	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature
		*pd3dGraphicsRootSignature);
	virtual void ReleaseUploadBuffers();
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);
};

class CMapShader : public CShader
{
private:
	float width=300.0f;
	float height=200.0f;
	float depth=200.0f;

	CWallObject **m_ppObjects = NULL;
	
public:
	CMapShader();
	virtual ~CMapShader();

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList
		*pd3dCommandList);
	virtual void AnimateObjects(XMFLOAT3 pos, float fTimeElapsed);
	virtual void ReleaseObjects();
	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);
	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature
		*pd3dGraphicsRootSignature);
	virtual void ReleaseUploadBuffers();
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);

	CWallObject** getWalls() { return m_ppObjects; }
	int getWallnum() const { return m_nObjects; }

};