#pragma once
#include"IKEObject3d.h"
#include<DirectXMath.h>
#include "DirectXCommon.h"
#include<memory>

using namespace DirectX;

class LoadManager
{
public:

protected:
	std::vector<XMFLOAT3>Pos;
	std::vector<XMFLOAT3>Rot;
	std::vector<XMFLOAT3>Scl;

public:
	/*初期化*/
	virtual void Initialize() = 0;

	/*最初のステージ更新*/
	virtual void FirstUpdate() = 0;

	/*2個目のステージ更新*/
	virtual void SecondUpdate() = 0;

	/*描画*/
	virtual void Draw(DirectXCommon* dxCommon)=0;


};

