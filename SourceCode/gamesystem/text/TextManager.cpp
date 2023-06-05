#include "TextManager.h"

const XMFLOAT2 kFirstRowPos{ 5.f,0.f };
const XMFLOAT2 kSecondRowPos{ 5.f,-40.f };
const XMFLOAT2 kThirdRowPos{ 5.f, -80.f };


TextManager* TextManager::GetInstance()
{
	static TextManager instance;
	return &instance;
}

void TextManager::Create(DirectXCommon* dxcomon)
{
	conversation_.FirstFont = new Font();
	conversation_.SecondFont = new Font();
	conversation_.ThirdFont = new Font();

	conversation_.FirstFont->LoadFont(dxcomon);
	conversation_.SecondFont->LoadFont(dxcomon);
	conversation_.ThirdFont->LoadFont(dxcomon);

	conversation_.FirstFont->SetColor(color_);
	conversation_.SecondFont->SetColor(color_);
	conversation_.ThirdFont->SetColor(color_);

}


//初期化
void TextManager::Initialize(DirectXCommon* dxcomon)
{
	//ワード追加
	CreateWord(NONE, L" ", L" ", L" ");
	CreateWord(TYUTORIAL_TALK1, L"ここはどこだろう?", L"ん?なにか動いてる", L"");
	CreateWord(SETUMEI1, L"Lスティックで移動してみよう", L"", L"");
	CreateWord(TYUTORIAL_TALK2, L"これは.....メガホン?", L"でも、動いてる?");
	CreateWord(TYUTORIAL_TALK3, L"う..う.......はっ!?", L"敵!?.......じゃないみたいだな", L" ");
	CreateWord(TYUTORIAL_TALK4, L"え!?メガホンが喋った!", L" ", L" ");
	CreateWord(TYUTORIAL_TALK5, L"メガホンじゃない、オレはストポンだ", L"メガホンに取り憑いた亡霊だ!", L"こうしちゃいられない....追手が来ちまう");
	CreateWord(TYUTORIAL_TALK6, L"追手ってあれのこと?", L" ", L" ");
	CreateWord(TYUTORIAL_TALK7, L"チッ、もう追ってきやがった",L"おい人間、死にたくないなら",L"今からいうことをしっかり聞くんだ");
	CreateWord(TYUTORIAL_TALK8, L"う、うん! ", L" ", L" ");
	CreateWord(TYUTORIAL_TALK9, L"今からあの追手を倒す", L"その為に近くにいる亡霊に", L"魂を吹き込んで仲間にしろ");
	CreateWord(SETUMEI2, L"亡霊に近づいてRBを押せ", L"", L"");
	CreateWord(TYUTORIAL_TALK10, L"よし、仲間にできたな", L"そしたら亡霊の力を借りて攻撃だ", L"");
	CreateWord(SETUMEI3, L"敵に向かってAボタンで攻撃", L"", L"");
	CreateWord(TYUTORIAL_TALK11, L"ひとまずはこれで安心だな", L"", L"");
	CreateWord(TYUTORIAL_TALK12, L"あれ?", L"仲間にした亡霊が消えそう", L"");
	CreateWord(TYUTORIAL_TALK13, L"亡霊は魂が吹き込まれたことで", L"飢えを感じる", L"");
	CreateWord(TYUTORIAL_TALK14, L"飢えてしまうと", L"亡霊は消えていなくなる", L"");
	CreateWord(TYUTORIAL_TALK15, L"そうならないために別の亡霊に", L"食料を取ってきてもらうしかない", L"");
	CreateWord(TYUTORIAL_TALK16, L"試しに近くにいる亡霊に", L"食料を取ってこいと命令してみろ", L"");
	CreateWord(SETUMEI4, L"亡霊に近づいてLBを押せ", L"", L"");
	CreateWord(TYUTORIAL_TALK17, L"よし、これで亡霊が", L"消える心配は今はないな", L"");
	CreateWord(TYUTORIAL_TALK18, L"だが、亡霊は魂を吹き込むと", L"常に飢えを感じるから定期的に食料を", L"与えないと直ぐに消えてしまうからな");
	CreateWord(TYUTORIAL_TALK19, L"また追手が来やがった", L"さっき教えたことをちゃんと覚えているな", L"");
	CreateWord(TYUTORIAL_TALK20, L"死にたくないなら力をかせ", L"", L"");
	CreateWord(TYUTORIAL_TALK21, L"お前、なかなかやるじゃないか", L"", L"");
	CreateWord(TYUTORIAL_TALK22, L"そういえば", L"名前を聞いてなかったな", L"");
	CreateWord(TYUTORIAL_TALK23, L"私の名前はコトコ", L"", L"");
	CreateWord(TYUTORIAL_TALK24, L"そうかコトコか", L"", L"");
	CreateWord(TYUTORIAL_TALK25, L"改めてコトコに頼みがある", L"", L"");
	CreateWord(TYUTORIAL_TALK26, L"俺の友達を助ける為に力をかしてくれ", L"", L"");
	CreateWord(TYUTORIAL_TALK27, L"友達?", L"", L"");
	CreateWord(TYUTORIAL_TALK28, L"あぁ、俺の友達は皆良いやつなんだ", L"しかし、悪霊となってしまったんだ", L"");
	CreateWord(TYUTORIAL_TALK29, L"俺の呼びかけじゃ", L"皆を元に戻すことは出来なかった", L"");
	CreateWord(TYUTORIAL_TALK30, L"だから頼む", L"", L"");
	CreateWord(TYUTORIAL_TALK31, L"いいよその代わり私の友達になって", L"", L"");
	CreateWord(TYUTORIAL_TALK32, L"ありがとう", L"これからよろしくなコトコ", L"");
	CreateWord(TYUTORIAL_TALK33, L"うんよろしくねストポン", L"", L"");

	CreateWord(AISATU, L"おはよう", L"こんにちは", L"こんばんは");


	CreateWord(Name_First::VIEWBOSS, L"ちえよしもどってこい!!!",L"そんなにうごきまわったらしんでしまうぞ!!",L"");
	CreateWord(Name_First::SPEAKPLAYER1, L"すてぽん。。ちえよしはもう。。");
	CreateWord(Name_First::SPEALPLAYER2, L"ちえよしは、とっしんすることしかあたまにないんじゃろう。。。",L"くっ。。。にんげんときにはあきらめもたいせつか。。。",L"ちえよし。。しんでくれ!!!");
	CreateWord(Name_First::VIEWBOSS, L"ちちうえ!ほんがうごきまわってるでござる!",L"どうするでござるか!?",L"しめるでござるか!?");
	CreateWord(Name_First::SPEAKPLAYER1, L"。。。。。。。");
	CreateWord(Name_First::SPEALPLAYER2, L"あれはわしらにはすくえぬものじゃ",L"ぜんりょくでころすのじゃ");

	SecondCreateWord(ANGER_TALK, L"うぉおおおおい!!!", L"いったいオマエは!!!!!!", L"ナニしにキタ!?!?!?!?!?!?");
	SecondCreateWord(ANGER_TALK2, L"もしかして", L"おれにたいして", L"ケンカをうりにきたのか!?");
	SecondCreateWord(JOY_TALK, L"まあまあおちついてよ", L"ほらみてみなよ", L"こわがってるじゃんか!");
	SecondCreateWord(JOY_TALK2, L"いきなりなんだけどヒトツきみに", L"きいてみたいことがあるんだ!", L"");
	SecondCreateWord(JOY_TALK3, L"きみは", L"イカリとヨロコビというキモチ", L"どっちがダイジだとおもう?");
	SecondCreateWord(SELECT_TALK, L"どっちだろう・・・?", L"Y:イカリ", L"X:ヨロコビ");
	SecondCreateWord(SELECT_ANGER, L"イカリがダイジだとおもうか", L"よくわかってるじゃないか・・・", L"ってことはよ・・・");
	SecondCreateWord(SELECT_ANGER2, L"このカンジョウにまかせて", L"オマエをたおしてもいいってことだな!!!", L"!!!!!!!!");
	SecondCreateWord(SELECT_JOY, L"キミよくわかってるね!", L"ってことはさ、キミは", L"このぼくのカンジョウ・・・");
	SecondCreateWord(SELECT_JOY2, L"めのまえのやつをたおすヨロコビを", L"リカイしてくれるよね!!!", L"!!!!!!!!!!");
	//コンヴァージョン初期化
	Create(dxcomon);

	//フォントのあれこれ
	conversation_.FirstFont->SetPos(kFirstRowPos);
	conversation_.SecondFont->SetPos(kSecondRowPos);
	conversation_.ThirdFont->SetPos(kThirdRowPos);


}
//描画?
void TextManager::Draw(DirectXCommon* dxcommon)
{
	//コンヴァージョン.フォントドローする
	conversation_.FirstFont->Draw(dxcommon);
	conversation_.SecondFont->Draw(dxcommon);
	conversation_.ThirdFont->Draw(dxcommon);
	Font::PostDraw(dxcommon);
}

