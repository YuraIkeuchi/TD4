#pragma once
#include "InterCD.h"

class ConfuCD :
	public InterCD {
public:
	ConfuCD();
	bool Initialize() override;//������

	void Origin_Draw(DirectXCommon* dxCommon) override;//�`��
public:

private:
	void Action() override;//�s��

	void ImGui_Origin() override;
private:

};