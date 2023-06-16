#pragma once
#include "InterCD.h"

class ConfuCD :
	public InterCD {
public:
	ConfuCD();
	bool Initialize() override;//èâä˙âª

	void Origin_Draw(DirectXCommon* dxCommon) override;//ï`âÊ

	void BirthCD() override;
	void StayCD() override;
	void ThroughCD() override;
	void CatchCD() override;
	void ThrowCD() override;
	void DeathCD() override;
public:

private:
	void Action() override;//çsìÆ

	void ImGui_Origin() override;
private:

};