void TextManager::SetAllColor(const XMVECTOR& color)
{
	conversation_.FirstFont->SetColor(color);
	conversation_.SecondFont->SetColor(color);
	conversation_.ThirdFont->SetColor(color);
}

void TextManager::SetOnceColor(int row, const XMVECTOR& color)
{
	assert(row > 2);
	if (row == 0) {
		conversation_.FirstFont->SetColor(color);
	}
	else if (row == 1) {
		conversation_.SecondFont->SetColor(color);
	}
	else if (row == 2) {
		conversation_.ThirdFont->SetColor(color);
	}
}

//名前から文字列を呼び出しセットする
void TextManager::SetConversation(Name name,const XMVECTOR& color)
{
	std::map<TextManager::Name, Word>::iterator itr = wordlist_.find(name);

	CreateCon(conversation_, itr->second);

	conversation_.FirstFont->SetColor(color);
	conversation_.SecondFont->SetColor(color);
	conversation_.ThirdFont->SetColor(color);
}
//名前から文字列を呼び出しセットする
void TextManager::SetConversation(Name_First name)
{
	std::map<TextManager::Name_First, Word>::iterator itr = wordlist_first.find(name);

	CreateCon(conversation_, itr->second);
	//conversation_ = CreateConversation(itr->second);
}

