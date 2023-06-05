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
		TYUTORIAL_TALK7,
		TYUTORIAL_TALK8,
		TYUTORIAL_TALK9,
		TYUTORIAL_TALK10,
		TYUTORIAL_TALK11,
		TYUTORIAL_TALK12,
		TYUTORIAL_TALK13,
		TYUTORIAL_TALK14,
		TYUTORIAL_TALK15,
		TYUTORIAL_TALK16,
		TYUTORIAL_TALK17,
		TYUTORIAL_TALK18,
		TYUTORIAL_TALK19,
		TYUTORIAL_TALK20,
		TYUTORIAL_TALK21,
		TYUTORIAL_TALK22,
		TYUTORIAL_TALK23,
		TYUTORIAL_TALK24,
		TYUTORIAL_TALK25,
		TYUTORIAL_TALK26,
		TYUTORIAL_TALK27,
		TYUTORIAL_TALK28,
		TYUTORIAL_TALK29,
		TYUTORIAL_TALK30,
		TYUTORIAL_TALK31,
		TYUTORIAL_TALK32,
		TYUTORIAL_TALK33,
		AISATU,
		SETUMEI1,
		SETUMEI2,
		SETUMEI3,
		SETUMEI4,
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

	void SetConversation(Name name=NONE,const XMVECTOR& color={1.f,1.f,1.f,1.f});

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

