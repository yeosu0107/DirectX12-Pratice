#pragma once
#include "GameObject.h"



class EnemyCube : public CGameObject {
private:
	bool LiveCube;
	bool LivePaticle;
	bool Live;

	int numOfPaticle;
	Paticle *paticle;
	int paticleLiveTime;
public:
	EnemyCube();
	virtual ~EnemyCube();
	
	bool getCubeLive() { return LiveCube; }
	bool getPaticleLive() { return LivePaticle; }
	bool getLive() { return Live; }

	void setCube(float posz, float rotSpeed, float movSpeed);
	//void setCube(XMFLOAT3 Pos);
	void DestroyObject();
	void setNumOfPaticle(int num) { numOfPaticle = num; }
	virtual void Animate(XMFLOAT3& pos, float movSpeed, float rotSpeed);
	virtual void Render(HDC hDCFrameBuffer, CCamera *pCamera);
};

