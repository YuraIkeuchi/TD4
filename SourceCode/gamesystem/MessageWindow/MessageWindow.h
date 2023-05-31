﻿#pragma once
#include"IKESprite.h"
#include"VariableCommon.h"

class MessageWindow
{

public:
	void Initialize();

	void characterColor();

	void Update(const XMFLOAT4& girlcolor , const XMFLOAT4& sutoponcolor );

	void Draw();

	void Display();

	void DisplayCharacter(XMFLOAT4& color);

	bool DisplayCheck();
private:
	unique_ptr<IKESprite> window_;
	unique_ptr<IKESprite> blackback_;
	unique_ptr<IKESprite> girl_;
	unique_ptr<IKESprite> sutopon_;
	//��W
	XMFLOAT2 window_pos{ WinApp::window_width / 2.f,WinApp::window_height + 100 };
	XMFLOAT2 window_size{ 0.f,0.f };
	XMFLOAT4 black_color{ 1.f,1.f,1.f,0.f };
	XMFLOAT4 girl_color{ 1.5f,1.5f,1.5f,0.f };
	XMFLOAT4 sutopon_color{ 1.f,1.f,1.f,0.f };

	float maxframe = 20.f;
	float nowframe = 0.f;
	float frame = 0.f;

	float maxcolorframe = 20.f;
	float nowcolorframe = 0.f;
	float colorframe = 0.f;
};

