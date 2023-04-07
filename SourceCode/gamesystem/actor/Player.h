#pragma once
#include"DirectXCommon.h"
#include <DirectXMath.h>
#include"IKEFBXObject3d.h"
#include<memory>
using namespace DirectX;
class Player
{
public:
	Player();
	~Player();

private:
	//基礎パラメータ
	XMFLOAT3 Position;
	XMFLOAT3 Rotation;
	XMFLOAT3 Scale;

	bool m_LoopFlag = true;
	int m_AnimationSpeed = 1;
	bool m_StopFlag=false;

private:
	std::unique_ptr<IKEFBXObject3d>Model;
public:
	void Init();
	void Upda();
	void Draw(DirectXCommon* dxCommon);

private:
	//各アニメーション
	enum class AnimeName
	{
		IDLE=7,
		ATTACK=0
	}_animeName;

	void AnimationControl(AnimeName name, const bool& loop, int speed);
};

