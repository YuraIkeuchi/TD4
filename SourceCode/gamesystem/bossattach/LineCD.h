#pragma once
#include "InterCD.h"

class LineCD :
	public InterCD {
public:
	LineCD();
	bool Initialize() override;//初期化

	void Draw(DirectXCommon* dxCommon) override;//描画
public:

private:
	void Action() override;//行動

	void ImGui_Origin() override;
private:

};