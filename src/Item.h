#pragma once
#include "GameObject.h"

class Item : public CGameObject {
private:
	int m_type;
	bool m_live;
public:
	Item();
	virtual ~Item();
	bool setType(int, XMFLOAT3&);
	void setLive(bool live) { m_live = live; }
	bool getLive() const { return m_live; }
	int  getType() const { return m_type; }

	virtual void Animate(XMFLOAT3&);
};