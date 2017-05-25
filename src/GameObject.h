#pragma once
#include "Mesh.h"
#include "Camera.h"
class CShader;

class CGameObject
{
private: 
	int m_nReferences = 0;

protected:
	XMFLOAT4X4 m_xmf4x4World; 
	CMesh *m_pMesh = NULL;
	CShader *m_pShader = NULL;

public:
	CGameObject();
	virtual ~CGameObject();

	void AddRef() { m_nReferences++; } 
	void Release() { if (--m_nReferences <= 0) delete this; }

	void Rotate(XMFLOAT3 *pxmf3Axis, float fAngle);


	void ReleaseUploadBuffers();
	virtual void SetMesh(CMesh *pMesh);
	virtual void SetShader(CShader *pShader);
	virtual void Animate(float fTimeElapsed);
	virtual void OnPrepareRender(); 
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);

	XMFLOAT4X4& getMatrix() { return m_xmf4x4World; }
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
