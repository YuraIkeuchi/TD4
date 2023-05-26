#include "TextManager.h"

TextManager* TextManager::GetInstance()
{
	static TextManager instance;
	return &instance;
}



void TextManager::Initialize()
{
	//ワード追加
	CreateWord(AISATU,L"Hello",L"World",L"GoodBye");



	//コンヴァージョン初期化

	//フォントのあれこれ

}

void TextManager::Draw(DirectXCommon* dxcommon)
{
	//コンヴァージョン.フォントドローする
}


void TextManager::SetConversation(Name name)
{
	std::map<TextManager::Name, Word>::iterator itr = wordlist_.find(name);

	conversation_ = CreateConversation(itr->second);
}


void TextManager::CreateWord(Name name, wchar_t* tex1, wchar_t* tex2, wchar_t* tex3)
{
	Word temp = SetWord(tex1, tex2, tex3);
	wordlist_.insert(std::make_pair(name, temp));
}

TextManager::Word TextManager::SetWord(wchar_t* tex1, wchar_t* tex2, wchar_t* tex3)
{
	Word word = {};
	word.FirstWord = tex1;
	word.SecondWord = tex2;
	word.ThirdWord = tex3;
	return word;
}

TextManager::Conversation TextManager::CreateConversation(Word word)
{
	Conversation temp = {};
	temp.FirstFont->SetString(word.FirstWord);
	temp.SecondFont->SetString(word.SecondWord);
	temp.ThirdFont->SetString(word.ThirdWord);
	return temp;
}
