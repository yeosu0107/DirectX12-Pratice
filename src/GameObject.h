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

	//xmfloat3 ����
    CVertex(float x, float y, float z) { m_xmf3Position = XMFLOAT3(x, y, z); }

    XMFLOAT3				m_xmf3Position;  
};

class CPolygon
{
public:
	//���ؽ� ������ ������ ���� (���ؽ�)
	CPolygon(int nVertices);  
	virtual ~CPolygon();

    int						m_nVertices;     
    CVertex					*m_pVertices;   

	//�������� ���ؽ� ����(���ؽ� �ε���, ���ؽ� ���۷�����)
	void SetVertex(int nIndex, CVertex& vertex);

	//������ �׸���(����, ��ȯ���&, ī�޶� ������)
	void Draw(HDC hDCFrameBuffer, XMFLOAT4X4& xm4x4Transform, CCamera *pCamera);
};

class CMesh
{
public:
	//������ ������ �Ž� ����(������ ����)
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
	//�Ž��� ������ �߰�(������ �ε���, ������ ������)
	void SetPolygon(int nIndex, CPolygon *pPolygon);
	//�׸���(����, ��ȯ���&, ī�޶� ������)
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
	//ť�� �׸���(width, height, depth) (�⺻ 4,4,4)
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
	//�Ž��� ���¸� ����(�Ž� ������)
	CGameObject(CMesh *pMesh);

	CGameObject();
	virtual ~CGameObject();

public:
    CMesh					*m_pMesh;    //�Ž��� ���� (ť��, �� ���...)
    XMFLOAT4X4				m_xmf4x4World;     //���� ��ǥ

	BoundingOrientedBox		m_xmOOBB;   //����ǥ�迡���� �⵹����
	BoundingOrientedBox		m_xmOOBBTransformed; //������ǥ�迡���� �浹 ����
	CGameObject				*m_pCollider; 

	DWORD					m_dwColor; //��ü ����

	XMFLOAT3				m_xmf3MovingDirection; //�̵�����
	float					m_fMovingSpeed; //�̵��ӵ�
	float					m_fMovingRange; //�̵��ݰ�

	XMFLOAT3				m_xmf3RotationAxis;  //ȸ����
	float					m_fRotationSpeed;  //ȸ���ӵ�

public:
	//�Ž� ����(CMesh*)
	void SetMesh(CMesh *pMesh) { m_pMesh = pMesh; if (pMesh) pMesh->AddRef(); }
	//�÷� ����(DWORD)
	void SetColor(DWORD dwColor) { m_dwColor = dwColor; }
	//������ǥ�迡���� ��ġ(float, float, float)
	void SetPosition(float x, float y, float z);
	//������ǥ�迡���� ��ġ(XMFLOAT3&)
	void SetPosition(XMFLOAT3& xmf3Position);


	//�浹���� ����(XMFLOAT3&, XMFLOAT3&, XMFLOAT4&)
	//�߽���ǥ, �߽ɿ��� ���������� �Ÿ�, ȸ��
	void SetOOBB(XMFLOAT3& xmCenter, XMFLOAT3& xmExtents, XMFLOAT4& xmOrientation) { m_xmOOBBTransformed = m_xmOOBB = BoundingOrientedBox(xmCenter, xmExtents, xmOrientation); }

	//�̵� �ݰ� ����(XMFLOAT3&)
	void SetMovingDirection(XMFLOAT3& xmf3MovingDirection) { m_xmf3MovingDirection = Vector3::Normalize(xmf3MovingDirection); }
	//�̵� �ӵ� ����(float)
	void SetMovingSpeed(float fSpeed) { m_fMovingSpeed = fSpeed; }
	//�̵� �ݰ� ����(float)
	void SetMovingRange(float fRange) { m_fMovingRange = fRange; }

	//ȸ���� ����(XMFLOAT3&)
	void SetRotationAxis(XMFLOAT3& xmf3RotationAxis) { m_xmf3RotationAxis = Vector3::Normalize(xmf3RotationAxis); }
	//ȸ�� �ӵ� ����(float)
	void SetRotationSpeed(float fSpeed) { m_fRotationSpeed = fSpeed; }

	//x�� �̵�(float)
	void MoveStrafe(float fDistance=1.0f);
	//y�� �̵�(float)
	void MoveUp(float fDistance=1.0f);
	//z�� �̵�(float)
	void MoveForward(float fDistance=1.0f);
	//������ �������� ������ �ӵ��� �̵� (XMFLOAT3&, float)
	void Move(XMFLOAT3& vDirection, float fSpeed);

	//x,y,z������ ȸ��(float, float, float) (�⺻ 10,10,10)
	void Rotate(float fPitch=10.0f, float fYaw=10.0f, float fRoll=10.0f);
	//������ ������ ������ ������ŭ ȸ��(XMFLOAT3&, float)
	void Rotate(XMFLOAT3& xmf3Axis, float fAngle);

	XMFLOAT3 GetPosition(); //������ǥ�� ��ġ ��ȯ
	XMFLOAT3 GetLook();     //�躤�� ��ȯ
	XMFLOAT3 GetUp();       //������ ��ȯ
	XMFLOAT3 GetRight();    //������ ���� ��ȯ
	DWORD getColor() { return m_dwColor; }

	BoundingOrientedBox* getOOBB() { return &m_xmOOBBTransformed; } //Ʈ������ oobb�ּ� ��ȯ

	//�ִϸ��̼� (�̵�, ȸ��, �浹 ��...)
	virtual void Animate();
	//�׸���(����, ī�޶� ������)
	virtual void Render(HDC hDCFrameBuffer, CCamera *pCamera);

};


class Paticle : public CGameObject {
private:
public:
	Paticle() {}
	~Paticle() {}
	virtual void Animate();
};

