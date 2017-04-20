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

	//xmfloat3 생성
    CVertex(float x, float y, float z) { m_xmf3Position = XMFLOAT3(x, y, z); }

    XMFLOAT3				m_xmf3Position;  
};

class CPolygon
{
public:
	//버텍스 갯수의 폴리곤 생성 (버텍스)
	CPolygon(int nVertices);  
	virtual ~CPolygon();

    int						m_nVertices;     
    CVertex					*m_pVertices;   

	//폴리곤의 버텍스 설정(버텍스 인덱스, 버텍스 레퍼런스값)
	void SetVertex(int nIndex, CVertex& vertex);

	//폴리곤 그리기(버퍼, 변환행렬&, 카메라 포인터)
	void Draw(HDC hDCFrameBuffer, XMFLOAT4X4& xm4x4Transform, CCamera *pCamera);
};

class CMesh
{
public:
	//폴리곤 갯수의 매쉬 생성(폴리곤 갯수)
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
	//매쉬에 폴리곤 추가(폴리곤 인덱스, 폴리곤 포인터)
	void SetPolygon(int nIndex, CPolygon *pPolygon);
	//그리기(버퍼, 변환행렬&, 카메라 포인터)
	virtual void Render(HDC hDCFrameBuffer, XMFLOAT4X4& xm4x4Transform, CCamera *pCamera);
};

class CBox : public CMesh
{
public:
	CBox(float max);
	virtual ~CBox();

	void setBox(float end);
};

class CCubeMesh : public CMesh
{
public:
	//큐브 그리기(width, height, depth) (기본 4,4,4)
	CCubeMesh(float fWidth = 4.0f, float fHeight = 4.0f, float fDepth = 4.0f);
	virtual ~CCubeMesh();
};

class CWallTileMesh : public CMesh {
public:
	CWallTileMesh(float fWidth, float fHeight, float fDepth);
	virtual ~CWallTileMesh();
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
	//매쉬의 형태를 결정(매쉬 포인터)
	CGameObject(CMesh *pMesh);

	CGameObject();
	virtual ~CGameObject();

public:
    CMesh					*m_pMesh;    //매쉬의 형태 (큐브, 벽 등등...)
    XMFLOAT4X4				m_xmf4x4World;     //월드 좌표

	BoundingOrientedBox		m_xmOOBB;   //모델좌표계에서의 출돌영역
	BoundingOrientedBox		m_xmOOBBTransformed; //월드좌표계에서의 충돌 영역
	CGameObject				*m_pCollider; 

	DWORD					m_dwColor; //객체 색깔

	XMFLOAT3				m_xmf3MovingDirection; //이동방향
	float					m_fMovingSpeed; //이동속도
	float					m_fMovingRange; //이동반경

	XMFLOAT3				m_xmf3RotationAxis;  //회전축
	float					m_fRotationSpeed;  //회전속도

public:
	//매쉬 설정(CMesh*)
	void SetMesh(CMesh *pMesh) { m_pMesh = pMesh; if (pMesh) pMesh->AddRef(); }
	//컬러 설정(DWORD)
	void SetColor(DWORD dwColor) { m_dwColor = dwColor; }
	//월드좌표계에서의 위치(float, float, float)
	void SetPosition(float x, float y, float z);
	//월드좌표계에서의 위치(XMFLOAT3&)
	void SetPosition(XMFLOAT3& xmf3Position);


	//충돌영역 설정(XMFLOAT3&, XMFLOAT3&, XMFLOAT4&)
	//중심좌표, 중심에서 끝점까지의 거리, 회전
	void SetOOBB(XMFLOAT3& xmCenter, XMFLOAT3& xmExtents, XMFLOAT4& xmOrientation) { m_xmOOBBTransformed = m_xmOOBB = BoundingOrientedBox(xmCenter, xmExtents, xmOrientation); }

	//이동 반경 설정(XMFLOAT3&)
	void SetMovingDirection(XMFLOAT3& xmf3MovingDirection) { m_xmf3MovingDirection = Vector3::Normalize(xmf3MovingDirection); }
	//이동 속도 설정(float)
	void SetMovingSpeed(float fSpeed) { m_fMovingSpeed = fSpeed; }
	//이동 반경 설정(float)
	void SetMovingRange(float fRange) { m_fMovingRange = fRange; }

	//회전축 설정(XMFLOAT3&)
	void SetRotationAxis(XMFLOAT3& xmf3RotationAxis) { m_xmf3RotationAxis = Vector3::Normalize(xmf3RotationAxis); }
	//회전 속도 설정(float)
	void SetRotationSpeed(float fSpeed) { m_fRotationSpeed = fSpeed; }

	//x축 이동(float)
	void MoveStrafe(float fDistance=1.0f);
	//y축 이동(float)
	void MoveUp(float fDistance=1.0f);
	//z축 이동(float)
	void MoveForward(float fDistance=1.0f);
	//임의의 방향으로 임의의 속도로 이동 (XMFLOAT3&, float)
	void Move(XMFLOAT3& vDirection, float fSpeed);

	//x,y,z축으로 회전(float, float, float) (기본 10,10,10)
	void Rotate(float fPitch=10.0f, float fYaw=10.0f, float fRoll=10.0f);
	//임의의 축으로 임의의 각도만큼 회전(XMFLOAT3&, float)
	void Rotate(XMFLOAT3& xmf3Axis, float fAngle);

	XMFLOAT3 GetPosition(); //월드좌표계 위치 반환
	XMFLOAT3 GetLook();     //룩벡터 반환
	XMFLOAT3 GetUp();       //업벡터 반환
	XMFLOAT3 GetRight();    //오른쪽 벡터 반환
	DWORD getColor() { return m_dwColor; }

	BoundingOrientedBox* getOOBB() { return &m_xmOOBBTransformed; } //트랜스폼 oobb주소 반환

	//애니메이션 (이동, 회전, 충돌 등...)
	virtual void Animate();
	//그리기(버퍼, 카메라 포인터)
	virtual void Render(HDC hDCFrameBuffer, CCamera *pCamera);

};


class Paticle : public CGameObject {
private:
public:
	Paticle() {}
	~Paticle() {}
	virtual void Animate();
};

