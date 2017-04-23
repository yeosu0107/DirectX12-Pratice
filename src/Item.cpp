#include "GameObject.h"
#include "stdafx.h"
#include "Item.h"



DWORD	   itemColor = RGB(255, 100, 255);

enum	   itemType { bulletDeley = 0, boostGauge };

Item::Item() : m_live { false } {
	
	//m_pMesh = itemMesh;
	SetColor(itemColor);
	SetRotationAxis(XMFLOAT3(1.0f, 1.0f, 1.0f)); 
	SetRotationSpeed(10.0f);
	SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.5f, 1.5f, 1.5f), XMFLOAT4(0, 0, 0, 1.0f));
}

Item::~Item() {
	//CGameObject::~CGameObject();
}

bool Item::setType(int type, XMFLOAT3& pos) {
	if (type > itemType::boostGauge)
		return false;
	m_type = type;
	SetPosition(pos);
	return true;
}

void Item::Animate(XMFLOAT3& pos)
{
	if (this->GetPosition().z < pos.z - 50)
		m_live = false;
	CGameObject::Animate();
}
