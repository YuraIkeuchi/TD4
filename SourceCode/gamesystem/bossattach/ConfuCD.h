#pragma once
#include "InterCD.h"

class ConfuCD :
	public InterCD {
public:
	ConfuCD();
	bool Initialize() override;//初期化

	void Origin_Draw(DirectXCommon* dxCommon) override;//描画

	void BirthCD() override;
	void StayCD() override;
	void CatchCD() override;
	void DeathCD() override;
	void ResPornCD() override;
public:

private:
	void Action() override;//行動

	void AudioAction() override;//行動

	void ImGui_Origin() override;
private:

};