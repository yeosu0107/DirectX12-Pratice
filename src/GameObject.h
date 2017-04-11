#pragma once

#define DIR_FORWARD			0x01
#define DIR_BACKWARD		0x02
#define DIR_LEFT			0x04
#define DIR_RIGHT			0x08
#define DIR_UP				0x10
#define DIR_DOWN			0x20

#define DegreeToRadian(x)	((x)*3.1415926535/180.0)

struct VIEWPORT
{
	int						m_xStart;
	int						m_yStart;
	int						m_nWidth;
	int						m_nHeight;
};

class CCamera;

class CVertex
{
public:
	CVertex() { }
    CVertex(float x, float y, float z) { m_xmf3Position = XMFLOAT3(x, y, z); }

    XMFLOAT3				m_xmf3Position;  
};

class CPolygon
{
public:
	CPolygon(int nVertices);
	virtual ~CPolygon();

    int						m_nVertices;     
    CVertex					*m_pVertices;   

	void SetVertex(int nIndex, CVertex& vertex);
	void Draw(HDC hDCFrameBuffer, XMFLOAT4X4& xm4x4Transform, CCamera *pCamera);
};

class CMesh
{
public:
	CMesh(int nPolygons);
	virtual ~CMesh();

private:
	int						m_nReferences;

    int						m_nPolygons;     
    CPolygon				**m_ppPolygons;   

public:
	void AddRef() { m_nReferences++; }
	void Release() { m_nReferences--; if (m_nReferences <= 0) delete this; }

public:
	void SetPolygon(int nIndex, CPolygon *pPolygon);
	virtual void Render(HDC hDCFrameBuffer, XMFLOAT4X4& xm4x4Transform, CCamera *pCamera);
};

class CCubeMesh : public CMesh
{
public:
	CCubeMesh(float fWidth = 4.0f, float fHeight = 4.0f, float fDepth = 4.0f);
	virtual ~CCubeMesh();
};

class CWallMesh : public CMesh
{
public:
	CWallMesh(float fWidth = 4.0f, float fHeight = 4.0f, float fDepth = 4.0f);
	virtual ~CWallMesh();
};

class CAirplaneMesh : public CMesh
{
public:
	CAirplaneMesh(float fWidth = 20.0f, float fHeight = 20.0f, float fDepth = 4.0f);
	virtual ~CAirplaneMesh();
};

class CGameObject
{
public:
	CGameObject(CMesh *pMesh);
	CGameObject();
	virtual ~CGameObject();

public:
    CMesh					*m_pMesh; 
    XMFLOAT4X4				m_xmf4x4World;    

	BoundingOrientedBox		m_xmOOBB;
	BoundingOrientedBox		m_xmOOBBTransformed;
	CGameObject				*m_pCollider;

	DWORD					m_dwColor;

	XMFLOAT3				m_xmf3MovingDirection;
	float					m_fMovingSpeed;
	float					m_fMovingRange;

	XMFLOAT3				m_xmf3RotationAxis;
	float					m_fRotationSpeed;

public:
	void SetMesh(CMesh *pMesh) { m_pMesh = pMesh; if (pMesh) pMesh->AddRef(); }
	void SetColor(DWORD dwColor) { m_dwColor = dwColor; }
	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3& xmf3Position);

	void SetOOBB(XMFLOAT3& xmCenter, XMFLOAT3& xmExtents, XMFLOAT4& xmOrientation) { m_xmOOBBTransformed = m_xmOOBB = BoundingOrientedBox(xmCenter, xmExtents, xmOrientation); }

	void SetMovingDirection(XMFLOAT3& xmf3MovingDirection) { m_xmf3MovingDirection = Vector3::Normalize(xmf3MovingDirection); }
	void SetMovingSpeed(float fSpeed) { m_fMovingSpeed = fSpeed; }
	void SetMovingRange(float fRange) { m_fMovingRange = fRange; }

	void SetRotationAxis(XMFLOAT3& xmf3RotationAxis) { m_xmf3RotationAxis = Vector3::Normalize(xmf3RotationAxis); }
	void SetRotationSpeed(float fSpeed) { m_fRotationSpeed = fSpeed; }

	void MoveStrafe(float fDistance=1.0f);
	void MoveUp(float fDistance=1.0f);
	void MoveForward(float fDistance=1.0f);
	void Move(XMFLOAT3& vDirection, float fSpeed);

	void Rotate(float fPitch=10.0f, float fYaw=10.0f, float fRoll=10.0f);
	void Rotate(XMFLOAT3& xmf3Axis, float fAngle);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetLook();
	XMFLOAT3 GetUp();
	XMFLOAT3 GetRight();

	virtual void Animate();
	virtual void Render(HDC hDCFrameBuffer, CCamera *pCamera);
};

class CPlayer : public CGameObject
{
public:
	CPlayer();
	virtual ~CPlayer();

	XMFLOAT3					m_xmf3Position;
	XMFLOAT3					m_xmf3Right;
	XMFLOAT3					m_xmf3Up;
	XMFLOAT3					m_xmf3Look;

	XMFLOAT3					m_xmf3CameraOffset;
	XMFLOAT3					m_xmf3Velocity;
	float						m_fFriction;

	float           			m_fPitch;
	float           			m_fYaw;
	float           			m_fRoll;

	CCamera						*m_pCamera;

	void SetPosition(float x, float y, float z);
	void Move(DWORD dwDirection, float fDistance);
	void Move(XMFLOAT3& xmf3Shift, bool bUpdateVelocity);
	void Move(float x, float y, float z);
	void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);
	void SetCameraOffset(XMFLOAT3& xmf3CameraOffset);
	void Update(float fTimeElapsed=0.016f);

	virtual void Render(HDC hDCFrameBuffer, CCamera *pCamera);
};

class CCamera
{
public:
	CCamera();
	virtual ~CCamera();

	XMFLOAT3				m_xmf3Position;
	XMFLOAT3				m_xmf3Right;
	XMFLOAT3				m_xmf3Up;
	XMFLOAT3				m_xmf3Look;

	XMFLOAT4X4				m_xmf4x4View;
	XMFLOAT4X4				m_xmf4x4Projection;
	XMFLOAT4X4				m_xmf4x4ViewProject;

	VIEWPORT				m_Viewport;

	void GenerateViewMatrix();
	void GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fFOVAngle);
	void SetViewport(int xStart, int yStart, int nWidth, int nHeight);

	void SetLookAt(XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up);
	void SetLookAt(XMFLOAT3& vPosition, XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up);

	void Move(XMFLOAT3& xmf3Shift);
	void Move(float x, float y, float z);
	void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);
	void Update(CPlayer *pPlayer, XMFLOAT3& xmf3LookAt, float fTimeElapsed = 0.016f);
};

