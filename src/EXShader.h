#pragma once
#include "Shader.h"

const int numOfPaticles = 100;
const int maxBullet = 100;

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

	void setRun();
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