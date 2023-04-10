#include "UI.h"
#include"ImageManager.h"

UI::~UI()
{
	TexList.clear();
}

//初期化
void UI::Initialize()
{
	//初期化
	{
		TexA.Tex.reset(IKESprite::Create(ImageManager::GAMECLEAR, { 0.f,0.f }));
		TexA.Tex->SetAnchorPoint({ 0.5f,0.5f });
		TexA.Scale = { 1000.f,1000.f };
		TexA.Position = {0.f,0.f};
	}


	/*生成したテクスチャはListぶち込んでください　draw(),Upda()を一括で回す*/
	TexList.emplace_back(std::move(TexA));
}

//更新
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

//描画
void UI::Draw()
{
	for (auto i = 0; i < TexList.size(); i++)
	{
		if (TexList[i].Tex == nullptr)continue;
		TexList[i].Tex->Draw();
	}
}


