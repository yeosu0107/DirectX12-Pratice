#pragma once
#include "GameObject.h"

class Tile : public CGameObject {
private:
public:
	Tile();
	~Tile();
	virtual void Animate();
};

class Wall : public CGameObject {
private:
	bool Live;
	int zPos;
	int size;
public:
	Wall();
	~Wall();

	const bool getLive() { return Live; }

	virtual void Render(HDC hDCFrameBuffer, CCamera *pCamera);
	virtual void Animate(XMFLOAT3& playerPos);
};