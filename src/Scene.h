#pragma once
class CScene
{
public:
	CScene();
	~CScene();

	void BuildObjects(ID3D12Device *pd3dDevice);
	void ReleaseObjects();
	bool ProcessInput();
	void AnimateObjects(float fTimeElapsed);
	void Render(ID3D12GraphicsCommandList *pd3dCommandList);

	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam); 
	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	

	//��Ʈ �ñ׳��ĸ� ��Ÿ���� �������̽� ������
	ID3D12RootSignature *m_pd3dGraphicsRootSignature; 
	//���������� ���¸� ��Ÿ���� �������̽� ������ 
	ID3D12PipelineState *m_pd3dPipelineState; 
};

