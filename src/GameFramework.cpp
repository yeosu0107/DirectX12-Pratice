//-----------------------------------------------------------------------------
// File: CGameFramework.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "GameFramework.h"

CGameFramework::CGameFramework()
{
	m_hInstance = NULL;
	m_hWnd = NULL;     

    m_hDCFrameBuffer = NULL;   
    m_hBitmapFrameBuffer = NULL;   

	m_bActive = true;    

	m_nObjects = 0;
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
	CAirplaneMesh *pAirplaneMesh = new CAirplaneMesh(6.0f, 6.0f, 1.0f);
	m_pPlayer = new CPlayer();
	m_pPlayer->SetPosition(0.0f, 0.0f, -30.0f);
	m_pPlayer->SetMesh(pAirplaneMesh);
	m_pPlayer->SetColor(RGB(0, 0, 255));
	m_pPlayer->SetCameraOffset(XMFLOAT3(0.0f, 5.0f, -15.0f));

	CWallMesh *pWallCubeMesh = new CWallMesh(40.0f, 40.0f, 40.0f);
	m_pWall = new CGameObject();
	m_pWall->SetPosition(0.0f, 0.0f, 0.0f);
	m_pWall->SetMesh(pWallCubeMesh);
	m_pWall->SetColor(RGB(0, 0, 0));
	m_pWall->SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(20.0f, 20.0f, 20.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	m_pxmf4WallPlanes[0] = XMFLOAT4(+1.0f, 0.0f, 0.0f, 20.0f);
	m_pxmf4WallPlanes[1] = XMFLOAT4(-1.0f, 0.0f, 0.0f, 20.0f);
	m_pxmf4WallPlanes[2] = XMFLOAT4(0.0f, +1.0f, 0.0f, 20.0f);
	m_pxmf4WallPlanes[3] = XMFLOAT4(0.0f, -1.0f, 0.0f, 20.0f);
	m_pxmf4WallPlanes[4] = XMFLOAT4(0.0f, 0.0f, +1.0f, 20.0f);
	m_pxmf4WallPlanes[5] = XMFLOAT4(0.0f, 0.0f, -1.0f, 20.0f);

	CCubeMesh *pObjectCubeMesh = new CCubeMesh(4.0f, 4.0f, 4.0f);

	m_nObjects = 8;
	m_pObjects = new CGameObject[m_nObjects];
    m_pObjects[0].SetMesh(pObjectCubeMesh);
	m_pObjects[0].SetColor(RGB(255, 0, 0));
	m_pObjects[0].SetPosition(-13.5f, 0.0f, -14.0f);
	m_pObjects[0].SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 1.0f));
	m_pObjects[0].SetRotationSpeed(0.05f);
	m_pObjects[0].SetMovingDirection(XMFLOAT3(1.0f, 0.0f, 0.0f));
	m_pObjects[0].SetMovingSpeed(0.001f);
	m_pObjects[0].SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

    m_pObjects[1].SetMesh(pObjectCubeMesh);
	m_pObjects[1].SetColor(RGB(0, 0, 255));
	m_pObjects[1].SetPosition(+13.5f, 0.0f, -14.0f);
	m_pObjects[1].SetRotationAxis(XMFLOAT3(1.0f, 1.0f, 0.0f));
	m_pObjects[1].SetRotationSpeed(0.07f);
	m_pObjects[1].SetMovingDirection(XMFLOAT3(-1.0f, 0.0f, 0.0f));
	m_pObjects[1].SetMovingSpeed(0.001f);
	m_pObjects[1].SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	m_pObjects[2].SetMesh(pObjectCubeMesh);
	m_pObjects[2].SetColor(RGB(0, 255, 0));
	m_pObjects[2].SetPosition(0.0f, +5.0f, 20.0f);
	m_pObjects[2].SetRotationAxis(XMFLOAT3(1.0f, 1.0f, 0.0f));
	m_pObjects[2].SetRotationSpeed(0.15f);
	m_pObjects[2].SetMovingDirection(XMFLOAT3(1.0f, -1.0f, 0.0f));
	m_pObjects[2].SetMovingSpeed(0.02f);
	m_pObjects[2].SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	m_pObjects[3].SetMesh(pObjectCubeMesh);
	m_pObjects[3].SetColor(RGB(0, 255, 255));
	m_pObjects[3].SetPosition(0.0f, 0.0f, 0.0f);
	m_pObjects[3].SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 1.0f));
	m_pObjects[3].SetRotationSpeed(0.06f);
	m_pObjects[3].SetMovingDirection(XMFLOAT3(0.0f, 0.0f, 1.0f));
	m_pObjects[3].SetMovingSpeed(0.004f);
	m_pObjects[3].SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	m_pObjects[4].SetMesh(pObjectCubeMesh);
	m_pObjects[4].SetColor(RGB(128, 0, 255));
	m_pObjects[4].SetPosition(10.0f, 0.0f, 0.0f);
	m_pObjects[4].SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	m_pObjects[4].SetRotationSpeed(0.06f);
	m_pObjects[4].SetMovingDirection(XMFLOAT3(0.0f, 1.0f, 1.0f));
	m_pObjects[4].SetMovingSpeed(0.004f);
	m_pObjects[4].SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	m_pObjects[5].SetMesh(pObjectCubeMesh);
	m_pObjects[5].SetColor(RGB(255, 0, 255));
	m_pObjects[5].SetPosition(-10.0f, 0.0f, -10.0f);
	m_pObjects[5].SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	m_pObjects[5].SetRotationSpeed(0.06f);
	m_pObjects[5].SetMovingDirection(XMFLOAT3(1.0f, 0.0f, 1.0f));
	m_pObjects[5].SetMovingSpeed(0.004f);
	m_pObjects[5].SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	m_pObjects[6].SetMesh(pObjectCubeMesh);
	m_pObjects[6].SetColor(RGB(255, 0, 255));
	m_pObjects[6].SetPosition(-10.0f, 10.0f, -10.0f);
	m_pObjects[6].SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	m_pObjects[6].SetRotationSpeed(0.06f);
	m_pObjects[6].SetMovingDirection(XMFLOAT3(1.0f, 1.0f, 1.0f));
	m_pObjects[6].SetMovingSpeed(0.004f);
	m_pObjects[6].SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	m_pObjects[7].SetMesh(pObjectCubeMesh);
	m_pObjects[7].SetColor(RGB(255, 0, 128));
	m_pObjects[7].SetPosition(-10.0f, 10.0f, -20.0f);
	m_pObjects[7].SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	m_pObjects[7].SetRotationSpeed(0.06f);
	m_pObjects[7].SetMovingDirection(XMFLOAT3(-1.0f, 1.0f, 1.0f));
	m_pObjects[7].SetMovingSpeed(0.004f);
	m_pObjects[7].SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
}

