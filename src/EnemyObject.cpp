#include "GameObject.h"
#include "stdafx.h"
#include "EnemyObject.h"
#include "Player.h"
#include <random>


CCubeMesh *baseCubeMesh = new CCubeMesh(4.0f, 4.0f, 4.0f);
CCubeMesh *paticleMesh = new CCubeMesh(0.5f, 0.5f, 0.5f);


std::default_random_engine dre;
std::uniform_int_distribution<> ui(-10, 10);

std::uniform_int_distribution<> x(-20.0f, 20.0f);
std::uniform_int_distribution<> y(-13.0f, 13.0f);
std::uniform_int_distribution<> z(0.0f, 100.0f);
std::uniform_int_distribution<> color(0,255);
std::uniform_int_distribution<> select(0, 3);

EnemyCube::EnemyCube() : LiveCube{ true }, LivePaticle{ false }, Live{ true }, paticleLiveTime{ 0 }, numOfPaticle{ 50 } {
	paticle = new Paticle[numOfPaticle];
	m_pMesh = baseCubeMesh;
}


EnemyCube::~EnemyCube() {
	delete[] paticle;


}

void EnemyCube::setCube(float posz, float rotSpeed, float movSpeed) {

	XMFLOAT3 Pos(x(dre), y(dre), z(dre)+posz);
	DWORD color(RGB(color(dre), color(dre), color(dre)));

	switch (select(dre)) {
	case 0:
		SetRotationAxis(XMFLOAT3(1.0f, 0.0f, 0.0f));
		SetMovingDirection(XMFLOAT3(1.0f, 0.0f, 0.0f));
		break;
	case 1:
		SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
		SetMovingDirection(XMFLOAT3(0.0f, 1.0f, 0.0f));
		break;
	case 2:
		SetRotationAxis(XMFLOAT3(0.0f, 0.0f, 1.0f));
		SetMovingDirection(XMFLOAT3(0.0f, 0.0f, 1.0f));
		break;
	case 3:
		SetRotationAxis(XMFLOAT3(1.0f, 1.0f, 1.0f));
		SetMovingDirection(XMFLOAT3(1.0f, 1.0f, 1.0f));
		break;
	default:
		SetRotationAxis(XMFLOAT3(1.0f, 1.0f, 1.0f));
		SetMovingDirection(XMFLOAT3(1.0f, 1.0f, 1.0f));
		break;
	}
	SetPosition(Pos);
	SetColor(color);
	SetRotationSpeed(rotSpeed);
	SetMovingSpeed(movSpeed);

	SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	
	for (int i = 0; i < numOfPaticle; ++i) {
		paticle[i].SetMesh(paticleMesh);
		paticle[i].SetColor(color);
		paticle[i].SetRotationAxis(XMFLOAT3(1.0f, 1.0f, 1.0f));
		paticle[i].SetRotationSpeed(1.0f);
		paticle[i].SetMovingDirection(XMFLOAT3(ui(dre), ui(dre), ui(dre)));
		paticle[i].SetMovingSpeed(1.0f);
		//paticle[i].SetPosition(pos);
	}
}

void EnemyCube::DestroyObject() {
	if (!LiveCube)
		return;
	LiveCube = false;
	LivePaticle = true;
	for (int i = 0; i < numOfPaticle; ++i) {
		paticle[i].SetPosition(GetPosition());
	}
	
}

void EnemyCube::Animate(XMFLOAT3 pos) {
	if (!Live) {
  	    //this->MoveForward(pos.z + 100.0f);
		this->setCube(pos.z+100, 1.0f, 0.1f);
		Live = true;
		LiveCube = true;
		//return;
	}
	if (LiveCube) {
		if (this->GetPosition().z < pos.z-50) {
			LiveCube = false;
			LivePaticle = false;
			Live = false;
		}
		CGameObject::Animate();
		return;
	}
	if(LivePaticle){
		if (paticleLiveTime > 50) {
			LivePaticle = false;
			paticleLiveTime = 0;
			Live = false;
			return;
		}
		for (int i = 0; i < numOfPaticle; ++i) {
			paticle[i].Animate();
		}
		paticleLiveTime += 1;
	}
	
}



void EnemyCube::Render(HDC hDCFrameBuffer, CCamera *pCamera) {
	if (LiveCube) {
		CGameObject::Render(hDCFrameBuffer, pCamera);
	}
	if(LivePaticle){
		for (int i = 0; i < numOfPaticle; ++i) {
			paticle[i].Render(hDCFrameBuffer, pCamera);
		}
	}
}



