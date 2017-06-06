#pragma once
#include "Mesh.h"
#include "Camera.h"
class CShader;

class CGameObject
{
private: 
	int m_nReferences = 0;

	BoundingOrientedBox m_xmOOBB; //모델좌표계에서의 충돌영역
	BoundingOrientedBox	m_xmOOBBTransformed; //월드좌표계에서의 충돌 영역

protected:
	XMFLOAT4X4 m_xmf4x4World; 
	CMesh *m_pMesh = NULL;

public:
	CGameObject();
	virtual ~CGameObject();

	void AddRef() { m_nReferences++; } 
	void Release() { if (--m_nReferences <= 0) delete this; }

	

	void ReleaseUploadBuffers();
	virtual void SetMesh(CMesh *pMesh);
	virtual void SetShader(CShader *pShader);
	virtual void Animate(float fTimeElapsed);
	virtual void OnPrepareRender(); 
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);

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

	void MoveStrafe(float fDistance = 1.0f);
	void MoveUp(float fDistance = 1.0f);
	void MoveForward(float fDistance = 1.0f);
	void MoveVector(XMFLOAT3 dir, float fDistance=1.0f);

	void SetOOBB(XMFLOAT3& xmCenter, XMFLOAT3& xmExtents, XMFLOAT4& xmOrientation) { m_xmOOBBTransformed = m_xmOOBB = BoundingOrientedBox(xmCenter, xmExtents, xmOrientation); }
	BoundingOrientedBox* getOOBB() { return &m_xmOOBBTransformed; } //트랜스폼 oobb주소 반환
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
};

class CWallObject : public CGameObject
{
private:
	bool set = false;
	int index = -1;

	float width		= 0.0f;
	float height	= 0.0f;
	float depth		= 0.0f;


public:
	CWallObject();
	virtual ~CWallObject();

	void setObject(float w, float h, float d);
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

class CPaticles
{
private:
	Paticle *paticle = NULL;
	int numOfPaticle = 0;

	bool run = false;
	float runtime = 0.0f;
	float maxtime = 50.0f;
public:
	CPaticles(int num, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	~CPaticles();

	void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);
	void Animate(float fTimeElapsed);

	void setPositions(XMFLOAT3 pos); 
	void setRun();

	bool getRun() const { return run; }
};