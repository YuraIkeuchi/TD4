#pragma once
#include "InterCD.h"

class DebuffCD :
	public InterCD {
public:
	DebuffCD();
	bool Initialize() override;//‰Šú‰»

	void Origin_Draw(DirectXCommon* dxCommon) override;//•`‰æ
public:

private:
	void Action() override;//s“®

	void ImGui_Origin() override;
private:

};