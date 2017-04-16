#include "GameObject.h"
#include "stdafx.h"
#include "Map.h"
#include "Player.h"


Tile::Tile() {
	//SetRotationAxis(0.0f, 0.0f, 0.0f);

	SetRotationSpeed(0.0f);
}

Tile::~Tile() {

}

//void Tile::Render(HDC hDCFrameBuffer, CCamera *pCamera) {
//
//}

Wall::Wall() : size{ 300 }, Live{ false }, n_tile{ 48 } {
	tile = new Tile[n_tile];
	float tileWidth = 10.0f;
	float tileDepth = 20.0f;
	float halfTileWidth = tileWidth / 2;
	//float halfTileDepth = tileDepth / 2;

	CWallTileMesh *tileMesh = new CWallTileMesh(tileWidth, 0, tileDepth);
	width = 20.0f;
	height = 20.0f;
	depth = 60.0f;

	for (int index = 0; index < n_tile; ++index) {
		tile[index].SetMesh(tileMesh);
		//tile[index].SetOOBB(XMFLOAT3(0, 0, 0), XMFLOAT3(tileWidth, 0, tileDepth), XMFLOAT4(0, 0, 0, 1));

	}
	int index = 0;
	float diff = -10;

	//up
	for (float i = 0; i < 3; ++i) {
		float j = 1;
		tile[index].SetPosition(0, height - halfTileWidth, depth*(i / 3) + (j-1) * diff);
		//tile[index].SetOOBB(XMFLOAT3(0, 0, 0), XMFLOAT3(tileWidth, 0, tileDepth), XMFLOAT4(0, 0, 0, 1));
		index++;
		for (; j < 3; ++j) {
			tile[index].SetPosition(width* (j / 2), height - halfTileWidth, depth*(i / 3) + j * diff);
			//tile[index].SetOOBB(XMFLOAT3(0, 0, 0), XMFLOAT3(tileWidth, 0, tileDepth), XMFLOAT4(0, 0, 0, 1));
			index++;
			tile[index].SetPosition(-width* (j / 2), height - halfTileWidth, depth*(i / 3) + j * diff);
			//tile[index].SetOOBB(XMFLOAT3(0, 0, 0), XMFLOAT3(tileWidth, 0, tileDepth), XMFLOAT4(0, 0, 0, 1));
			index++;
		}
	}

	//botton
	for (float i = 0; i < 3; ++i) {
		float j = 1;
		tile[index].SetPosition(0, -height + halfTileWidth, depth*(i / 3) + (j - 1) * diff);
		//tile[index].SetOOBB(XMFLOAT3(0, 0, 0), XMFLOAT3(tileWidth, 0, tileDepth), XMFLOAT4(0, 0, 0, 1));
		index++;
		for (; j < 3; ++j) {
			tile[index].SetPosition(width* (j / 2), -height + halfTileWidth, depth*(i / 3) + j * diff);
			//tile[index].SetOOBB(XMFLOAT3(0, 0, 0), XMFLOAT3(tileWidth, 0, tileDepth), XMFLOAT4(0, 0, 0, 1));
			index++;
			tile[index].SetPosition(-width* (j / 2), -height + halfTileWidth, depth*(i / 3) + j * diff);
			//tile[index].SetOOBB(XMFLOAT3(0, 0, 0), XMFLOAT3(tileWidth, 0, tileDepth), XMFLOAT4(0, 0, 0, 1));
			index++;
		}
	}

	//Right
	for (float i = 0; i < 3; ++i) {
		float j = 1;
		tile[index].SetPosition(width + halfTileWidth, 0, depth*(i / 3) + (j - 1) * diff);
		tile[index].Rotate(0, 90, 0);
		//tile[index].SetOOBB(XMFLOAT3(0, 0, 0), XMFLOAT3(tileWidth, 0, tileDepth), XMFLOAT4(0, 1, 0, 1));
		index++;
		for (; j < 2; ++j) {
			tile[index].SetPosition(width + halfTileWidth, height* (j / 2), depth*(i / 3) + j * diff);
			tile[index].Rotate(0, 90, 0);
			//tile[index].SetOOBB(XMFLOAT3(0, 0, 0), XMFLOAT3(tileWidth, 0, tileDepth), XMFLOAT4(0, 1, 0, 1));
			index++;
			tile[index].SetPosition(width + halfTileWidth, -height* (j / 2), depth*(i / 3) + j * diff);
			tile[index].Rotate(0, 90, 0);
			//tile[index].SetOOBB(XMFLOAT3(0, 0, 0), XMFLOAT3(tileWidth, 0, tileDepth), XMFLOAT4(0, 1, 0, 1));
			index++;
		}
	}

	//Left
	for (float i = 0; i < 3; ++i) {
		float j = 1;
		tile[index].SetPosition(-width - halfTileWidth, 0, depth*(i / 3) + (j - 1) * diff);
		tile[index].Rotate(0, 90, 0);
		//tile[index].SetOOBB(XMFLOAT3(0, 0, 0), XMFLOAT3(tileWidth, 0, tileDepth), XMFLOAT4(0, 1, 0, 1));
		index++;
		for (; j < 2; ++j) {
			tile[index].SetPosition(-width - halfTileWidth, height* (j / 2), depth*(i / 3) + j * diff);
			tile[index].Rotate(0, 90, 0);
			//tile[index].SetOOBB(XMFLOAT3(0, 0, 0), XMFLOAT3(tileWidth, 0, tileDepth), XMFLOAT4(0, 1, 0, 1));
			index++;
			tile[index].SetPosition(-width - halfTileWidth, -height* (j / 2), depth*(i / 3) + j * diff);
			tile[index].Rotate(0, 90, 0);
			//tile[index].SetOOBB(XMFLOAT3(0, 0, 0), XMFLOAT3(tileWidth, 0, tileDepth), XMFLOAT4(0, 1, 0, 1));
			index++;
		}
	}

}



Wall::~Wall() {
	delete[] tile;
}

void Wall::SetTile(float zPos) {
	for (int i = 0; i < n_tile; ++i) {
		tile[i].MoveForward(zPos);
		//tile[i].SetColor(this->getColor());
	}
	return;
}

void Wall::Animate(XMFLOAT3& playerPos) {
	zPos = this->GetPosition().z;
	
	if (playerPos.z > zPos - size && playerPos.z < zPos + size/3 ) {
		Live = true;
		for (int i = 0; i < n_tile; ++i)
			tile[i].Animate();
		return;
	}
	else if(Live) {
		index += 5;
		SetPosition(0, 0, index * 60);
		SetTile(300);
		Live = false;
		return;
	}
		
	
}



void Wall::Render(HDC hDCFrameBuffer, CCamera *pCamera) {
	if (Live) {
		for (int i = 0; i < n_tile; ++i)
			tile[i].Render(hDCFrameBuffer, pCamera);
	}

}

