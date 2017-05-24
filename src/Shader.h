#pragma once
#include "GameObject.h"
class CShader
{
private: int 
	m_nReferences = 0;

protected: 
	//셰이더가 포함하는 게임 객체들의 리스트(배열)이다. 
	CGameObject **m_ppObjects = NULL; 
	int m_nObjects = 0;
	//파이프라인 상태 객체들의 리스트(배열)이다. 
	ID3D12PipelineState **m_ppd3dPipelineStates = NULL; int m_nPipelineStates = 0;

public:
	CShader();
	virtual ~CShader();

	void AddRef() { m_nReferences++; } 
	void Release() { if (--m_nReferences <= 0) delete this; }

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout(); 
	virtual D3D12_RASTERIZER_DESC CreateRasterizerState(); 
	virtual D3D12_BLEND_DESC CreateBlendState(); 
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();

	//정점셰이더 바이트코드 생성(컴파일)
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob); 
	//필셀셰이더 바이트코드 생성(컴파일
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);

	//셰이더 소스코드 컴파일 & 바이트 코드 구조체를 반환
	D3D12_SHADER_BYTECODE CompileShaderFromFile(WCHAR *pszFileName, 
		LPCSTR pszShaderName, LPCSTR pszShaderProfile, ID3DBlob **ppd3dShaderBlob);

	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dRootSignature);
	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList) { } 
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList) { } 
	virtual void ReleaseShaderVariables() { }

	virtual void ReleaseUploadBuffers();

	virtual void BuildObjects(ID3D12Device *pd3dDevice, 
		ID3D12GraphicsCommandList *pd3dCommandList, void *pContext = NULL); 
	virtual void AnimateObjects(float fTimeElapsed); 
	virtual void ReleaseObjects();
	
	virtual void OnPrepareRender(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList);

};

