#pragma once
#include"Text.h"
#include"Input.h"
#include"IKESprite.h"
#include"VariableCommon.h"

class Conversation
{
private:

public:
	void Initialize(DirectXCommon* dxcommon);

	void Update();

	void SproteDraw();

	void FontDraw(DirectXCommon* dxcommon);

	void Tyutorial();

	void WardNone();
	
private:
	//クラス
	Input* input = Input::GetInstance();

	unique_ptr<Text> playertext_;
	unique_ptr<Text>sutopontext_;
	unique_ptr<IKESprite> conversationwindow;
	unique_ptr<IKESprite> blackwindow;
	unique_ptr<IKESprite> girl;
	unique_ptr<IKESprite> megahon;
	//��W
	XMFLOAT2 window_pos{ WinApp::window_width / 2.f,WinApp::window_height + 100 };
	XMFLOAT2 window_size{ 0.f,0.f };
	XMFLOAT4 black_color{ 1.f,1.f,1.f,0.f };
	XMFLOAT4 girl_color{ 1.5f,1.5f,1.5f,0.f };
	XMFLOAT4 sutopon_color{ 1.f,1.f,1.f,0.f };

	float maxframe = 20.f;
	float nowframe = 0.f;
	float frame = 0.f;
	wchar_t* playerward_[3];
	wchar_t* sutoponward_[3];
	int len[3];
	int conversation = 0;
};

