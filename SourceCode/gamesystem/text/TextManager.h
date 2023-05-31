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
		NONE = 0,
		TYUTORIAL_TALK1,
		TYUTORIAL_TALK2,
		TYUTORIAL_TALK3,
		TYUTORIAL_TALK4,
		TYUTORIAL_TALK5,
		TYUTORIAL_TALK6,
		AISATU ,
		KAIWA,
		BATTLE,
		ANGER_TALK,
		ANGER_TALK2,
		JOY_TALK,
		JOY_TALK2,
		JOY_TALK3,
		SELECT_TALK,
		SELECT_ANGER,
		SELECT_ANGER2,
		SELECT_JOY,
		SELECT_JOY2,
	};

	static TextManager* GetInstance();

	//
	void Create(DirectXCommon* dxcomon);

	void Initialize(DirectXCommon* dxcomon);

	void Draw(DirectXCommon* dxcommon);

	void SetAllColor(const XMVECTOR& color={1.f,1.f,1.f,1.f});

	void SetOnceColor(int row, const XMVECTOR& color = { 1.f,1.f,1.f,1.f });

	void SetConversation(Name name=NONE);


	void NoneText();
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

