#pragma once
#include"DirectXCommon.h"
#include <DirectXMath.h>
using namespace DirectX;
class CharactorManager
{
public:
	CharactorManager(XMFLOAT3 StartPos = { 0.f,0.f,0.f });
	virtual ~CharactorManager(){};

protected:
	//基礎パラメータ
	XMFLOAT3 Position;
	XMFLOAT3 Rotation;
	XMFLOAT3 Scale;

public:
	//初期化
	virtual void Initialize()=0;
	//更新
	virtual void Update()=0;
	//描画
	virtual void Draw(DirectXCommon* dxCommon)=0;
};

