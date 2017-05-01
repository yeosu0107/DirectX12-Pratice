#include "stdafx.h"
#include "Player.h"
#include "GameObject.h"
#include "Bullet.h"
#include <random>


using namespace Vector3;
CPlayer::CPlayer() : maxBulletCount{ 1000 }, bulletDelay{ 10 }, maxBulletDelay{ 10 },
numOfPaticle{ 100 }, paticleLiveTime{ 0 }, maxpaicleLiveTime{ 50 }, Live{ true }, 
speed{ 0.5f }, backupSpeed{ speed }, boostSpeed{ speed * 3 }, boostGauge{ 100 }, maxboostGauge{ 100 }
{
	m_pCamera = new CCamera();
	m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, 60.0f);

	m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);

	m_xmf3CameraOffset = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_fFriction = 125.0f;

	m_fPitch = 0.0f;
	m_fRoll = 0.0f;
	m_fYaw = 0.0f;

	m_pBullet = new Bullet[maxBulletCount];
	CCubeMesh *bulletMesh = new CCubeMesh(0.5, 0.5, 0.5);
	for (int i = 0; i < maxBulletCount; ++i)
		m_pBullet[i].SetMesh(bulletMesh);

	bulletIndex = 0;

	paticle = new Paticle[numOfPaticle];
	CCubeMesh *paticleMesh = new CCubeMesh(0.5f, 0.5f, 0.5f);
	std::default_random_engine dre;
	std::uniform_int_distribution<> ui(-10, 10);

	for (int i = 0; i < numOfPaticle; ++i) {
		paticle[i].SetMesh(paticleMesh);
		paticle[i].SetColor(getColor());
		paticle[i].SetRotationAxis(XMFLOAT3(1.0f, 1.0f, 1.0f));
		paticle[i].SetRotationSpeed(0.05f);
		paticle[i].SetMovingDirection(XMFLOAT3(ui(dre), ui(dre), ui(dre)));
		paticle[i].SetMovingSpeed(1.0f);
	}
}

CPlayer::~CPlayer()
{
	delete[] m_pBullet;
	delete[] paticle;
	delete m_pCamera;

	m_pBullet = nullptr;
	paticle = nullptr;
	m_pCamera = nullptr;
	//CGameObject::~CGameObject();
}

void CPlayer::SetPosition(float x, float y, float z)
{
	m_xmf3Position = XMFLOAT3(x, y, z);

	CGameObject::SetPosition(x, y, z);
}

void CPlayer::SetCameraOffset(XMFLOAT3& xmf3CameraOffset)
{
	m_xmf3CameraOffset = xmf3CameraOffset;
	m_pCamera->SetLookAt(Vector3::Add(m_xmf3Position, m_xmf3CameraOffset), m_xmf3Position, m_xmf3Up);
	m_pCamera->GenerateViewMatrix();
}

void CPlayer::Move(DWORD dwDirection)
{
	if (dwDirection)
	{
		XMFLOAT3 xmf3Shift = XMFLOAT3(0, 0, 0);
		if (dwDirection & DIR_FORWARD) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, speed);
		if (dwDirection & DIR_BACKWARD) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, -speed);
		if (dwDirection & DIR_RIGHT) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, speed);
		if (dwDirection & DIR_LEFT) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, -speed);
		if (dwDirection & DIR_UP) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, speed);
		if (dwDirection & DIR_DOWN) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, -speed);

		Move(xmf3Shift, true);
	}
}

void CPlayer::Move(XMFLOAT3& xmf3Shift, bool bUpdateVelocity)
{
	if (bUpdateVelocity)
	{
		m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, xmf3Shift);
	}
	else
	{
		m_xmf3Position = Vector3::Add(xmf3Shift, m_xmf3Position);
		m_pCamera->Move(xmf3Shift);
	}
}

void CPlayer::Move(float x, float y, float z)
{
	Move(XMFLOAT3(x, y, z), false);
}

