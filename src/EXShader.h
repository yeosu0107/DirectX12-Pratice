#pragma once
#include "Shader.h"

const int numOfPaticles = 100;
const int maxBullet = 100;

struct SCENE3_MIRO
{
	int layout[2];
};

class CPaticlesShader : public CInstancingShader
{
private:
	bool run = false;
	float scale = 1.0f;

	//float runtime = 0.0f;
	//float maxtime = 50.0f;
public:
	CPaticlesShader();
	virtual ~CPaticlesShader();

	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature
		*pd3dGraphicsRootSignature);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);
	virtual void AnimateObjects(float fTime);
	void setPosition(XMFLOAT3 pos);

	void setRun(XMFLOAT4& color);
	bool getRun() const { return run; }
};


class CBulletShader : public CInstancingShader
{
private:
	int nowIndex = 0;

	int nowtime = 10;
	int maxDelay = 10;

public:
	CBulletShader();
	virtual ~CBulletShader();

	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature
		*pd3dGraphicsRootSignature);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);
	virtual void AnimateObjects(float fTime);
	
	void Shoot(XMFLOAT3 pos, XMFLOAT3 look);
};

class CMazeShader : public CInstancingShader
{
private:
	SCENE3_MIRO miro[10][10] =
	{
		{ { 1, 2 },{ 1, 0 },{ 2, 0 },{ 2, 0 },{ 2, 0 },{ 2, 0 },{ 2, 0 },{ 2, 0 },{ 2, 0 },{ 2, 3 } },
		{ { 1, 0 },{ 1, 0 },{ 2, 0 },{ 1, 0 },{ 4, 0 },{ 4, 0 },{ 1, 0 },{ 4, 0 },{ 2, 0 },{ 3, 0 } },
		{ { 1, 0 },{ 2, 0 },{ 4, 0 },{ 4, 0 },{ 4, 0 },{ 3, 0 },{ 2, 0 },{ 1, 0 },{ 1, 0 },{ 3, 0 } },
		{ { 1, 0 },{ 2, 0 },{ 1, 0 },{ 2, 0 },{ 2, 0 },{ 3, 0 },{ 3, 0 },{ 1, 0 },{ 2, 0 },{ 3, 0 } },
		{ { 1, 2 },{ 1, 0 },{ 1, 0 },{ 1, 0 },{ 4, 0 },{ 3, 0 },{ 3, 0 },{ 1, 0 },{ 4, 0 },{ 3, 0 } },
		{ { 1, 0 },{ 1, 0 },{ 4, 0 },{ 1, 0 },{ 1, 0 },{ 4, 0 },{ 3, 0 },{ 1, 0 },{ 1, 0 },{ 3, 0 } },
		{ { 1, 0 },{ 4, 0 },{ 4, 0 },{ 4, 0 },{ 4, 0 },{ 4, 0 },{ 4, 0 },{ 0, 0 },{ 3, 0 },{ 3, 0 } },
		{ { 1, 0 },{ 3, 0 },{ 3, 0 },{ 4, 0 },{ 4, 0 },{ 3, 0 },{ 3, 0 },{ 1, 0 },{ 2, 0 },{ 3, 0 } },
		{ { 1, 0 },{ 4, 0 },{ 4, 0 },{ 4, 0 },{ 3, 0 },{ 3, 0 },{ 3, 0 },{ 1, 0 },{ 3, 0 },{ 3, 0 } },
		{ { 1, 4 },{ 4, 0 },{ 4, 0 },{ 4, 0 },{ 4, 0 },{ 4, 0 },{ 4, 0 },{ 4, 0 },{ 4, 0 },{ 3, 0 } }
	};
public:
	CMazeShader();
	virtual ~CMazeShader();

	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature
		*pd3dGraphicsRootSignature);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void* pContext);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);
	virtual void AnimateObjects(float fTime);

	virtual CGameObject *PickObjectByRayIntersection(XMFLOAT3& xmf3PickPosition,
		XMFLOAT4X4& xmf4x4View, float *pfNearHitDistance) {
		return nullptr;
	}
};

struct pos
{
	float xPos;
	float zPos;
};

class CEnemyShader : public CInstancingShader{
private:
	pos EnemyPos[15] = {
		{629.0f, 656.0f}, { 627.0f, 875.0f }, { 562.0f, 741.0f },
		{477.0f, 1025.0f},{ 294.0f, 994.0f }, { 388.0f,	882.0f },
		{294.0f, 751.0f}, { 443.0f,	570.0f }, { 409.0f,	1051.0f},
		{123.0f, 565.0f}, { 176.0f,	1079.0f}, { 144.0f, 747.0f },
		{181.0f, 811.0f}, { 141.0f,	894.0f }, { 110.0f,	967.0f }
	};
public:
	CEnemyShader();
	virtual ~CEnemyShader();
	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature
		*pd3dGraphicsRootSignature);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void* pContext);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);
	virtual void AnimateObjects(float fTime);
};