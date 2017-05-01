#include "GameObject.h"
#include "stdafx.h"
#include "Bullet.h"




Bullet::Bullet() : Live{ false }, damage{ 0 }, LimitTime{ 0 }, maxLimitTime{ 1000 } {
	//CCubeMesh *bulletMesh = new CCubeMesh(0.5, 0.5, 0.5);
	//m_pMesh = bulletMesh;
	SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	SetMovingSpeed(3.0f);
	SetRotationAxis(XMFLOAT3(0.0f, 0.0f, 0.0f));
	SetRotationSpeed(0.0f);
}

Bullet::~Bullet() {
	//CGameObject::~CGameObject();
}

void Bullet::SetBullet(DWORD dwColor, float movSpeed)
{
	SetColor(dwColor);
	SetMovingSpeed(movSpeed);
}

void Bullet::Shoot(XMFLOAT3 pos, XMFLOAT3 dir, XMFLOAT3 up, XMFLOAT3 right)
{
	if (Live)
		return;
	/*XMFLOAT3 look = this->GetLook();
	XMFLOAT3 rightV = this->GetRight();
	float angleUP = Vector3::Angle(look, dir)-90.0f;
	float angleRIGHT = Vector3::Angle(rightV, right)-90.0f;*/

	SetPosition(pos);
	//Rotate(angleUP, 0, angleRIGHT);
	//Rotate(right, angleUP);
	SetMovingDirection(dir);

	Live = true;
}



void Bullet::Animate()
{
	if (Live && LimitTime >= maxLimitTime) {
		Live = false;
		LimitTime = 0;
		return;
	}
	XMFLOAT3 tmpPos = this->GetPosition();
	if (Live && ((tmpPos.x > mapHalfWidth + 10) || (tmpPos.x < -mapHalfWidth - 10) || (tmpPos.y > mapHalfHeight + 10) || (tmpPos.y < -mapHalfHeight - 10))) {
		Live = false;
		LimitTime = 0;
		return;
	}

	LimitTime++;
	CGameObject::Animate();
}

