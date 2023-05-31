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


//������
void TextManager::Initialize(DirectXCommon* dxcomon)
{
	//���[�h�ǉ�
	CreateWord(NONE, L" ", L" ", L" ");
	CreateWord(TYUTORIAL_TALK1, L"�����͂ǂ����낤!", L"����������񂵂Ă݂悤", L"L�X�e�B�b�N�ňړ����Ă݂悤");
	CreateWord(TYUTORIAL_TALK2, L"�����.....���K�z��?", L"�ł��A�����Ă�?");
	CreateWord(TYUTORIAL_TALK3, L"��..��.......�͂�!?", L"�G!?.......����Ȃ��݂�������", L" ");
	CreateWord(TYUTORIAL_TALK4, L"��!?���K�z����������!", L" ", L" ");
	CreateWord(TYUTORIAL_TALK5, L"���K�z������Ȃ��A�I���̓X�g�|����", L"���K�z���Ɏ��߂����S�삾!", L"���������Ⴂ���Ȃ�....�ǎ肪�����܂�");
	CreateWord(TYUTORIAL_TALK6, L"�@�ǎ���Ă���̂���?", L" ", L" ");
	CreateWord(AISATU,L"���͂悤",L"����ɂ���",L"����΂��");
	CreateWord(ANGER_TALK, L"��������������!!!", L"���������I�}�G��!!!!!!", L"�i�j���ɃL�^!?!?!?!?!?!?");
	CreateWord(ANGER_TALK2, L"����������", L"����ɂ�������", L"�P���J������ɂ����̂�!?");
	CreateWord(JOY_TALK, L"�܂��܂��������Ă�", L"�ق�݂Ă݂Ȃ�", L"���킪���Ă邶���!");
	CreateWord(JOY_TALK2, L"�����Ȃ�Ȃ񂾂��ǃq�g�c���݂�", L"�����Ă݂������Ƃ������!", L"");
	CreateWord(JOY_TALK3, L"���݂�", L"�C�J���ƃ����R�r�Ƃ����L���`", L"�ǂ������_�C�W���Ƃ�����?");
	CreateWord(SELECT_TALK, L"�ǂ������낤�E�E�E?", L"Y:�C�J��", L"X:�����R�r");
	CreateWord(SELECT_ANGER, L"�C�J�����_�C�W���Ƃ�������", L"�悭�킩���Ă邶��Ȃ����E�E�E", L"���Ă��Ƃ͂�E�E�E");
	CreateWord(SELECT_ANGER2, L"���̃J���W���E�ɂ܂�����", L"�I�}�G���������Ă��������Ă��Ƃ���!!!", L"!!!!!!!!");
	CreateWord(SELECT_JOY, L"�L�~�悭�킩���Ă��!", L"���Ă��Ƃ͂��A�L�~��", L"���̂ڂ��̃J���W���E�E�E�E");
	CreateWord(SELECT_JOY2, L"�߂̂܂��̂�������������R�r��", L"���J�C���Ă������!!!", L"!!!!!!!!!!");
	//�R�����@�[�W����������
	Create(dxcomon);

	//�t�H���g�̂��ꂱ��
	conversation_.FirstFont->SetPos(kFirstRowPos);
	conversation_.SecondFont->SetPos(kSecondRowPos);
	conversation_.ThirdFont->SetPos(kThirdRowPos);


}
//�`��?
void TextManager::Draw(DirectXCommon* dxcommon)
{
	//�R�����@�[�W����.�t�H���g�h���[����
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

//���O���當������Ăяo���Z�b�g����
void TextManager::SetConversation(Name name)
{
	std::map<TextManager::Name, Word>::iterator itr = wordlist_.find(name);

	CreateCon(conversation_, itr->second);
	//conversation_ = CreateConversation(itr->second);
}
//���O�ƕ�����Z�b�g�ŕۑ�
void TextManager::CreateWord(Name name, wchar_t* tex1, wchar_t* tex2, wchar_t* tex3)
{
	Word temp = SetWord(tex1, tex2, tex3);
	wordlist_.insert(std::make_pair(name, temp));
}

//������ۑ�
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
//������Ăяo��
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
