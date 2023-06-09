#pragma once
#include "InterStamp.h"

class JoyStamp : public InterStamp {
public:
	JoyStamp();
private:
	bool Initialize(const XMFLOAT3& pos) override;//初期化
	void Action() override;//行動
	void ImGui_Origin() override;//ImGui
	void Origin_Draw(DirectXCommon* dxCommon) override;//
};