void CPlayer::Rotate(float fPitch, float fYaw, float fRoll)
{
	XMFLOAT3 saveLook = m_xmf3Look;
	XMFLOAT3 saveUp = m_xmf3Up;
	XMFLOAT3 saveRight = m_xmf3Right;

	
	if (fPitch != 0.0f)
	{
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Right), XMConvertToRadians(fPitch));
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, mtxRotate);
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, mtxRotate);
	}
	if (fYaw != 0.0f)
	{
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Up), XMConvertToRadians(fYaw));
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, mtxRotate);
		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, mtxRotate);
	}
	if (fRoll != 0.0f)
	{
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Look), XMConvertToRadians(fRoll));
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, mtxRotate);
		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, mtxRotate);
	}

	XMFLOAT3 baseLookv = XMFLOAT3(0.0f, 0.0f, 1.0f);
	XMFLOAT3 nowLookv = m_xmf3Look; 
	float angle = Vector3::Angle(baseLookv, nowLookv);

//#ifdef _DEBUG
//	printf("%f, %f, %f\t%f, %f, %f\t%f\n", baseLookv.x, baseLookv.y, baseLookv.z,
//		nowLookv.x, nowLookv.y, nowLookv.z, angle);
//#endif

	if (angle > 60.0f) {
		m_xmf3Look = saveLook;
		m_xmf3Right = saveRight;
		m_xmf3Up = saveUp;
	}
	else {
		m_pCamera->Rotate(fPitch, fYaw, fRoll);
		m_xmf3Look = Vector3::Normalize(m_xmf3Look);
		m_xmf3Right = Vector3::Normalize(Vector3::CrossProduct(m_xmf3Up, m_xmf3Look));
		m_xmf3Up = Vector3::Normalize(Vector3::CrossProduct(m_xmf3Look, m_xmf3Right));
	}
}

void CPlayer::Update(float fTimeElapsed)
{
	Move(m_xmf3Velocity, false);

	m_pCamera->Update(this, m_xmf3Position, fTimeElapsed);
	m_pCamera->GenerateViewMatrix();

	XMFLOAT3 xmf3Deceleration = Vector3::Normalize(Vector3::ScalarProduct(m_xmf3Velocity, -1.0f));
	float fLength = Vector3::Length(m_xmf3Velocity);
	float fDeceleration = m_fFriction * fTimeElapsed;
	if (fDeceleration > fLength) fDeceleration = fLength;
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, xmf3Deceleration, fDeceleration);
}

void CPlayer::Shoot()
{
	if (bulletDelay < maxBulletDelay) {
		bulletDelay += 1;
		return;
	}

	XMFLOAT3 upV = this->GetUp();
	XMFLOAT3 lookV = this->GetLook();
	XMFLOAT3 rightV = this->GetRight();


	m_pBullet[bulletIndex].Shoot(this->GetPosition(), upV, lookV, rightV);
	bulletIndex += 1;
	if (bulletIndex > maxBulletCount)
		bulletIndex = 0;
	bulletDelay = 0;
}

void CPlayer::OnDestroy()
{
	if (Live) {
		Live = false;
		for (int i = 0; i < maxBulletCount; ++i)
			m_pBullet[i].setLive(false);
		for (int i = 0; i < numOfPaticle; ++i)
			paticle[i].SetPosition(GetPosition());
	}
}

void CPlayer::ItemChecker(int item)
{
	//enum	   itemType { bulletDeley = 0, boostGauge, speed };
	switch (item) {
	case 0:
		if(maxBulletDelay>6)
			maxBulletDelay -= 1;
		break;
	case 1:
		maxboostGauge += 10;
		break;
	//case 2:
	//	if (speed < 1.5f)
	//		speed += 0.1f;
	//	break;
	default:
		break;
	}
	//printf("ItemGet!\n");
}

void CPlayer::Boost()
{
	if (boostGauge > 0) {
		speed = boostSpeed;
		boostGauge--;
	}
	else {
		speed = backupSpeed;
	}
}

void CPlayer::endBoost()
{
	if(boostGauge<maxboostGauge)
		boostGauge++;
	speed = backupSpeed;
}

void CPlayer::MoveBack(DWORD dwDirection)
{
	if (dwDirection)
	{
		XMFLOAT3 xmf3Shift = XMFLOAT3(0, 0, 0);
		if (dwDirection & DIR_FORWARD) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, -speed);
		if (dwDirection & DIR_BACKWARD) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, speed);
		if (dwDirection & DIR_RIGHT) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, -speed);
		if (dwDirection & DIR_LEFT) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, speed);
		if (dwDirection & DIR_UP) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, -speed);
		if (dwDirection & DIR_DOWN) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, speed);

		Move(xmf3Shift, true);
	}
}

