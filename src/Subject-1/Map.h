#pragma once
#include "GameObject.h"

class Wall : public CGameObject {
private:
	bool Live;
	float zPos;
	float size;
	int index;

	float width;
	float height;
	float depth;

	CGameObject *tile;
	int n_tile;
public:
	Wall();
	virtual ~Wall();

	const bool getLive() { return Live; }
	void SetIndex(int tmp) { index = tmp; }
	void SetTile(float zPos);
	CGameObject* GetTile()const { return tile; }
	virtual void Render(HDC hDCFrameBuffer, CCamera *pCamera);
	virtual void Animate(XMFLOAT3& playerPos);
};