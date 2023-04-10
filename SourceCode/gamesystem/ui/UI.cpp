#include "UI.h"
#include"ImageManager.h"

UI::~UI()
{
	TexList.clear();
}

//������
void UI::Initialize()
{
	//������
	{
		TexA.Tex.reset(IKESprite::Create(ImageManager::GAMECLEAR, { 0.f,0.f }));
		TexA.Tex->SetAnchorPoint({ 0.5f,0.5f });
		TexA.Scale = { 1000.f,1000.f };
		TexA.Position = {0.f,0.f};
	}


	/*���������e�N�X�`����List�Ԃ�����ł��������@draw(),Upda()���ꊇ�ŉ�*/
	TexList.emplace_back(std::move(TexA));
}

//�X�V
void UI::Update()
{
	for (auto i = 0; i < TexList.size(); i++)
	{
		if (TexList[i].Tex == nullptr)continue;
		TexList[i].Tex->SetPosition(TexList[i].Position);
		TexList[i].Tex->SetSize(TexList[i].Scale);
		TexList[i].Tex->SetRotation(TexList[i].Rotation);
		TexList[i].Tex->SetColor(TexList[i].Color);
	}
}

//�`��
void UI::Draw()
{
	for (auto i = 0; i < TexList.size(); i++)
	{
		if (TexList[i].Tex == nullptr)continue;
		TexList[i].Tex->Draw();
	}
}


