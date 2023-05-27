#include "TextManager.h"

TextManager* TextManager::GetInstance()
{
	static TextManager instance;
	return &instance;
}

void TextManager::WordLoad(DirectXCommon* dxcommon)
{
	wchar_t* hello[3] = { L"Hello",L"World",L"aa" };
	Conversation::GetInstance()->CreateText(dxcommon,Conversation::AISATU, hello);
	wchar_t* aisatu[3] = {L"Hello",L"World",L"aa"};
	Conversation::GetInstance()->CreateText(dxcommon, Conversation::KAIWA, aisatu);
}

void TextManager::WordSet()
{
	Word w;
	WordSet_One(w, L"Hello", L"World", L"aa");
	wordlist_.insert(std::make_pair(AISATU, w));



}

void TextManager::WordSet_One(Word& w, wchar_t* tex1, wchar_t* tex2, wchar_t* tex3) {
	w.word[0] = tex1;
	w.word[1] = tex2;
	w.word[2] = tex3;
}
