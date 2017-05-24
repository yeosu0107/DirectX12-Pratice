#pragma once
#include "Shader.h"
#include "GameObject.h"
#include "Camera.h"
class CScene
{
protected: 
	//���� ���̴����� �����̴�. ���̴����� ���� ��ü���� �����̴�. 
	//CShader **m_ppShaders = NULL; 
	//int m_nShaders = 0;

	//���� ���� ��ü���� �����̴�. ���� ��ü�� ���̴��� �����Ѵ�. 
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


	//��Ʈ �ñ׳��� ����
	ID3D12RootSignature *CreateGraphicsRootSignature(ID3D12Device *pd3dDevice); 
	ID3D12RootSignature *GetGraphicsRootSignature();

	//���������� ���¸� ��Ÿ���� �������̽� ������ 
	//ID3D12PipelineState *m_pd3dPipelineState; 
};

