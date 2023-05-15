#pragma once
#include "InterStamp.h"

class AngerStamp : public InterStamp {
public:
	AngerStamp();
private:
	bool Initialize(const XMFLOAT3& pos) override;//������
	void Action() override;//�s��
	void ImGui_Origin() override;//ImGui
	void Origin_Draw(DirectXCommon* dxCommon) override;//

	void BirthParticle();
};