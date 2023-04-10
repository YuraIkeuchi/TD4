#include "UI.h"
#include"ImageManager.h"

std::unique_ptr<UI::Debug>UI::TexA = {};

//初期化
void UI::Initialize()
{
	TexA.reset(new Debug);
	TexA->Tex=IKESprite::Create(ImageManager::GAMECLEAR, { 0.f,0.f });
	TexA->Tex->SetAnchorPoint({ 0.5f,0.5f });
	TexA->Tex->SetSize({ 1900.f,1000.f });
}

//更新
void UI::Update()
{
	
}

//描画
void UI::Draw()
{
	TexA->Tex->Draw();
}


