#pragma once
#include "InterCD.h"

class LineCD :
	public InterCD {
public:
	LineCD();
	bool Initialize() override;//������

	void Origin_Draw(DirectXCommon* dxCommon) override;//�`��

	void BirthCD() override;
	void StayCD() override;
	void CatchCD() override;
	void ThrowCD() override;
	void DeathCD() override;
public:

private:
	void Action() override;//�s��

	void ImGui_Origin() override;
private:

};