void CPlayer::Render(HDC hDCFrameBuffer, CCamera *pCamera)
{
	m_xmf4x4World._11 = m_xmf3Right.x; m_xmf4x4World._12 = m_xmf3Right.y; m_xmf4x4World._13 = m_xmf3Right.z;
	m_xmf4x4World._21 = m_xmf3Up.x; m_xmf4x4World._22 = m_xmf3Up.y; m_xmf4x4World._23 = m_xmf3Up.z;
	m_xmf4x4World._31 = m_xmf3Look.x; m_xmf4x4World._32 = m_xmf3Look.y; m_xmf4x4World._33 = m_xmf3Look.z;
	m_xmf4x4World._41 = m_xmf3Position.x; m_xmf4x4World._42 = m_xmf3Position.y; m_xmf4x4World._43 = m_xmf3Position.z;

	m_xmf4x4World = Matrix4x4::Multiply(XMMatrixRotationRollPitchYaw(XMConvertToRadians(90.0f), 0.0f, 0.0f), m_xmf4x4World);
	if (Live) {
		CGameObject::Render(hDCFrameBuffer, pCamera);
		for (int i = 0; i < maxBulletCount; ++i) {
			if (m_pBullet[i].GetLive())
				m_pBullet[i].Render(hDCFrameBuffer, pCamera);
		}
	}
	else {
		for(int i=0; i<numOfPaticle; ++i)
			paticle[i].Render(hDCFrameBuffer, pCamera);
	}
	
}

void CPlayer::Animate()
{
	if (Live) {
		CGameObject::Animate();

		for (int i = 0; i < maxBulletCount; ++i) {
			if (m_pBullet[i].GetLive())
				m_pBullet[i].Animate();
		}
	}
	else {
		if (paticleLiveTime < maxpaicleLiveTime) {
			for (int i = 0; i < numOfPaticle; ++i)
				paticle[i].Animate();
			paticleLiveTime++;
		}
	}

}


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CCamera::CCamera()
{
	m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);

	m_xmf4x4View = Matrix4x4::Identity();
	m_xmf4x4Projection = Matrix4x4::Identity();
	m_xmf4x4ViewProject = Matrix4x4::Identity();

	m_Viewport.m_xStart = 0;
	m_Viewport.m_yStart = 0;
	m_Viewport.m_nWidth = 640;
	m_Viewport.m_nHeight = 480;
}

CCamera::~CCamera()
{
}

void CCamera::GenerateViewMatrix()
{
	m_xmf3Look = Vector3::Normalize(m_xmf3Look);
	m_xmf3Right = Vector3::Normalize(Vector3::CrossProduct(m_xmf3Up, m_xmf3Look));
	m_xmf3Up = Vector3::Normalize(Vector3::CrossProduct(m_xmf3Look, m_xmf3Right));
	m_xmf4x4View._11 = m_xmf3Right.x; m_xmf4x4View._12 = m_xmf3Up.x; m_xmf4x4View._13 = m_xmf3Look.x;
	m_xmf4x4View._21 = m_xmf3Right.y; m_xmf4x4View._22 = m_xmf3Up.y; m_xmf4x4View._23 = m_xmf3Look.y;
	m_xmf4x4View._31 = m_xmf3Right.z; m_xmf4x4View._32 = m_xmf3Up.z; m_xmf4x4View._33 = m_xmf3Look.z;
	m_xmf4x4View._41 = -Vector3::DotProduct(m_xmf3Position, m_xmf3Right);
	m_xmf4x4View._42 = -Vector3::DotProduct(m_xmf3Position, m_xmf3Up);
	m_xmf4x4View._43 = -Vector3::DotProduct(m_xmf3Position, m_xmf3Look);

	m_xmf4x4ViewProject = Matrix4x4::Multiply(m_xmf4x4View, m_xmf4x4Projection);
}

void CCamera::SetLookAt(XMFLOAT3& xmf3Position, XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up)
{
	m_xmf3Position = xmf3Position;
	m_xmf4x4View = Matrix4x4::LookAtLH(m_xmf3Position, xmf3LookAt, xmf3Up);
	m_xmf3Right = Vector3::Normalize(XMFLOAT3(m_xmf4x4View._11, m_xmf4x4View._21, m_xmf4x4View._31));
	m_xmf3Up = Vector3::Normalize(XMFLOAT3(m_xmf4x4View._12, m_xmf4x4View._22, m_xmf4x4View._32));
	m_xmf3Look = Vector3::Normalize(XMFLOAT3(m_xmf4x4View._13, m_xmf4x4View._23, m_xmf4x4View._33));
}

