#pragma once
#include "GameObject.h"

extern const float mapHalfWidth;
extern const float mapHalfHeight;

class Bullet : public CGameObject {
private:
	int damage;
	bool Live;
	int LimitTime;
	int maxLimitTime;
public:
	Bullet();
	virtual ~Bullet();

	void SetBullet(DWORD dwColor, float movSpeed);
	void Shoot(XMFLOAT3 pos, XMFLOAT3 dir, XMFLOAT3 up, XMFLOAT3 right);
	
	bool GetLive() const { return Live; }
	void setLive(bool tmp) { Live = tmp; }
	//virtual void Render(HDC hDCFrameBuffer, CCamera *pCamera);
	virtual void Animate();
};