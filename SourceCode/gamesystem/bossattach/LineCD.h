#pragma once
#include "InterCD.h"

class LineCD :
	public InterCD {
public:
	LineCD();
	bool Initialize() override;//初期化

	void Origin_Draw(DirectXCommon* dxCommon) override;//描画

	void BirthCD() override;
	void StayCD() override;
	void ThroughCD() override;
	void CatchCD() override;
	void ThrowCD() override;
	void DeathCD() override;
	void ResPornCD() override;
public:

private:
	void Action() override;//行動

	void ImGui_Origin() override;
private:

};