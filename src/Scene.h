#pragma once
#include "Shader.h"
#include "EXShader.h"
#include "GameObject.h"
#include "Camera.h"

enum playerStatus { Normal = 0, noMove, Death };

class CScene
{
private:

protected: 
	//씬은 셰이더들의 집합이다. 셰이더들은 게임 객체들의 집합이다.
	//랜더를 위해 사용
	CShader **m_ppShaders = NULL;
	int m_nShaders = 0;

	CBulletShader* m_BulletShader = NULL;

	CPaticlesShader** m_PaticleShaders = NULL;
	int m_nPaticleShaders = 0;
	int nowPaticle = 1;

	CGameObject **m_pEnemy = NULL;
	int m_nEnemy = 0;

	CGameObject **m_pBullet = NULL;
	int m_nBullet = 0;

	CGameObject** m_pWall = NULL;
	int m_nWall = 0;

	CHeightMapTerrain* m_pTerrain = NULL;

	ID3D12RootSignature *m_pd3dGraphicsRootSignature = NULL;

	


public:
	CScene();
	~CScene();

	void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	void ReleaseObjects();

	bool ProcessInput(UCHAR *pKeysBuffer);
	void AnimateObjects(float fTimeElapsed, XMFLOAT3 player);
	bool CrashObjects(BoundingOrientedBox& m_pPlayer, playerStatus& playerDeath);
	void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);

	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam); 
	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	
	void ReleaseUploadBuffers();

	void playerUpdate(XMFLOAT3 pos, XMFLOAT3 look);

	//루트 시그너쳐 생성
	ID3D12RootSignature *CreateGraphicsRootSignature(ID3D12Device *pd3dDevice); 
	ID3D12RootSignature *GetGraphicsRootSignature();

	CHeightMapTerrain *GetTerrain() { return(m_pTerrain); }

	CGameObject *PickObjectPointedByCursor(int xClient, int yClient, CCamera *pCamera);

	//파이프라인 상태를 나타내는 인터페이스 포인터 
	//ID3D12PipelineState *m_pd3dPipelineState; 
};

