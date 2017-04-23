//-----------------------------------------------------------------------------
// File: CGameFramework.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "GameFramework.h"
#include "Map.h"
#include "Player.h"

#include "EnemyObject.h"
#include "Item.h"
#include <stdio.h>
#include <random>

std::default_random_engine itemEngine;
std::uniform_int_distribution<> itemSet(0, 10);

enum gameStatus {main=0, Gameloop, die};



CGameFramework::CGameFramework()
{
	m_gameStatus = gameStatus::main;

	m_hInstance = NULL;
	m_hWnd = NULL;     

    m_hDCFrameBuffer = NULL;   
    m_hBitmapFrameBuffer = NULL;   

	m_bActive = true;    

	m_nObjects = 0;

	score = new Score();
	ui = new UI();

	_tcscpy_s(m_pszFrameRate, _T("LapProject (")); 

}

CGameFramework::~CGameFramework()
{
	
}

bool CGameFramework::OnCreate(HINSTANCE hInstance, HWND hMainWnd)
{
    srand(timeGetTime());

	m_hInstance = hInstance;
	m_hWnd = hMainWnd;

	RECT rcClient;
	GetClientRect(m_hWnd, &rcClient);

	m_pPlayer = new CPlayer();
	m_pPlayer->m_pCamera->SetViewport(rcClient.left, rcClient.top, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);

	BuildFrameBuffer(); 

	BuildObjects(); 

	SetupGameState();

	return(true);
}

void CGameFramework::BuildFrameBuffer()
{
    HDC hDC = ::GetDC(m_hWnd);

	RECT rcClient;
	GetClientRect(m_hWnd, &rcClient);

    m_hDCFrameBuffer = ::CreateCompatibleDC(hDC);
	m_hBitmapFrameBuffer = ::CreateCompatibleBitmap(hDC, (rcClient.right - rcClient.left) + 1, (rcClient.bottom - rcClient.top) + 1);
    ::SelectObject(m_hDCFrameBuffer, m_hBitmapFrameBuffer);

	::ReleaseDC(m_hWnd, hDC);
    ::SetBkMode(m_hDCFrameBuffer, TRANSPARENT);
}

void CGameFramework::ClearFrameBuffer(DWORD dwColor)
{
    HBRUSH hBrush = ::CreateSolidBrush(dwColor);
    HBRUSH hOldBrush = (HBRUSH)::SelectObject(m_hDCFrameBuffer, hBrush);
	::Rectangle(m_hDCFrameBuffer, m_pPlayer->m_pCamera->m_Viewport.m_xStart, m_pPlayer->m_pCamera->m_Viewport.m_yStart, m_pPlayer->m_pCamera->m_Viewport.m_nWidth, m_pPlayer->m_pCamera->m_Viewport.m_nHeight);
    ::SelectObject(m_hDCFrameBuffer, hOldBrush);
    ::DeleteObject(hBrush);
}

void CGameFramework::PresentFrameBuffer()
{    
    HDC hDC = ::GetDC(m_hWnd);
    ::BitBlt(hDC, m_pPlayer->m_pCamera->m_Viewport.m_xStart, m_pPlayer->m_pCamera->m_Viewport.m_yStart, m_pPlayer->m_pCamera->m_Viewport.m_nWidth, m_pPlayer->m_pCamera->m_Viewport.m_nHeight, m_hDCFrameBuffer, m_pPlayer->m_pCamera->m_Viewport.m_xStart, m_pPlayer->m_pCamera->m_Viewport.m_yStart, SRCCOPY);
    ::ReleaseDC(m_hWnd, hDC);
}

void CGameFramework::SetupGameState()
{
}

void CGameFramework::SetupRenderStates()
{
}

