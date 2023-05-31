#pragma once
#include"DirectXCommon.h"
#include"VariableCommon.h"
#include"Font.h"
class TextManager
{
private:
	struct Word {
		wchar_t* FirstWord;//一行目
		wchar_t* SecondWord;//二行目
		wchar_t* ThirdWord;//三行目
	};
	struct Conversation
	{
		Font* FirstFont;
		Font* SecondFont;
		Font* ThirdFont;
	};

public:
	enum Name
	{
		NONE=0,
		AISATU =1,
		KAIWA =2,
		BATTLE=3,
		ANGER_TALK=4,
		ANGER_TALK2=5,
		JOY_TALK = 6,
		JOY_TALK2 = 7,
		JOY_TALK3 = 8,
		SELECT_TALK = 9,
		SELECT_ANGER = 10,
		SELECT_ANGER2 = 11,
		SELECT_JOY = 12,
		SELECT_JOY2 = 13,
	};

	static TextManager* GetInstance();

	//
	void Create(DirectXCommon* dxcomon);

	void Initialize(DirectXCommon* dxcomon);


	void Draw(DirectXCommon* dxcommon);

	void SetColor(const XMVECTOR& color={1.f,1.f,1.f,1.f});

	void SetConversation(Name name=NONE);
private:
	
	//
	void CreateWord(Name name, wchar_t* tex1, wchar_t* tex2 = L" ", wchar_t* tex3 = L" ");
	//
	Word SetWord(wchar_t* tex1, wchar_t* tex2, wchar_t* tex3);
	//
	Conversation CreateConversation(Word word);

	void CreateCon(Conversation con, Word word);
private:
	std::map<TextManager::Name, Word> wordlist_;

	Conversation conversation_ = {};

	Conversation bossconversation_ = {};

	XMVECTOR color_{ 1.f,1.f,1.f,1.f };
};

