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
		AISATU = 0,
		KAIWA = 1,
		BATTLE = 2,
	};

	static TextManager* GetInstance();

	void Initialize();


	void Draw(DirectXCommon* dxcommon);

	void SetConversation(Name name);
private:
	//
	void CreateWord(Name name, wchar_t* tex1, wchar_t* tex2 = L" ", wchar_t* tex3 = L" ");
	//
	Word SetWord(wchar_t* tex1, wchar_t* tex2, wchar_t* tex3);
	//
	Conversation CreateConversation(Word word);
private:
	std::map<TextManager::Name, Word> wordlist_;

	Conversation conversation_ = {};
};