void CGameFramework::ReleaseObjects()
{
	if (m_pObjects) delete [] m_pObjects;
	m_pObjects = NULL;
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
		if (pKeyBuffer[VK_UP] & 0xF0) dwDirection |= DIR_FORWARD;
		if (pKeyBuffer[VK_DOWN] & 0xF0) dwDirection |= DIR_BACKWARD;
		if (pKeyBuffer[VK_LEFT] & 0xF0) dwDirection |= DIR_LEFT;
		if (pKeyBuffer[VK_RIGHT] & 0xF0) dwDirection |= DIR_RIGHT;
		if (pKeyBuffer[VK_PRIOR] & 0xF0) dwDirection |= DIR_UP;
		if (pKeyBuffer[VK_NEXT] & 0xF0) dwDirection |= DIR_DOWN;
	}
	float cxDelta = 0.0f, cyDelta = 0.0f;
	POINT ptCursorPos;
	if (GetCapture() == m_hWnd)
	{
		SetCursor(NULL);
		GetCursorPos(&ptCursorPos);
		cxDelta = (float)(ptCursorPos.x - m_ptOldCursorPos.x) / 3.0f;
		cyDelta = (float)(ptCursorPos.y - m_ptOldCursorPos.y) / 3.0f;
		SetCursorPos(m_ptOldCursorPos.x, m_ptOldCursorPos.y);
	}
	if ((dwDirection != 0) || (cxDelta != 0.0f) || (cyDelta != 0.0f))
	{
		if (cxDelta || cyDelta)
		{
			if (pKeyBuffer[VK_RBUTTON] & 0xF0)
				m_pPlayer->Rotate(cyDelta, 0.0f, -cxDelta);
			else
				m_pPlayer->Rotate(cyDelta, cxDelta, 0.0f);
		}
		if (dwDirection) m_pPlayer->Move(dwDirection, 0.025f);
	}
	m_pPlayer->Update(0.00516f);
}

void CGameFramework::AnimateObjects()
{
	for (int i = 0; i < m_nObjects; i++) m_pObjects[i].Animate();

	for (int i = 0; i < m_nObjects; i++)
	{
		ContainmentType containType = m_pWall->m_xmOOBB.Contains(m_pObjects[i].m_xmOOBB);
		switch (containType)
		{
			case DISJOINT:
			{
				int nPlaneIndex = -1;
				for (int j = 0; j < 6; j++)
				{
					PlaneIntersectionType intersectType = m_pObjects[i].m_xmOOBB.Intersects(XMLoadFloat4(&m_pxmf4WallPlanes[j]));
					if (intersectType == BACK)
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
				for (int j = 0; j < 6; j++)
				{
					PlaneIntersectionType intersectType = m_pObjects[i].m_xmOOBB.Intersects(XMLoadFloat4(&m_pxmf4WallPlanes[j]));
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
	for (int i = 0; i < m_nObjects; i++) m_pObjects[i].m_pCollider = NULL;
	for (int i = 0; i < m_nObjects; i++)
	{
		for (int j = (i + 1); j < m_nObjects; j++)
		{
			if (m_pObjects[i].m_xmOOBB.Intersects(m_pObjects[j].m_xmOOBB))
			{
				m_pObjects[i].m_pCollider = &m_pObjects[j];
				m_pObjects[j].m_pCollider = &m_pObjects[i];
			}
		}
	}
	for (int i = 0; i < m_nObjects; i++)
	{
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
}

void CGameFramework::FrameAdvance()
{    
    if (!m_bActive) return;

	ProcessInput();

	AnimateObjects();

    ClearFrameBuffer(RGB(255, 255, 255));

	m_pWall->Render(m_hDCFrameBuffer, m_pPlayer->m_pCamera);

	for (int i = 0; i < m_nObjects; i++) m_pObjects[i].Render(m_hDCFrameBuffer, m_pPlayer->m_pCamera);

	m_pPlayer->Render(m_hDCFrameBuffer, m_pPlayer->m_pCamera);

	PresentFrameBuffer();
}


