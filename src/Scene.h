#pragma once
#include "Shader.h"
#include "GameObject.h"
#include "Camera.h"
class CScene
{
protected: 
	//씬은 셰이더들의 집합이다. 셰이더들은 게임 객체들의 집합이다. 
	//CShader **m_ppShaders = NULL; 
	//int m_nShaders = 0;

	//씬은 게임 객체들의 집합이다. 게임 객체는 셰이더를 포함한다. 
	CGameObject **m_ppObjects = NULL; 
	int m_nObjects = 0;

	ID3D12RootSignature *m_pd3dGraphicsRootSignature = NULL;


public:
	CScene();
	~CScene();

	void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	void ReleaseObjects();

	bool ProcessInput(UCHAR *pKeysBuffer);
	void AnimateObjects(float fTimeElapsed);
	void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);

	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam); 
	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	
	void ReleaseUploadBuffers();


	//루트 시그너쳐 생성
	ID3D12RootSignature *CreateGraphicsRootSignature(ID3D12Device *pd3dDevice); 
	ID3D12RootSignature *GetGraphicsRootSignature();

	//파이프라인 상태를 나타내는 인터페이스 포인터 
	//ID3D12PipelineState *m_pd3dPipelineState; 
};

