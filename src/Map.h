#pragma once
#include "GameObject.h"


class Tile :public CGameObject {
private:

public:
	Tile();
	~Tile();

	//virtual void Animate() {}
	//virtual void Render(HDC hDCFrameBuffer, CCamera *pCamera);
};

class Wall : public CGameObject {
private:
	bool Live;
	float zPos;
	float size;
	int index;

	float width;
	float height;
	float depth;

	Tile *tile;
	int n_tile;
public:
	Wall();
	~Wall();

	const bool getLive() { return Live; }
	void SetIndex(int tmp) { index = tmp; }
	void SetTile(float zPos);
	Tile* GetTile()const { return tile; }
	virtual void Render(HDC hDCFrameBuffer, CCamera *pCamera);
	virtual void Animate(XMFLOAT3& playerPos);
};