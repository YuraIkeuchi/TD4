#pragma once
#include "InterCD.h"

class BarrangeCD :
	public InterCD {
public:
	BarrangeCD();
	bool Initialize() override;//初期化
	
	void Origin_Draw(DirectXCommon* dxCommon) override;//描画
public:

private:
	void Action() override;//行動
	
	void ImGui_Origin() override;
private:

};