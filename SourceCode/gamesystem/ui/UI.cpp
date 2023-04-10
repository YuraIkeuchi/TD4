#include "UI.h"
#include"ImageManager.h"

std::unique_ptr<UI::Debug>UI::TexA = {};

//‰Šú‰»
void UI::Initialize()
{
	TexA.reset(new Debug);
	TexA->Tex=IKESprite::Create(ImageManager::GAMECLEAR, { 0.f,0.f });
	TexA->Tex->SetAnchorPoint({ 0.5f,0.5f });
	TexA->Tex->SetSize({ 1900.f,1000.f });
}

//XV
void UI::Update()
{
	
}

//•`‰æ
void UI::Draw()
{
	TexA->Tex->Draw();
}


