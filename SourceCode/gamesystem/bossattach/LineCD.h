#pragma once
#include "InterCD.h"

class LineCD :
	public InterCD {
public:
	LineCD();
	bool Initialize() override;//������

	void Draw(DirectXCommon* dxCommon) override;//�`��
public:

private:
	void Action() override;//�s��

	void ImGui_Origin() override;
private:

};