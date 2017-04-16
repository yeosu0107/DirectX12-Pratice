#pragma once
#include "GameObject.h"
#include "Bullet.h"



class CPlayer : public CGameObject
{
private:
	Bullet* m_pBullet;
	int maxBulletCount;
	int bulletIndex;
	int bulletDelay;
	int maxBulletDelay;

	Paticle* paticle;
	int HP;
	int numOfPaticle;
	bool Live;
	int paticleLiveTime;
	int maxpaicleLiveTime;

	float speed;
public:
	CPlayer();
	virtual ~CPlayer();

	XMFLOAT3					m_xmf3Position;
	XMFLOAT3					m_xmf3Right;
	XMFLOAT3					m_xmf3Up;
	XMFLOAT3					m_xmf3Look;

	XMFLOAT3					m_xmf3CameraOffset;
	XMFLOAT3					m_xmf3Velocity;
	float						m_fFriction;

	float           			m_fPitch;
	float           			m_fYaw;
	float           			m_fRoll;

	CCamera						*m_pCamera;


	void SetPosition(float x, float y, float z);
	void Move(DWORD dwDirection);
	void Move(XMFLOAT3& xmf3Shift, bool bUpdateVelocity);
	void Move(float x, float y, float z);
	void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);
	void SetCameraOffset(XMFLOAT3& xmf3CameraOffset);
	void Update(float fTimeElapsed = 0.016f);

	
	void Shoot();
	void OnDestroy();

	bool getLive() const { return Live; }
	void setSpeed(float tmp) { speed = tmp; }
	float getSpeed() const { return speed; }

	virtual void Render(HDC hDCFrameBuffer, CCamera *pCamera);
	virtual void Animate();

	Bullet* GetBullet() const{ return m_pBullet; }
};


class CCamera
{
public:
	CCamera();
	virtual ~CCamera();

	XMFLOAT3				m_xmf3Position;
	XMFLOAT3				m_xmf3Right;
	XMFLOAT3				m_xmf3Up;
	XMFLOAT3				m_xmf3Look;

	XMFLOAT4X4				m_xmf4x4View;
	XMFLOAT4X4				m_xmf4x4Projection;
	XMFLOAT4X4				m_xmf4x4ViewProject;

	VIEWPORT				m_Viewport;

	void GenerateViewMatrix();
	void GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fFOVAngle);
	void SetViewport(int xStart, int yStart, int nWidth, int nHeight);

	void SetLookAt(XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up);
	void SetLookAt(XMFLOAT3& vPosition, XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up);

	void Move(XMFLOAT3& xmf3Shift);
	void Move(float x, float y, float z);
	void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);
	void Update(CPlayer *pPlayer, XMFLOAT3& xmf3LookAt, float fTimeElapsed = 0.016f);
};


