#pragma once
#include"IKEObject3d.h"
#include<DirectXMath.h>
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

	/*更新*/
	virtual void Update() = 0;

	/*描画*/
	virtual void Draw()=0;


};

