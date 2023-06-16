#pragma once
#include "InterCD.h"

class ConfuCD :
	public InterCD {
public:
	ConfuCD();
	bool Initialize() override;//‰Šú‰»

	void Draw(DirectXCommon* dxCommon) override;//•`‰æ
public:

private:
	void Action() override;//s“®

	void ImGui_Origin() override;
private:

};