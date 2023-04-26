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
		TexA.Tex=IKESprite::Create(ImageManager::GAMECLEAR, { 0.f,0.f });
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

//���̌���
//void UI::ArrowRota() {
//	XMFLOAT2 l_MovePos = ikeplayer->GetMovePos();
//	XMFLOAT2 l_SavePos = ikeplayer->GetSavePos();
//
//	XMFLOAT2 l_Rot{};
//	float radius = 0;
//
//	l_Rot.x = (l_MovePos.x - l_SavePos.x);
//	l_Rot.y = (l_MovePos.y - l_SavePos.y);
//
//	radius = atan2f(l_Rot.x, l_Rot.y);
//
//	Arrow.Rotation = (radius * (180.0f / XM_PI));
//
//	TexList[ARROW].Rotation = ArrowRota().Rotation;
//}