//名前から文字列を呼び出しセットする
void TextManager::SetSecondConversation(Name_Second name)
{
	std::map<TextManager::Name_Second, Word>::iterator itr = wordlist_second.find(name);

	CreateCon(conversation_, itr->second);
	//conversation_ = CreateConversation(itr->second);
}
//名前と文字列セットで保存
void TextManager::CreateWord(Name name, wchar_t* tex1, wchar_t* tex2, wchar_t* tex3)
{
	Word temp = SetWord(tex1, tex2, tex3);
	wordlist_.insert(std::make_pair(name, temp));
}

//名前と文字列セットで保存
void TextManager::CreateWord(Name_First name, wchar_t* tex1, wchar_t* tex2, wchar_t* tex3)
{
	Word temp = SetWord(tex1, tex2, tex3);
	wordlist_first.insert(std::make_pair(name, temp));
}

//名前と文字列セットで保存(2個目のボス)
void TextManager::SecondCreateWord(Name_Second name, wchar_t* tex1, wchar_t* tex2, wchar_t* tex3)
{
	Word temp = SetWord(tex1, tex2, tex3);
	wordlist_second.insert(std::make_pair(name, temp));
}

//文字列保存
TextManager::Word TextManager::SetWord(wchar_t* tex1, wchar_t* tex2, wchar_t* tex3)
{
	Word word = {};
	word.FirstWord = tex1;
	word.SecondWord = tex2;
	word.ThirdWord = tex3;
	return word;
}
void TextManager::NoneText()
{
	std::map<TextManager::Name, Word>::iterator itr = wordlist_.find(NONE);
	
	CreateCon(conversation_, itr->second);
}
//文字列呼び出し
//TextManager::Conversation TextManager::CreateConversation(Word word)
//{
//	/*Conversation* temp = {};
//	temp->FirstFont->SetString(word.FirstWord);
//	temp->SecondFont->SetString(word.SecondWord);
//	temp->ThirdFont->SetString(word.ThirdWord);
//	return temp;*/
//}

void TextManager::CreateCon(Conversation con, Word word)
{
	con.FirstFont->SetString(word.FirstWord);
	con.SecondFont->SetString(word.SecondWord);
	con.ThirdFont->SetString(word.ThirdWord);
}
