#include "GameObject.h"
#include "stdafx.h"
#include "EnemyObject.h"
#include "Player.h"
#include <random>


CCubeMesh *baseCubeMesh = new CCubeMesh(4.0, 4.0, 4.0);
CCubeMesh *paticleMesh = new CCubeMesh(1.0, 1.0, 1.0);


std::default_random_engine dre;
std::uniform_int_distribution<> ui(-10, 10);

EnemyCube::EnemyCube() : LiveCube{ true }, LivePaticle{ false }, Live{ true }, paticleLiveTime{ 0 }, numOfPaticle{ 40 } {
	paticle = new Paticle[numOfPaticle];
	m_pMesh = baseCubeMesh;
}


EnemyCube::~EnemyCube() {
	delete[] paticle;


}

void EnemyCube::setCube(XMFLOAT3 pos, DWORD dwColor,
	XMFLOAT3 Axis, float rotSpeed, XMFLOAT3 dir, float movSpeed) {

	SetPosition(pos);
	SetColor(dwColor);
	SetRotationAxis(Axis);
	SetRotationSpeed(rotSpeed);
	SetMovingDirection(dir);
	SetMovingSpeed(movSpeed);
	SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	
	for (int i = 0; i < numOfPaticle; ++i) {
		paticle[i].SetMesh(paticleMesh);
		paticle[i].SetColor(dwColor);
		paticle[i].SetRotationAxis(XMFLOAT3(1.0f, 1.0f, 1.0f));
		paticle[i].SetRotationSpeed(0.05f);
		paticle[i].SetMovingDirection(XMFLOAT3(ui(dre), ui(dre), ui(dre)));
		paticle[i].SetMovingSpeed(0.1f);
		//paticle[i].SetPosition(pos);
	}
}

void EnemyCube::DestroyObject() {
	LiveCube = false;
	LivePaticle = true;
	for (int i = 0; i < numOfPaticle; ++i) {
		paticle[i].SetPosition(GetPosition());
	}
}

void EnemyCube::Animate() {
	if (LiveCube) {
		if (m_fRotationSpeed != 0.0f) Rotate(m_xmf3RotationAxis, m_fRotationSpeed);
		if (m_fMovingSpeed != 0.0f) Move(m_xmf3MovingDirection, m_fMovingSpeed);

		m_xmOOBBTransformed.Transform(m_xmOOBB, XMLoadFloat4x4(&m_xmf4x4World));
		XMStoreFloat4(&m_xmOOBBTransformed.Orientation, XMQuaternionNormalize(XMLoadFloat4(&m_xmOOBBTransformed.Orientation)));
		return;
	}
	if(LivePaticle){
		if (paticleLiveTime > 500) {
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
		XMFLOAT4X4 xm4x4Transform = Matrix4x4::Multiply(m_xmf4x4World, pCamera->m_xmf4x4ViewProject);
		HPEN hPen = ::CreatePen(PS_SOLID, 0, m_dwColor);
		HPEN hOldPen = (HPEN)::SelectObject(hDCFrameBuffer, hPen);
		if (m_pMesh) m_pMesh->Render(hDCFrameBuffer, xm4x4Transform, pCamera);
		::SelectObject(hDCFrameBuffer, hOldPen);
		::DeleteObject(hPen);
	}
	if(LivePaticle){
		for (int i = 0; i < numOfPaticle; ++i) {
			paticle[i].Render(hDCFrameBuffer, pCamera);
		}
	}
}

void Paticle::Animate() {
	if (m_fRotationSpeed != 0.0f)
		Rotate(m_xmf3RotationAxis, m_fRotationSpeed);
	if (m_fMovingSpeed != 0.0f)
		Move(m_xmf3MovingDirection, m_fMovingSpeed);
}