void CCamera::SetLookAt(XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up)
{
	XMFLOAT4X4 xmf4x4View = Matrix4x4::LookAtLH(m_xmf3Position, xmf3LookAt, xmf3Up);
	m_xmf3Right = Vector3::Normalize(XMFLOAT3(xmf4x4View._11, xmf4x4View._21, xmf4x4View._31));
	m_xmf3Up = Vector3::Normalize(XMFLOAT3(xmf4x4View._12, xmf4x4View._22, xmf4x4View._32));
	m_xmf3Look = Vector3::Normalize(XMFLOAT3(xmf4x4View._13, xmf4x4View._23, xmf4x4View._33));
}

void CCamera::SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight)
{
	m_Viewport.m_xStart = xTopLeft;
	m_Viewport.m_yStart = yTopLeft;
	m_Viewport.m_nWidth = nWidth;
	m_Viewport.m_nHeight = nHeight;
}

void CCamera::GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fFOVAngle)
{
	float fAspectRatio = (float(m_Viewport.m_nWidth) / float(m_Viewport.m_nHeight));
	m_xmf4x4Projection = Matrix4x4::PerspectiveFovLH(fFOVAngle, fAspectRatio, fNearPlaneDistance, fFarPlaneDistance);
}

void CCamera::Move(XMFLOAT3& xmf3Shift)
{
	m_xmf3Position = Vector3::Add(m_xmf3Position, xmf3Shift);
}

void CCamera::Move(float x, float y, float z)
{
	Move(XMFLOAT3(x, y, z));
}

void CCamera::Rotate(float fPitch, float fYaw, float fRoll)
{
	if (fPitch != 0.0f)
	{
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Right), XMConvertToRadians(fPitch));
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, mtxRotate);
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, mtxRotate);
	}
	if (fYaw != 0.0f)
	{
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Up), XMConvertToRadians(fYaw));
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, mtxRotate);
		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, mtxRotate);
	}
	if (fRoll != 0.0f)
	{
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Look), XMConvertToRadians(fRoll));
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, mtxRotate);
		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, mtxRotate);
	}
}

void CCamera::Update(CPlayer *pPlayer, XMFLOAT3& xmf3LookAt, float fTimeElapsed)
{
	XMFLOAT4X4 mtxRotate = Matrix4x4::Identity();
	mtxRotate._11 = pPlayer->m_xmf3Right.x; mtxRotate._21 = pPlayer->m_xmf3Up.x; mtxRotate._31 = pPlayer->m_xmf3Look.x;
	mtxRotate._12 = pPlayer->m_xmf3Right.y; mtxRotate._22 = pPlayer->m_xmf3Up.y; mtxRotate._32 = pPlayer->m_xmf3Look.y;
	mtxRotate._13 = pPlayer->m_xmf3Right.z; mtxRotate._23 = pPlayer->m_xmf3Up.z; mtxRotate._33 = pPlayer->m_xmf3Look.z;

	XMFLOAT3 xmf3Offset = Vector3::TransformCoord(pPlayer->m_xmf3CameraOffset, mtxRotate);
	XMFLOAT3 xmf3Position = Vector3::Add(pPlayer->m_xmf3Position, xmf3Offset);
	XMFLOAT3 xmf3Direction = Vector3::Subtract(xmf3Position, m_xmf3Position);
	float fLength = Vector3::Length(xmf3Direction);
	xmf3Direction = Vector3::Normalize(xmf3Direction);

	float fTimeLagScale = fTimeElapsed * (1.0f / 0.25f);
	float fDistance = fLength * fTimeLagScale;
	if (fDistance > fLength) fDistance = fLength;
	if (fLength < 0.01f) fDistance = fLength;
	if (fDistance > 0)
	{
		m_xmf3Position = Vector3::Add(m_xmf3Position, xmf3Direction, fDistance);
		SetLookAt(pPlayer->m_xmf3Position, pPlayer->m_xmf3Up);
	}
}

