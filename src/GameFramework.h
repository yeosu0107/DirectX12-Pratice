#pragma once

//-----------------------------------------------------------------------------
// File: GameFramework.h
//-----------------------------------------------------------------------------

#ifndef _GAME_APPLICATION_FRAMEWORK_H_
#define _GAME_APPLICATION_FRAMEWORK_H_

#include "stdafx.h"
#include "Timer.h"
#include "Scene.h"
#include "Camera.h"
#include "Player.h"

class UI;
class Score;

class CGameFramework
{
private:
	CScene						*m_pScene;
	HINSTANCE					m_hInstance;
	HWND						m_hWnd;
	
	int							client_width;
	int							client_height;

	//D3D ����
	//���丮 �������̽� ������
	ComPtr<IDXGIFactory4>		pdxgiFactory;
	//����ü�� �������̽� ������
	IDXGISwapChain3*			pdxgiSwapChain;
	//D3D12 ����̽� �������̽� ������
	ComPtr<ID3D12Device>		pd3Device;
	//���߻��ø� Ȱ��/��Ȱ��
	bool						bMsaa4xEnable = false;
	//���߻��ø� ����Ƽ ���� ����
	UINT						nMsaa4xQualityLevels = 0;
	//���� ü���� �ĸ� ������ ����
	static const UINT m_nSwapChainBuffers = 2;
	//���� ���� ü���� �ĸ� ���� �ε���
	UINT m_nSwapChainBufferIndex;

	//����Ÿ�ٹ���
	ID3D12Resource *m_ppd3dRenderTargetBuffers[m_nSwapChainBuffers];
	//������ �� �������̽� ������
	ID3D12DescriptorHeap *m_pd3dRtvDescriptorHeap;
	//����Ÿ�� �������� ���� ũ��
	UINT m_nRtvDescriptorIncrementSize;

	//����-���ٽ� ����
	ID3D12Resource *m_pd3dDepthStencilBuffer;
	//������ �� �������̽� ������
	ID3D12DescriptorHeap *m_pd3dDsvDescriptorHeap;
	//����-���Ľ� ������ ���� ũ��
	UINT m_nDsvDescriptorIncrementSize;

	//��� ť
	ID3D12CommandQueue *m_pd3dCommandQueue;
	//��� ���������
	ID3D12CommandAllocator *m_pd3dCommandAllocator;
	//��ɸ���Ʈ
	ID3D12GraphicsCommandList *m_pd3dCommandList;

	//�潺 �������̽� ������
	ID3D12Fence *m_pd3dFence;
	//�潺 ��
	UINT64 m_nFenceValue[m_nSwapChainBuffers];
	//�潺 �ڵ�
	HANDLE m_hFenceEvent;

#if defined(_DEBUG) 
	ID3D12Debug *m_pd3dDebugController;
#endif
	CGameTimer					m_GameTimer;
	_TCHAR						m_pszFrameRate[50];

	HDC							m_hDCFrameBuffer;

	UI*							m_ui = NULL;

public:
	CPlayerShader* playerShader = NULL;
	

	CCamera *m_pCamera = NULL;
	CPlayer *m_pPlayer = NULL;
	POINT m_ptOldCursorPos;
	//TRACKMOUSEEVENT mouseEvent;

	CGameFramework(void);
	~CGameFramework(void);

	//���� �����ӿ�ũ ����
	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	void OnDestroy();
	void FrameAdvance();
	

	//D3D����
	void CreateSwapChain();
	void CreateDirect3DDevice();
	void CreateRtvAndDsvDescriptorHeaps();
	void CreateCommandQueueAndList();

	void CreateRenderTargetView();
	void CreateDepthStencilView();

	void OnResizeBackBuffers();

	void WaitForGpuComplete();

	//�����ϱ� 06
	void MoveToNextFrame();

	//���콺, Ű���� �����
	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	

	//������Ʈ ����
	void BuildObjects();
	void ReleaseObjects();
	void AnimateObjects();
	void ProcessInput();


};


class Score {
private:
	float distance;
	float killCount;
	float score;
public:
	Score() : distance{ 0 }, killCount{ 0 }, score{ 0 } {}
	~Score() {}
	void killEnemy() { killCount++; }
	void moveForward(float dist) { distance += dist; }
	int setScore() {
		score = killCount * 100 + distance / 10;
		return (int)score;
	}
	void ResetScore() { distance = 0; killCount = 0; score = 0; }
	float getDist() const { return distance; }
};

class UI {
private:
	wchar_t m_score[50];
	wchar_t m_start[30];
	wchar_t m_gameover[30];
	wchar_t m_restart[40];
	wchar_t m_boostGauag[30];
	//wchar_t m_speed[30];
	wchar_t m_bulletspeed[30];
public:
	UI();
	~UI();
	void DrawUI(HDC m_hDCFrameBuffer, int status, int score, int boost, int bullet);
};

#endif 