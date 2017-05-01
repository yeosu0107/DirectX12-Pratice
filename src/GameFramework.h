#pragma once

//-----------------------------------------------------------------------------
// File: GameFramework.h
//-----------------------------------------------------------------------------

#ifndef _GAME_APPLICATION_FRAMEWORK_H_
#define _GAME_APPLICATION_FRAMEWORK_H_

#include "stdafx.h"
#include "Timer.h"

class CGameFramework
{
private:
	HINSTANCE					m_hInstance;
	HWND						m_hWnd;
	
	int							client_width;
	int							client_height;

	//D3D ����
	//���丮 �������̽� ������
	IDXGIFactory4*				pdxgiFactory;
	//����ü�� �������̽� ������
	IDXGISwapChain3*			pdxgiSwapChain;
	//D3D12 ����̽� �������̽� ������
	ID3D12Device*				pd3Device;

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
	UINT64 m_nFenceValue;
	//�潺 �ڵ�
	HANDLE m_hFenceEvent;

#if defined(_DEBUG) 
	ID3D12Debug *m_pd3dDebugController;
#endif
	D3D12_VIEWPORT m_d3dViewport;
	D3D12_RECT m_d3dScissorRect; //����Ʈ�� ���� �簢���̴�.

	//���� �����ӿ�ũ ����
	bool						m_bActive;
	CGameTimer					m_GameTimer;
	_TCHAR						m_pszFrameRate[50];

	//���� ������Ʈ ����
public:
	CGameFramework(void);
	~CGameFramework(void);

	//���� �����ӿ�ũ ����
	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	void OnDestroy();
	void FrameAdvance();
	void SetActive(bool bActive) { m_bActive = bActive; }

	//D3D����
	void CreateSwapChain();
	void CreateDirect3DDevice();
	void CreateRtvAndDsvDescriptorHeaps();
	void CreateCommandQueueAndList();

	void CreateRenderTargetView();
	void CreateDepthStencilView();

	void WaitForGpuComplete();

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

#endif 