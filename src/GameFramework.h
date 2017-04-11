#pragma once

//-----------------------------------------------------------------------------
// File: GameFramework.h
//-----------------------------------------------------------------------------

#ifndef _GAME_APPLICATION_FRAMEWORK_H_
#define _GAME_APPLICATION_FRAMEWORK_H_

#include "stdafx.h"

#include "GameObject.h"

#define CLIENT_WIDTH	640
#define CLIENT_HEIGHT	480

class CGameFramework
{
public:
	CGameFramework(void);
	~CGameFramework(void);

	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	void OnDestroy();
	void FrameAdvance();

	void SetActive(bool bActive) { m_bActive = bActive; }

private:
	HINSTANCE					m_hInstance;
	HWND						m_hWnd; 

    bool						m_bActive;          

	HDC							m_hDCFrameBuffer;   
    HBITMAP						m_hBitmapFrameBuffer;   

	CPlayer						*m_pPlayer;

	int							m_nObjects;
	CGameObject					*m_pObjects;

	CGameObject					*m_pWall;
	XMFLOAT4					m_pxmf4WallPlanes[6];

public:
	void BuildFrameBuffer();
	void ClearFrameBuffer(DWORD dwColor);
	void PresentFrameBuffer();

	void BuildObjects();
	void ReleaseObjects();
	void SetupGameState();
	void SetupRenderStates();
	void AnimateObjects();
	void ProcessInput();

	void DrawObject(CGameObject *pObject, XMFLOAT4X4& mtxViewProject);
	void DrawPrimitive(CPolygon *pPolygon, XMFLOAT4X4& xm4x4Transform);

	POINT						m_ptOldCursorPos;    
};

#endif 

