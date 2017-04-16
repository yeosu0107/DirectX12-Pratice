#pragma once

//-----------------------------------------------------------------------------
// File: GameFramework.h
//-----------------------------------------------------------------------------

#ifndef _GAME_APPLICATION_FRAMEWORK_H_
#define _GAME_APPLICATION_FRAMEWORK_H_

#include "stdafx.h"
#include "GameObject.h"
#include "Player.h"
#include "EnemyObject.h"
#include "Map.h"
#include "Timer.h"


#define CLIENT_WIDTH	640
#define CLIENT_HEIGHT	480


class CGameFramework
{
public:
	CGameFramework(void);
	~CGameFramework(void);

	//게임 생성
	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	//게임 종료(파괴)
	void OnDestroy();
	//디바이스 인풋, 오브젝트 애니메이트, 랜더, 더블버퍼링
	void FrameAdvance();

	//게임 상태
	void SetActive(bool bActive) { m_bActive = bActive; }

private:
	HINSTANCE					m_hInstance;
	HWND						m_hWnd; 

    bool						m_bActive;          

	HDC							m_hDCFrameBuffer;   
    HBITMAP						m_hBitmapFrameBuffer;   

	CPlayer						*m_pPlayer;

	int							m_nObjects;  //오브젝트 갯수
	EnemyCube					*m_pObjects;

	int							m_nWall;
	Wall						*m_pWall;
	XMFLOAT4					m_pxmf4WallPlanes[6];

	CGameTimer					m_GameTimer;
	_TCHAR						m_pszFrameRate[50];

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

