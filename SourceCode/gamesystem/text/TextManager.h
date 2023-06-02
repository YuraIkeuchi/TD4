#pragma once
#include"DirectXCommon.h"
#include"VariableCommon.h"
#include"Font.h"
class TextManager
{
private:
	struct Word {
		wchar_t* FirstWord;//ˆês–Ú
		wchar_t* SecondWord;//“ñs–Ú
		wchar_t* ThirdWord;//ŽOs–Ú
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
	};

	enum Name_First
	{
		VIEWBOSS,
		SPEAKPLAYER1,
		SPEALPLAYER2
	};

	enum Name_Second {
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
	void SetConversation(Name_First name = VIEWBOSS);

	void SetSecondConversation(Name_Second name = ANGER_TALK);
private:
	
	//
	void CreateWord(Name name, wchar_t* tex1, wchar_t* tex2 = L" ", wchar_t* tex3 = L" ");
	void CreateWord(Name_First name, wchar_t* tex1, wchar_t* tex2 = L" ", wchar_t* tex3 = L" ");
	void SecondCreateWord(Name_Second name, wchar_t* tex1, wchar_t* tex2 = L" ", wchar_t* tex3 = L" ");

	//
	Word SetWord(wchar_t* tex1, wchar_t* tex2, wchar_t* tex3);
	//
	Conversation CreateConversation(Word word);

	void CreateCon(Conversation con, Word word);
private:
	std::map<TextManager::Name, Word> wordlist_;
	std::map<TextManager::Name_First, Word> wordlist_first;
	std::map<TextManager::Name_Second, Word> wordlist_second;
	Conversation conversation_ = {};

	Conversation bossconversation_ = {};

	XMVECTOR color_{ 1.f,1.f,1.f,1.f };
};

