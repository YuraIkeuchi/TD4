#pragma once
#include "InterCD.h"

class BarrangeCD :
	public InterCD {
public:
	BarrangeCD();
	bool Initialize() override;//������
	
	void Origin_Draw(DirectXCommon* dxCommon) override;//�`��
public:

private:
	void Action() override;//�s��
	
	void ImGui_Origin() override;
private:

};