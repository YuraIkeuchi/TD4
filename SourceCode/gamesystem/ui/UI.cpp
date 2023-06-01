#include "UI.h"
#include"ImageManager.h"
#include <Player.h>
#include <HungerGauge.h>
#include "EnemyManager.h"
#include <Easing.h>

UI::~UI() {
	TexList.clear();
}

//初期化
void UI::Initialize() {
	for (int i = HeartOne; i < UnderStatusGaugeMax; i++) {
		sprites[i] = CreateUi(ImageManager::WHITE, m_PlayerHpPos, m_PlayerHpSize, { 1.5f, 1.5f, 1.5f,1 });
		TexList.emplace_back(std::move(sprites[i]));
	}
	{//ゲージ下敷き
		sprites[UnderStatusGaugeMax] = CreateUi(ImageManager::UnderGauge, m_GaugePos, m_GaugeSize, { 1.5f, 1.5f, 1.5f,0.5f });
		sprites[UnderStatusGaugeMax].Tex->SetAnchorPoint({ 0,0.5f });
		TexList.emplace_back(std::move(sprites[UnderStatusGaugeMax]));
	}
	{//ゲージ下敷き
		sprites[UnderStatusGauge] = CreateUi(ImageManager::UnderGauge, m_GaugePos, m_GaugeSize, { 1.5f, 1.5f, 1.5f,1 });
		sprites[UnderStatusGauge].Tex->SetAnchorPoint({ 0,0.5f });
		TexList.emplace_back(std::move(sprites[UnderStatusGauge]));
	}
	{//ゲージ
		sprites[StatusGauge] = CreateUi(ImageManager::Gauge, m_GaugePos, m_GaugeSizeMini, { 1.5f, 1.5f, 1.5f,1 });
		sprites[StatusGauge].Tex->SetAnchorPoint({0,0.5f});
		TexList.emplace_back(std::move(sprites[StatusGauge]));
	}
	{//ゲージ
		sprites[UnderBossGauge] = CreateUi(ImageManager::WHITE, {880,0}, {400,40}, { 1.5f, 1.5f, 1.5f,1 });
		sprites[UnderBossGauge].Tex->SetAnchorPoint({ 0,0.f });
		TexList.emplace_back(std::move(sprites[UnderBossGauge]));
	}
	{//ゲージ
		sprites[BossGauge] = CreateUi(ImageManager::WHITE, { 880,0 }, { 400,40 }, { 0.f, 1.f, 0.f,1 });
		sprites[BossGauge].Tex->SetAnchorPoint({ 0,0.f });
		TexList.emplace_back(std::move(sprites[BossGauge]));
	}

}

//更新
void UI::Update() {
	//Gauge処理
	if (HungerGauge::GetInstance()->GetCatchCount() == 0) {
		TexList[UnderStatusGauge].IsVisible = false;
		TexList[StatusGauge].IsVisible = false;
	} else {
		TexList[StatusGauge].Size = { HungerGauge::GetInstance()->GetPercentage() * m_GaugeSizeMini.x,m_GaugeSizeMini.y };
		TexList[UnderStatusGauge].Size = { (HungerGauge::GetInstance()->GetHungerMax()/5.f) * m_GaugeSize.x/10.f,m_GaugeSize.y };
		TexList[UnderStatusGauge].IsVisible = true;
		TexList[StatusGauge].IsVisible = true;
	}
	//ライフ処理
	TexList[HeartThree].Size = { (Player::GetInstance()->GetHP()/ Player::GetInstance()->GetMaxHP()) * m_PlayerHpSize.x,m_PlayerHpSize.y };
	TexList[HeartThree].Color = {0,1,0,1};
	TexList[HeartTwo].Size = { 
		Ease(In,Quad,0.3f,TexList[HeartTwo].Size.x,TexList[HeartThree].Size.x),
		Ease(In,Quad,0.3f,TexList[HeartTwo].Size.y,TexList[HeartThree].Size.y),
	};
	TexList[HeartTwo].Color = { 1,0,0,1 };

	if (boss) {
		TexList[BossGauge].Size = { boss->HpPercent() * 400.f,40.f };
	} else {
		TexList[UnderBossGauge].IsVisible = false;
		TexList[BossGauge].IsVisible = false;

	}

	for (auto i = 0; i < TexList.size(); i++) {
		if (TexList[i].Tex == nullptr)continue;
		TexList[i].Tex->SetPosition(TexList[i].Position);
		TexList[i].Tex->SetSize(TexList[i].Size);
		TexList[i].Tex->SetRotation(TexList[i].Rotation);
		TexList[i].Tex->SetColor(TexList[i].Color);
	}
}

//描画
void UI::Draw() {
	IKESprite::PreDraw();
	for (auto i = 0; i < TexList.size(); i++) {
		if (TexList[i].Tex == nullptr) { continue; }
		if (TexList[i].IsVisible) {
			TexList[i].Tex->Draw();
		}
	}
	IKESprite::PostDraw();
}

UI::SpriteData UI::CreateUi(UINT texNumber, XMFLOAT2 pos, XMFLOAT2 size, XMFLOAT4 color) {
	SpriteData itr;
	itr.Tex = IKESprite::Create(texNumber, pos);
	itr.Tex->SetAnchorPoint({ 0.f,0.f });
	itr.Position = pos;
	itr.Size = size;
	itr.Color = color;
	return itr;
}

