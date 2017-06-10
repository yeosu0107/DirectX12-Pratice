#pragma once
#include "Mesh.h"
#include "Camera.h"
class CShader;

class CGameObject
{
private: 
protected:
	bool die = false;
	XMFLOAT4 color;

	float width = 0.0f;
	float height = 0.0f;
	float depth = 0.0f;

	XMFLOAT4X4 m_xmf4x4World; 
	CMesh *m_pMesh = NULL;

	int m_nReferences = 0;

	BoundingOrientedBox m_xmOOBB; //모델좌표계에서의 충돌영역
	BoundingOrientedBox	m_xmOOBBTransformed; //월드좌표계에서의 충돌 영역

	XMFLOAT3 movingDir;
public:
	CGameObject();
	virtual ~CGameObject();

	void AddRef() { m_nReferences++; } 
	void Release() { if (--m_nReferences <= 0) delete this; }

	

	void ReleaseUploadBuffers();
	virtual void SetMesh(CMesh *pMesh);
	virtual void SetShader(CShader *pShader);
	virtual void SetObject(float w, float h, float d);
	virtual void Animate(float fTimeElapsed);
	virtual void OnPrepareRender(); 
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera, UINT
		nInstances, D3D12_VERTEX_BUFFER_VIEW d3dInstancingBufferView);

	//상수 버퍼를 생성한다. 
	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList
	*pd3dCommandList);
	//상수 버퍼의 내용을 갱신한다. 
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetLook();
	XMFLOAT3 GetUp();
	XMFLOAT3 GetRight();

	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 xmf3Position);
	void Rotate(XMFLOAT3 *pxmf3Axis, float fAngle);
	void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
	void Scale(float num);
	void setScale(float num);

	void MoveStrafe(float fDistance = 1.0f);
	void MoveUp(float fDistance = 1.0f);
	void MoveForward(float fDistance = 1.0f);
	void MoveVector(XMFLOAT3 dir, float fDistance=1.0f);
	void Move(float fDist = 3.0f);

	void setMovingDir(XMFLOAT3 dir) { movingDir = dir; }
	XMFLOAT3& getMovingDir() { return movingDir; }

	void SetOOBB(XMFLOAT3& xmCenter, XMFLOAT3& xmExtents, XMFLOAT4& xmOrientation) { m_xmOOBBTransformed = m_xmOOBB = BoundingOrientedBox(xmCenter, xmExtents, xmOrientation); }
	BoundingOrientedBox* getOOBB() { return &m_xmOOBBTransformed; } //트랜스폼 oobb주소 반환
	XMFLOAT4X4& getMatrix() { return m_xmf4x4World; }

	bool getDie() const { return die; }
	void setDie(bool type) { die = type; }
	virtual void Reset(XMFLOAT3 pos) {}

	XMFLOAT4& getColor() { return color; }
	void setColor(XMFLOAT4& t) { color = t; }
};

class CRotatingObject : public CGameObject 
{
private:
	XMFLOAT3 m_xmf3RotationAxis; 
	float m_fRotationSpeed;
	

public: 
	CRotatingObject(); 
	virtual ~CRotatingObject();

	void SetRotationSpeed(float fRotationSpeed) { m_fRotationSpeed = fRotationSpeed; } 
	void SetRotationAxis(XMFLOAT3 xmf3RotationAxis) { m_xmf3RotationAxis = xmf3RotationAxis; }
	virtual void Animate(float fTimeElapsed);
	
	virtual void Reset(XMFLOAT3 pos);
	void setType(int type=0);
};

class CWallObject : public CGameObject
{
private:
	bool set = false;
	int index = -1;

public:
	CWallObject();
	virtual ~CWallObject();

	virtual void Animate(float fTimeElapsed);

	float getWidth() const { return width; }
	float getHeight() const { return height; }
	float getDepth() const { return depth; }
};

class Paticle : public CGameObject
{
private:
	XMFLOAT3 dir = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 axis = XMFLOAT3(0.0f, 0.0f, 0.0f);

	float movingSpeed = 0.0f;
	float rotSpeed = 0.0f;
public:
	Paticle() {	}
	virtual ~Paticle() { }

	void setPaticle(XMFLOAT3 tdir, XMFLOAT3 taxis, float mov, float rot);

	virtual void Animate(float fTimeElapsed);
};

class CBullet : public CGameObject
{
private:
	float runtime = 0.0f;
	float maxtime = 50.0f;
public:
	CBullet();
	virtual ~CBullet();

	virtual void Animate(float fTimeElapsed);
};