void CGameFramework::BuildObjects()
{
	score->ResetScore();

	CAirplaneMesh *pAirplaneMesh = new CAirplaneMesh(6.0f, 6.0f, 1.0f);
	m_pPlayer = new CPlayer();
	m_pPlayer->SetPosition(0.0f, 0.0f, 0.0f);
	m_pPlayer->SetMesh(pAirplaneMesh);
	m_pPlayer->SetColor(RGB(0, 0, 255));
	m_pPlayer->SetCameraOffset(XMFLOAT3(0.0f, 5.0f, -15.0f));
	m_pPlayer->SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(3.0f, 3.0f, 0.5f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	m_nWall = 5;
	m_pWall = new Wall[m_nWall];

	for (int i = 0; i < m_nWall; ++i) {
		m_pWall[i].SetIndex(i);
		m_pWall[i].SetPosition(0, 0, (float)i * 60);
		m_pWall[i].SetTile((float)i * 60);
		m_pWall[i].SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(25.0f, 15.0f, 100.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}


	m_pxmf4WallPlanes[0] = XMFLOAT4(+1.0f, 0.0f, 0.0f, 25.0f);
	m_pxmf4WallPlanes[1] = XMFLOAT4(-1.0f, 0.0f, 0.0f, 25.0f);
	m_pxmf4WallPlanes[2] = XMFLOAT4(0.0f, +1.0f, 0.0f, 15.0f);
	m_pxmf4WallPlanes[3] = XMFLOAT4(0.0f, -1.0f, 0.0f, 15.0f);
	//m_pxmf4WallPlanes[4] = XMFLOAT4(0.0f, 0.0f, +1.0f, 100.0f);
	//m_pxmf4WallPlanes[5] = XMFLOAT4(0.0f, 0.0f, -1.0f, 100.0f);


	m_nObjects = 10;
	m_pObjects = new EnemyCube[m_nObjects];
	m_pItem = new Item[m_nObjects];
	CCubeMesh *baseCubeMesh = new CCubeMesh(4.0f, 4.0f, 4.0f);
	CCubeMesh* itemMesh = new CCubeMesh(3.0, 3.0, 3.0);
	DWORD tmp2[4] = { RGB(255,255,0), RGB(0,255, 255), RGB(255,0,255), RGB(100,200,30) };

	for (int i = 0; i < m_nObjects; ++i) {
		m_pObjects[i].setCube(m_pPlayer->GetPosition().z+20.0f, 1.0f, 0.1f);
		m_pObjects[i].m_pCollider = NULL;
		m_pObjects[i].SetMesh(baseCubeMesh);
		m_pItem[i].SetMesh(itemMesh);
	}
}

void CGameFramework::ReleaseObjects()
{
	if (m_pObjects) delete[] m_pObjects;
	if (m_pItem) delete[] m_pItem;
	if (score) delete score;
	if (ui) delete ui;
	if (m_pWall) delete[] m_pWall;

	if (m_pPlayer) delete m_pPlayer;


	m_pObjects = nullptr;
	m_pWall = nullptr;
	m_pPlayer = nullptr;
	m_pItem = nullptr;
	score = nullptr;
	ui = nullptr;
}

void CGameFramework::OnDestroy()
{
	ReleaseObjects();

	if (m_hBitmapFrameBuffer) ::DeleteObject(m_hBitmapFrameBuffer);
    if (m_hDCFrameBuffer) ::DeleteDC(m_hDCFrameBuffer);

    if (m_hWnd) DestroyWindow(m_hWnd);
}

void CGameFramework::ProcessInput()
{
	static UCHAR pKeyBuffer[256];
	DWORD dwDirection = 0;
	if (GetKeyboardState(pKeyBuffer))
	{
		if (pKeyBuffer[VK_W] & 0xF0) dwDirection |= DIR_FORWARD;
		//if (pKeyBuffer[VK_S] & 0xF0) dwDirection |= DIR_BACKWARD;
		if (pKeyBuffer[VK_A] & 0xF0) dwDirection |= DIR_LEFT;
		if (pKeyBuffer[VK_D] & 0xF0) dwDirection |= DIR_RIGHT;
		if (pKeyBuffer[VK_PRIOR] & 0xF0) dwDirection |= DIR_UP;
		if (pKeyBuffer[VK_NEXT] & 0xF0) dwDirection |= DIR_DOWN;
		if (pKeyBuffer[VK_SPACE] & 0xF0)
			m_pPlayer->Boost();
		else
			m_pPlayer->endBoost();
		
		if (pKeyBuffer[VK_RETURN] & 0xF0) {
			if (m_gameStatus == gameStatus::main) {
				m_gameStatus = gameStatus::Gameloop;

			}
		}
		if (pKeyBuffer[VK_BACK] & 0xF0) {
			if (m_gameStatus == gameStatus::die) {
				BuildObjects();
				m_gameStatus = gameStatus::main;

			}
		}
		if (pKeyBuffer[VK_ESCAPE] & 0xF0) {
			PostQuitMessage(0);
			//ExitProcess(0);
		}

	}
	if (!m_pPlayer->getLive() || m_gameStatus!=gameStatus::Gameloop)
		return;

	float cxDelta = 0.0f, cyDelta = 0.0f;
	POINT ptCursorPos;
	if (GetCapture() == m_hWnd)
	{
		SetCursor(NULL);
		GetCursorPos(&ptCursorPos);
		cxDelta = (float)(ptCursorPos.x - m_ptOldCursorPos.x) / 5.0f;
		cyDelta = (float)(ptCursorPos.y - m_ptOldCursorPos.y) / 5.0f;
		SetCursorPos(m_ptOldCursorPos.x, m_ptOldCursorPos.y);
	}
	if(pKeyBuffer[VK_LBUTTON] & 0xF0)
		m_pPlayer->Shoot();
	if ((dwDirection != 0) || (cxDelta != 0.0f) || (cyDelta != 0.0f))
	{
		if (cxDelta || cyDelta)
		{
			if (pKeyBuffer[VK_RBUTTON] & 0xF0) {
					m_pPlayer->Rotate(cyDelta, 0.0f, -cxDelta);
			}
			else {
				m_pPlayer->Rotate(cyDelta, cxDelta, 0.0f);
			}
		}
		if (m_pPlayer->getLive() && dwDirection) {
			score->moveForward(m_pPlayer->getSpeed());
			m_pPlayer->Move(dwDirection);
		}
	}
	m_pPlayer->Update(0.516f);
}

void CGameFramework::AnimateObjects()
{
	// ------------- 애니메이션 부분 ----------------------
	m_pPlayer->Animate();
	for (int i = 0; i < m_nObjects; i++) {
		/*if(m_pObjects[i].getLive())*/
		m_pObjects[i].Animate(m_pPlayer->GetPosition(), (score->getDist()/1000.0f), (score->getDist() / 1000.0f));
	}
	for (int i = 0; i < m_nObjects; ++i) {
		if (m_pItem[i].getLive())
			m_pItem[i].Animate(m_pPlayer->GetPosition());
	}

	for (int i = 0; i < m_nWall; ++i)
		m_pWall[i].Animate(m_pPlayer->GetPosition());
	////////////////////////////////////////////////////////

	// ------------- 충돌체크 부분 ------------------------

	/*for (int wallIndex = 0; wallIndex < m_nWall; ++wallIndex) {
		if (!m_pWall[wallIndex].getLive())
			continue;
		for (int enemyIndex = 0; enemyIndex < m_nObjects; ++enemyIndex) {
			if (!m_pObjects[enemyIndex].getLive())
				continue;
			ContainmentType containType = m_pWall[wallIndex].m_xmOOBB.Contains(m_pObjects[enemyIndex].m_xmOOBB);
			if (containType == INTERSECTS) {
				Tile* tmpTile = m_pWall[wallIndex].GetTile();
				for (int tile = 0; tile < 48; ++tile) {
					ContainmentType tileType = tmpTile[tile].m_xmOOBB.Contains(m_pObjects[enemyIndex].m_xmOOBB);
					if (tileType == INTERSECTS) {
						printf("%d  %d\n", enemyIndex, tile);
						tmpTile[tile].SetColor(m_pObjects[enemyIndex].getColor());
						break;
					}
				}
			}
		}
	}*/

	for (int index = 0; index < m_nWall; ++index) {
		for (int i = 0; i < m_nObjects; i++)
		{
			ContainmentType containType = m_pWall[index].getOOBB()->Contains(*m_pObjects[i].getOOBB());
			switch (containType)
			{
			case DISJOINT:
			{
				int nPlaneIndex = -1;
				for (int j = 0; j < 4; j++)
				{
					PlaneIntersectionType intersectType = m_pObjects[i].getOOBB()->Intersects(XMLoadFloat4(&m_pxmf4WallPlanes[j]));
					if (intersectType == INTERSECTING ||intersectType == BACK)
					{
						nPlaneIndex = j;
						break;
					}
				}
				if (nPlaneIndex != -1)
				{
					XMVECTOR xmvNormal = XMVectorSet(m_pxmf4WallPlanes[nPlaneIndex].x, m_pxmf4WallPlanes[nPlaneIndex].y, m_pxmf4WallPlanes[nPlaneIndex].z, 0.0f);
					XMVECTOR xmvReflect = XMVector3Reflect(XMLoadFloat3(&m_pObjects[i].m_xmf3MovingDirection), xmvNormal);
					XMStoreFloat3(&m_pObjects[i].m_xmf3MovingDirection, xmvReflect);
				}
				break;
			}
			case INTERSECTS:
			{
				int nPlaneIndex = -1;
				for (int j = 0; j < 4; j++)
				{
					PlaneIntersectionType intersectType = m_pObjects[i].getOOBB()->Intersects(XMLoadFloat4(&m_pxmf4WallPlanes[j]));
					if (intersectType == INTERSECTING)
					{
						nPlaneIndex = j;
						break;
					}
				}
				if (nPlaneIndex != -1)
				{
					XMVECTOR xmvNormal = XMVectorSet(m_pxmf4WallPlanes[nPlaneIndex].x, m_pxmf4WallPlanes[nPlaneIndex].y, m_pxmf4WallPlanes[nPlaneIndex].z, 0.0f);
					XMVECTOR xmvReflect = XMVector3Reflect(XMLoadFloat3(&m_pObjects[i].m_xmf3MovingDirection), xmvNormal);
					XMStoreFloat3(&m_pObjects[i].m_xmf3MovingDirection, xmvReflect);
				}
				break;
			}
			case CONTAINS:
				break;
			}
		}

		ContainmentType containType = m_pWall[index].getOOBB()->Contains(*m_pPlayer->getOOBB());
		/*if (containType == INTERSECTS || containType == DISJOINT) {
			m_pPlayer->OnDestroy();
			m_gameStatus = gameStatus::die;
		}*/

		switch (containType)
		{
		case DISJOINT:
		{
			int nPlaneIndex = -1;
			for (int j = 0; j < 4; j++)
			{
				PlaneIntersectionType intersectType = m_pPlayer->getOOBB()->Intersects(XMLoadFloat4(&m_pxmf4WallPlanes[j]));
				if (intersectType == BACK || intersectType == INTERSECTING)
				{
					nPlaneIndex = j;
					break;
				}
			}
			if (nPlaneIndex != -1)
			{
				//m_pPlayer->SetPosition()
				//m_pPlayer->MoveBack(DIR_FORWARD);
				m_pPlayer->OnDestroy();
				m_gameStatus = gameStatus::die;
			}
			break;
		}
		case INTERSECTS:
		{
			int nPlaneIndex = -1;
			for (int j = 0; j < 4; j++)
			{
				PlaneIntersectionType intersectType = m_pPlayer->getOOBB()->Intersects(XMLoadFloat4(&m_pxmf4WallPlanes[j]));
				if (intersectType == INTERSECTING || intersectType == BACK)
				{
					nPlaneIndex = j;
					break;
				}
			}
			if (nPlaneIndex != -1)
			{
				//m_pPlayer->MoveBack(DIR_FORWARD);
				m_pPlayer->OnDestroy();
				m_gameStatus = gameStatus::die;
			}
			break;
		}
		case CONTAINS:
			int nPlaneIndex = -1;
			for (int j = 0; j < 4; j++)
			{
				PlaneIntersectionType intersectType = m_pPlayer->getOOBB()->Intersects(XMLoadFloat4(&m_pxmf4WallPlanes[j]));
				if (intersectType == BACK || intersectType == INTERSECTING)
				{
					nPlaneIndex = j;
					break;
				}
			}
			if (nPlaneIndex != -1)
			{
				//m_pPlayer->SetPosition()
				//m_pPlayer->MoveBack(DIR_FORWARD);
				m_pPlayer->OnDestroy();
				m_gameStatus = gameStatus::die;
			}
			break;
			break;
		}
	}

	if (m_gameStatus!=gameStatus::Gameloop)
		return;

	//오브젝트 충돌 정의
	for (int i = 0; i < m_nObjects; i++)
	{
		if (!m_pObjects[i].getLive())
			continue;
		//오브젝트끼리 충돌
		for (int j = (i + 1); j < m_nObjects; j++)
		{
			/*if (i == j)
				continue;*/

			if (m_pObjects[i].getOOBB()->Intersects(*m_pObjects[j].getOOBB()))
			{
				m_pObjects[i].m_pCollider = &m_pObjects[j];
				m_pObjects[j].m_pCollider = &m_pObjects[i];
			}
		}

		//총알과 충돌
		for (int k = 0; k < 1000; ++k) {
			if (!m_pPlayer->GetBullet()[k].GetLive() || !m_pPlayer->getLive())
				continue;
			if (m_pObjects[i].getCubeLive() && m_pObjects[i].getOOBB()->Intersects(*m_pPlayer->GetBullet()[k].getOOBB())) {
				//오브젝트 파괴
				m_pObjects[i].DestroyObject();
				m_pPlayer->GetBullet()[k].setLive(false);
				score->killEnemy();

				//아이템 생성 (3/11 확률)
				for (int itemIndex = 0; itemIndex < m_nObjects; ++itemIndex) {
					if (m_pItem[itemIndex].getLive())
						continue;
					else {
						m_pItem[itemIndex].setLive(m_pItem[itemIndex].setType(itemSet(itemEngine), m_pObjects[i].GetPosition()));
						break;
					}
				}
				break;
			}
		}

		//플레이어와 충돌
		if (m_pObjects[i].getCubeLive() && m_pPlayer->getLive() &&
			m_pObjects[i].getOOBB()->Intersects(*m_pPlayer->getOOBB())) {
			//printf("Crash!\n");
			//m_start = false;
			m_pPlayer->OnDestroy();
			m_gameStatus = gameStatus::die;
			m_pObjects[i].DestroyObject();
		}
	}
	for (int i = 0; i < m_nObjects; i++)
	{
		if (!m_pObjects[i].getLive())
			continue;
		if (m_pObjects[i].m_pCollider && m_pObjects[i].m_pCollider->m_pCollider)
		{
			XMFLOAT3 xmf3MovingDirection = m_pObjects[i].m_xmf3MovingDirection;
			float fMovingSpeed = m_pObjects[i].m_fMovingSpeed;
			m_pObjects[i].m_xmf3MovingDirection = m_pObjects[i].m_pCollider->m_xmf3MovingDirection;
			m_pObjects[i].m_fMovingSpeed = m_pObjects[i].m_pCollider->m_fMovingSpeed;
			m_pObjects[i].m_pCollider->m_xmf3MovingDirection = xmf3MovingDirection;
			m_pObjects[i].m_pCollider->m_fMovingSpeed = fMovingSpeed;
			m_pObjects[i].m_pCollider->m_pCollider = NULL;
			m_pObjects[i].m_pCollider = NULL;
		}
	}
	//플레이어 아이템과 충돌 체크
	for (int i = 0; i < m_nObjects; i++)
	{
		if (!m_pItem[i].getLive())
			continue;
		if (m_pPlayer->getLive() && m_pItem[i].getOOBB()->Intersects(*m_pPlayer->getOOBB())) {
			m_pPlayer->ItemChecker(m_pItem[i].getType());
			m_pItem[i].setLive(false);
		}
	}
}

void CGameFramework::FrameAdvance()
{    
    if (!m_bActive) return;
	m_GameTimer.Tick(60.0f);
	ProcessInput();


	AnimateObjects();

    ClearFrameBuffer(RGB(255, 255, 255));

	for (int i = 0; i < m_nWall; ++i) {
		if(m_pWall[i].getLive())
			m_pWall[i].Render(m_hDCFrameBuffer, m_pPlayer->m_pCamera);
	}

	for (int i = 0; i < m_nObjects; i++) {
		if(m_pObjects[i].getLive())
			m_pObjects[i].Render(m_hDCFrameBuffer, m_pPlayer->m_pCamera);
		if(m_pItem[i].getLive())
			m_pItem[i].Render(m_hDCFrameBuffer, m_pPlayer->m_pCamera);
		//printf("%d", m_pItem[i].getLive());
	}

	m_pPlayer->Render(m_hDCFrameBuffer, m_pPlayer->m_pCamera);

	ui->DrawUI(m_hDCFrameBuffer, m_gameStatus, score->setScore(), m_pPlayer->getBoost(), m_pPlayer->getBulletSpeed());


	PresentFrameBuffer();
	


	//프레임레이트 표현
	_itow_s(m_GameTimer.GetFrameRate(), (m_pszFrameRate + 12), 37, 10);    
	wcscat_s((m_pszFrameRate + 12), 37, _T(" FPS)"));
	m_GameTimer.GetFrameRate(m_pszFrameRate + 12, 37); ::SetWindowText(m_hWnd, m_pszFrameRate);
}

UI::UI() {
	wsprintf(m_gameover, L"Game Over");
	wsprintf(m_restart, L"Press BackSpace to Restart");
	wsprintf(m_start, L"Press Enter to Start");
}

UI::~UI() {

}

//RECT winRect = { 0,0,CLIENT_WIDTH, CLIENT_HEIGHT };

void UI::DrawUI(HDC hdc, int status, int score, int boost, int bullet) {
	switch (status) {
	case gameStatus::main:
		SetBkColor(hdc, RGB(255, 255, 0));
		TextOut(hdc, CLIENT_WIDTH / 2 - 60, CLIENT_HEIGHT / 2 - 20, m_start, lstrlen(m_start));
		break;
	case gameStatus::Gameloop:
		wsprintf(m_boostGauag, L"BoostGauge : %d", boost);
		wsprintf(m_score, L"Score : %d", score);
		//wsprintf(m_speed, L"now Speed : %d", (int)speed);
		wsprintf(m_bulletspeed, L"Bullet Speed : %d", 11-bullet);
		TextOut(hdc, 10, 10, m_score, lstrlen(m_score));
		TextOut(hdc, 10, 30, m_boostGauag, lstrlen(m_boostGauag));
		//TextOut(hdc, 10, 50, m_speed, lstrlen(m_speed));
		TextOut(hdc, 10, 50, m_bulletspeed, lstrlen(m_bulletspeed));
		break;
	case gameStatus::die:
		wsprintf(m_score, L"Your Score : %d", score);
		TextOut(hdc, CLIENT_WIDTH / 2 - 30, CLIENT_HEIGHT / 2 - 40, m_gameover, lstrlen(m_gameover));
		TextOut(hdc, CLIENT_WIDTH / 2 - 35, CLIENT_HEIGHT / 2 - 20, m_score, lstrlen(m_score));
		TextOut(hdc, CLIENT_WIDTH / 2 - 80, CLIENT_HEIGHT / 2, m_restart, lstrlen(m_restart));
		break;
	}
}
