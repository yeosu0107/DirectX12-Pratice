#pragma once
#include "Mesh.h"
#include "Camera.h"
class CShader;

enum enemyStatus { normal = 0, move = 1, attack = 2, death = 3 };

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
	CMesh **m_ppMeshes = NULL;

	int m_nMeshes = 0;
	int m_nReferences = 0;

	BoundingOrientedBox m_xmOOBB; //모델좌표계에서의 충돌영역
	BoundingOrientedBox	m_xmOOBBTransformed; //월드좌표계에서의 충돌 영역

	XMFLOAT3 movingDir;
public:
	CGameObject(int nMeshes=1);
	virtual ~CGameObject();

	void AddRef() { m_nReferences++; } 
	void Release() { if (--m_nReferences <= 0) delete this; }

	

	void ReleaseUploadBuffers();
	virtual void SetMesh(int nIndex, CMesh *pMesh);
	virtual void SetShader(CShader *pShader);
	//virtual void SetObject(float w, float h, float d);
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
	virtual void Rotate(XMFLOAT3 *pxmf3Axis, float fAngle);
	virtual void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
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
	void SetOOBB(BoundingOrientedBox& oobb) { m_xmOOBB = oobb; }
	BoundingOrientedBox* getOOBB() { return &m_xmOOBBTransformed; } //트랜스폼 oobb주소 반환
	XMFLOAT4X4& getMatrix() { return m_xmf4x4World; }

	bool getDie() const { return die; }
	void setDie(bool type) { die = type; }
	virtual void Reset(XMFLOAT3 pos) {}
	virtual void setStatus(enemyStatus type) {}
	virtual void StatusWork() {}
	virtual enemyStatus getStatus() { return enemyStatus::normal; }

	XMFLOAT4& getColor() { return color; }
	void setColor(XMFLOAT4& t) { color = t; }

	//모델 좌표계의 픽킹 광선을 생성한다.
	void GenerateRayForPicking(XMFLOAT3& xmf3PickPosition, 
		XMFLOAT4X4& xmf4x4View, XMFLOAT3 *pxmf3PickRayOrigin, XMFLOAT3 *pxmf3PickRayDirection);
	//카메라 좌표계의 한 점에 대한 모델 좌표계의 픽킹 광선을 생성하고 객체와의 교차를 검사한다. 
	int PickObjectByRayIntersection(XMFLOAT3& xmf3PickPosition, 
		XMFLOAT4X4& xmf4x4View, float *pfHitDistance);

};

class CRotatingObject : public CGameObject 
{
private:
	XMFLOAT3 m_xmf3RotationAxis; 
	float m_fRotationSpeed;
	
	enemyStatus status = enemyStatus::normal;
	float distance = 0.0f;
public: 
	CRotatingObject(int nMeshes=1); 
	virtual ~CRotatingObject();

	void SetRotationSpeed(float fRotationSpeed) { m_fRotationSpeed = fRotationSpeed; } 
	void SetRotationAxis(XMFLOAT3 xmf3RotationAxis) { m_xmf3RotationAxis = xmf3RotationAxis; }
	virtual void Animate(float fTimeElapsed);
	
	virtual void Reset(XMFLOAT3 pos);
	void setType(int type=0);
	virtual void setStatus(enemyStatus type) { status = type; }
	virtual void StatusWork();
	virtual enemyStatus getStatus() { return status; }
};

class CHeightMapTerrain : public CGameObject
{
public:
	CHeightMapTerrain(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList
		*pd3dCommandList, LPCTSTR pFileName, 
		int nWidth, int nLength, int nBlockWidth, int nBlockLength, XMFLOAT3 xmf3Scale, 
		XMFLOAT4 xmf4Color);
	virtual ~CHeightMapTerrain();
private:
	CHeightMapImage *m_pHeightMapImage;

	int m_nWidth;
	int m_nLength;

	XMFLOAT3 m_xmf3Scale;
public:
	//지형의 높이를 계산하는 함수이다(월드 좌표계). 높이 맵의 높이에 스케일의 y를 곱한 값이다. 
	float GetHeight(float x, float z) { 
		return(m_pHeightMapImage->
			GetHeight(x / m_xmf3Scale.x, z / m_xmf3Scale.z) * m_xmf3Scale.y);
	}

	//지형의 법선 벡터를 계산하는 함수이다(월드 좌표계). 높이 맵의 법선 벡터를 사용한다. 
	XMFLOAT3 GetNormal(float x, float z) {
		return(m_pHeightMapImage->
			GetHeightMapNormal(int(x / m_xmf3Scale.x), int(z /m_xmf3Scale.z))); 
	}

	int GetHeightMapWidth() { return(m_pHeightMapImage->GetHeightMapWidth()); }
	int GetHeightMapLength() { return(m_pHeightMapImage->GetHeightMapLength()); }
	XMFLOAT3 GetScale() { return(m_xmf3Scale); }

	//지형의 크기(가로/세로)를 반환한다. 높이 맵의 크기에 스케일을 곱한 값이다. 
	float GetWidth() { return(m_nWidth * m_xmf3Scale.x); }
	float GetLength() { return(m_nLength * m_xmf3Scale.z); }
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