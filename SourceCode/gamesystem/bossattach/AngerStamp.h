#pragma once
#include "InterStamp.h"

class AngerStamp : public InterStamp {
public:
	AngerStamp();
private:
	bool Initialize(const XMFLOAT3& pos) override;//初期化
	void Action() override;//行動
	void ImGui_Origin() override;//ImGui
	void Origin_Draw(DirectXCommon* dxCommon) override;//

	void BirthParticle();	//パーティクルの生成

	bool Collide();	//当たり判定

private:
	float m_DamagePower = {};
};