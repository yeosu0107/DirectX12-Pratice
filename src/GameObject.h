#pragma once
#include "Mesh.h"
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

	void ReleaseUploadBuffers();
	virtual void SetMesh(CMesh *pMesh);
	virtual void SetShader(CShader *pShader);
	virtual void Animate(float fTimeElapsed);
	virtual void OnPrepareRender(); 
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList);

};

