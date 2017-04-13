#include "GameObject.h"
#include "stdafx.h"
#include "Map.h"


Wall::Wall() {

}
Wall::~Wall() {

}

void Wall::Animate(XMFLOAT3& playerPos) {
	if (playerPos.z > zPos && playerPos.z < zPos + size)
		Live = true;
}

void Wall::Render(HDC hDCFrameBuffer, CCamera *pCamera) {
	if (Live) {

	}
}

Tile::Tile() {

}

Tile::~Tile() {

}

void Tile::Animate() {

}