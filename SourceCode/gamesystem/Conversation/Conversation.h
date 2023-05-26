#pragma once
#include"Text.h"
#include"Input.h"
#include"IKESprite.h"
#include"VariableCommon.h"
#include"TextManager.h"

class Conversation
{
public:
	static Conversation* GetInstance();
private:

public:

	enum class CharaText
	{
		GIRL,
		PLAYER,
		SUTOPON
	};
	void SetChara(CharaText chara) { m_Chara = chara; }
	void SetText(wchar_t* text, CharaText chara,int index);
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

	struct CharaState
	{
		UINT index;
		Font* font[3];
	};

	std::vector<CharaState>textList;
	//CharaState GetChara(){return}
	CharaText m_Chara;

public:
	enum ChatName
	{
		AISATU = 0,
		KAIWA = 1,
		BATTLE = 2,
	};
	
	void CreateText(DirectXCommon* comn, Conversation::ChatName num, wchar_t* text[3]);
	void TextUpdate(XMFLOAT2 *pos,XMFLOAT3 *color);
	void Draw(DirectXCommon* comn, Conversation::ChatName num);


	std::vector<Font*>FontList;

public:



};

