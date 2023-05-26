#pragma once
#include"Conversation.h"
#include"DirectXCommon.h"
class TextManager
{
	struct Word {
		wchar_t* word[3];
	};
public:
	enum Name
	{
		AISATU = 0,
		KAIWA = 1,
		BATTLE = 2,
	};

	static TextManager* GetInstance();

	void WordLoad(DirectXCommon* dxcommon);

	void WordSet();

	void WordSet_One(Word& w,wchar_t*tex1, wchar_t* tex2, wchar_t* tex3 );
private:
	wchar_t* word[3];
	std::map<Name, Word> wordlist_;
};

