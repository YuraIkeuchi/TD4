#include "UI.h"
#include"ImageManager.h"
#include <Player.h>
#include <HungerGauge.h>

UI::~UI()
{
	TexList.clear();
}

//初期化
void UI::Initialize()
{
	//初期化
	{//PlayerUI下敷き
		sprites[UnderStatusHp].Tex = IKESprite::Create(ImageManager::WHITE, { 0.f,0.f });
		sprites[UnderStatusHp].Tex->SetAnchorPoint({ 0.f,0.f });
		sprites[UnderStatusHp].Scale = { 0.f,40.f };
		sprites[UnderStatusHp].Position = { 0.f,680.f };
		sprites[UnderStatusHp].Color = { 1.f,1,1.f,1 };
		/*生成したテクスチャはListぶち込んでください　draw(),Upda()を一括で回す*/
		TexList.emplace_back(std::move(sprites[UnderStatusHp]));
	}

	{//PlayerUI
		sprites[StatusHp].Tex=IKESprite::Create(ImageManager::WHITE, { 0.f,0.f });
		sprites[StatusHp].Tex->SetAnchorPoint({ 0.f,0.f });
		sprites[StatusHp].Scale = { 0.f,40.f };
		sprites[StatusHp].Position = {0.f,680.f};
		sprites[StatusHp].Color = { 0.5f,1,0.5f,1 };
		/*生成したテクスチャはListぶち込んでください　draw(),Upda()を一括で回す*/
		TexList.emplace_back(std::move(sprites[StatusHp]));
	}

	{//ゲージ下敷き
		sprites[UnderStatusGauge].Tex = IKESprite::Create(ImageManager::WHITE, { 0.f,0.f });
		sprites[UnderStatusGauge].Tex->SetAnchorPoint({ 0.f,0.f });
		sprites[UnderStatusGauge].Scale = { 0.f,40.f };
		sprites[UnderStatusGauge].Position = { 0.f,640.f };
		sprites[UnderStatusGauge].Color = { 1.f,1,1.f,1 };
		/*生成したテクスチャはListぶち込んでください　draw(),Upda()を一括で回す*/
		TexList.emplace_back(std::move(sprites[UnderStatusGauge]));
	}

	{//ゲージ
		sprites[StatusGauge].Tex = IKESprite::Create(ImageManager::WHITE, { 0.f,0.f });
		sprites[StatusGauge].Tex->SetAnchorPoint({ 0.f,0.f });
		sprites[StatusGauge].Scale = { 0.f,40.f };
		sprites[StatusGauge].Position = { 0.f,640.f };
		sprites[StatusGauge].Color = { 1.f,0.5,1.f,1 };
		/*生成したテクスチャはListぶち込んでください　draw(),Upda()を一括で回す*/
		TexList.emplace_back(std::move(sprites[StatusGauge]));
	}




}

//更新
void UI::Update()
{
	TexList[UnderStatusHp].Scale = { 300,40.0f };
	TexList[StatusHp].Scale = {(Player::GetInstance()->GetHP()/ Player::GetInstance()->GetMaxHP()) * 300,40.0f};

	TexList[UnderStatusGauge].Scale = { 250,40.0f };
	TexList[StatusGauge].Scale = { (HungerGauge::GetInstance()->GetNowHunger() / HungerGauge::GetInstance()->GetHungerMax()) * 250,40.0f };

	for (auto i = 0; i < TexList.size(); i++)
	{
		if (TexList[i].Tex == nullptr)continue;
		TexList[i].Tex->SetPosition(TexList[i].Position);
		TexList[i].Tex->SetSize(TexList[i].Scale);
		TexList[i].Tex->SetRotation(TexList[i].Rotation);
		TexList[i].Tex->SetColor(TexList[i].Color);
	}
}

//描画
void UI::Draw()
{
	IKESprite::PreDraw();
	for (auto i = 0; i < TexList.size(); i++)
	{
		if (TexList[i].Tex == nullptr)continue;
		TexList[i].Tex->Draw();
	}
	IKESprite::PostDraw();
}

//矢印の向き
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


