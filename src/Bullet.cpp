#include "GameObject.h"
#include "stdafx.h"
#include "Bullet.h"


CCubeMesh *bulletMesh = new CCubeMesh(1.0, 1.0, 1.0);

Bullet::Bullet() : Live{ false }, damage{ 0 }, LimitTime{ 0 }, maxLimitTime{ 1000 } {
	m_pMesh = bulletMesh;
	SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	SetMovingSpeed(0.4);
	SetRotationAxis(XMFLOAT3(0.0f, 0.0f, 0.0f));
	SetRotationSpeed(0.0f);
}

Bullet::~Bullet() {

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
	LimitTime++;
	CGameObject::Animate();
}

