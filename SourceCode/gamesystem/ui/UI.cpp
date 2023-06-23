#include "UI.h"
#include"ImageManager.h"
#include <Player.h>
#include "InterBullet.h"
#include <HungerGauge.h>
#include "EnemyManager.h"
#include <Easing.h>
#include <Helper.h>
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
		sprites[ExtraGauge] = CreateUi(ImageManager::Gauge, m_GaugePos, m_GaugeSizeMini, { 1.5f, 1.5f, 1.5f,1 });
		sprites[ExtraGauge].Tex->SetAnchorPoint({ 0,0.5f });
		TexList.emplace_back(std::move(sprites[ExtraGauge]));
	}
	{//ゲージ
		sprites[StatusGauge] = CreateUi(ImageManager::Gauge, m_GaugePos, m_GaugeSizeMini, { 1.5f, 1.5f, 1.5f,1 });
		sprites[StatusGauge].Tex->SetAnchorPoint({ 0,0.5f });
		TexList.emplace_back(std::move(sprites[StatusGauge]));
	}
	{//ゲージ
		sprites[ChargeGauge] = CreateUi(ImageManager::Gauge, m_GaugePos, m_GaugeSizeMini, { 1.5f, 1.5f, 1.5f,1 });
		sprites[ChargeGauge].Tex->SetAnchorPoint({ 0,0.5f });
		TexList.emplace_back(std::move(sprites[ChargeGauge]));
	}
	{//ゲージ
		sprites[UnderBossGauge] = CreateUi(ImageManager::WHITE, { 880,0 }, { 400,40 }, { 1.5f, 1.5f, 1.5f,1 });
		sprites[UnderBossGauge].Tex->SetAnchorPoint({ 0,0.f });
		TexList.emplace_back(std::move(sprites[UnderBossGauge]));
	}
	{//ゲージ
		sprites[BossGauge] = CreateUi(ImageManager::WHITE, { 880,0 }, { 400,40 }, { 0.f, 1.f, 0.f,1 });
		sprites[BossGauge].Tex->SetAnchorPoint({ 0,0.f });
		TexList.emplace_back(std::move(sprites[BossGauge]));
	}
	{
		sprites[PlayerCircle] = CreateUi(ImageManager::CIRCLE, { m_PlayerCireclePos }, { m_PlayerCircleSize }, { 1.f,1.f,1.f,1.f });
		sprites[PlayerCircle].Tex->SetAnchorPoint({ 0.5,0.5f });
		TexList.emplace_back(std::move(sprites[PlayerCircle]));
	}
	{
		sprites[CircleCover] = CreateUi(ImageManager::CIRCLECOVER, { m_PlayerCireclePos }, { m_PlayerCircleSize }, { 1.f,1.f,1.f,1.f });
		sprites[CircleCover].Tex->SetAnchorPoint({ 0.5,0.5f });
		TexList.emplace_back(std::move(sprites[CircleCover]));
	}
	//ボスの位置
	{
		sprites[ArrowBoss] = CreateUi(ImageManager::BOSS_ARROW,{}, {62.0f,62.0f}, {1.f,1.f,1.f,1.f});
		sprites[ArrowBoss].Tex->SetAnchorPoint({ 0.5,0.5f });
		TexList.emplace_back(std::move(sprites[ArrowBoss]));
		TexList[ArrowBoss].IsVisible = false;
	}
}

//更新
void UI::Update() {
	//Gauge処理
	if (HungerGauge::GetInstance()->GetCatchCount() == 0) {
		TexList[UnderStatusGauge].IsVisible = false;
		TexList[StatusGauge].IsVisible = false;
		TexList[ExtraGauge].IsVisible = false;
		TexList[ChargeGauge].IsVisible = false;
	}
	else {
		TexList[StatusGauge].Size = { HungerGauge::GetInstance()->GetPercentage() * m_GaugeSizeMini.x,m_GaugeSizeMini.y };
		TexList[ExtraGauge].Size = { HungerGauge::GetInstance()->GetPercentageExtra() * m_GaugeSizeMini.x,m_GaugeSizeMini.y };
		TexList[ChargeGauge].Size = { Player::GetInstance()->GetPercentage() * m_GaugeSizeMini.x,m_GaugeSizeMini.y };
		TexList[UnderStatusGauge].Size = { (HungerGauge::GetInstance()->GetHungerMax() / 5.f) * m_GaugeSize.x / 10.f,m_GaugeSize.y };
		TexList[UnderStatusGauge].IsVisible = true;
		TexList[StatusGauge].IsVisible = true;
		TexList[ExtraGauge].IsVisible = true;
		TexList[ChargeGauge].IsVisible = true;
		TexList[ExtraGauge].Color = { 0.0f,1.0f,1.0f,1.0f };
		TexList[ChargeGauge].Color = { 1.0f,0.0f,1.0f,1.0f };
	}
	//ライフ処理
	TexList[HeartThree].Size = { (Player::GetInstance()->GetHP() / Player::GetInstance()->GetMaxHP()) * m_PlayerHpSize.x,m_PlayerHpSize.y };
	TexList[HeartThree].Color = { 0,1,0,1 };
	TexList[HeartTwo].Size = {
		Ease(In,Quad,0.3f,TexList[HeartTwo].Size.x,TexList[HeartThree].Size.x),
		Ease(In,Quad,0.3f,TexList[HeartTwo].Size.y,TexList[HeartThree].Size.y),
	};
	TexList[HeartTwo].Color = { 1,0,0,1 };
	TexList[PlayerCircle].Rotation = m_PlayerCircleRot;
	bullet_type_ = Player::GetInstance()->GetBulletType();
	float nowhunger = HungerGauge::GetInstance()->GetNowHunger();
	bool skip = Player::GetInstance()->GetSkip();
	if (bullet_type_ == Bullettype::BULLET_FORROW) {
		m_limit = 0.f;
		if (skip == true) {
			m_limit = 360.f;
		}
	}
	else if (bullet_type_ == Bullettype::BULLET_SEARCH) {
		m_limit = 120.f;
		if (skip == true) {
			m_limit=-240.f;
		}
	}
	else if (bullet_type_ == Bullettype::BULLET_ATTACK) {
		m_limit = 240.f;
		
	}
	int ans = bullet_type_ - oldbullet_type_;
	if (m_limit == -240) {
		m_limit = 120;
	}

	if (m_limit == 360) {
		m_limit = 0;
	}
	if (m_PlayerCircleRot != m_limit) {
		if (ans == 1 || ans == -2) {
			if (skip == false) {
				m_PlayerCircleRot += 30.f;
				if (m_PlayerCircleRot > 360.f) {
					m_PlayerCircleRot = 0.f;
				}
			}
			else if(skip==true) {
				m_PlayerCircleRot -= 30.f;
				if (m_PlayerCircleRot < -240.f) {
					m_PlayerCircleRot = 120.f;
					skip = false;
					Player::GetInstance()->SetSkip(skip);
				}
			}
		}
		else if (ans == -1 || ans == 2) {
			if (skip == false) {
				m_PlayerCircleRot -= 30.f;
				if (m_PlayerCircleRot < 0.f) {
					m_PlayerCircleRot = 360;
				}
			}
			else if (skip == true) {
				m_PlayerCircleRot += 30.f;
				if (m_PlayerCircleRot > 360.f) {
					m_PlayerCircleRot = 0.f;
					skip = false;
					Player::GetInstance()->SetSkip(skip);
				}
			}
		}
	}
	else {
		oldbullet_type_ = bullet_type_;
	}
	
	

	if (boss) {
		TexList[BossGauge].Size = { boss->HpPercent() * 400.f,40.f };
	}
	else {
		TexList[UnderBossGauge].IsVisible = false;
		TexList[BossGauge].IsVisible = false;

	}

	//ボスの探索
	if(boss)
	SeachBoss();

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

//ボスの探索
void UI::SeachBoss() {
	const float l_LookLength = 55.0f;
	XMFLOAT3 l_PlaPos = Player::GetInstance()->GetPosition();
	XMFLOAT3 l_bossPos = boss->GetPosition();
	XMFLOAT3 l_Position{};
	float l_Radius = 0;
	XMFLOAT2 rot{};
	l_Position.x = (l_PlaPos.x - l_bossPos.x);
	l_Position.z = (l_PlaPos.z - l_bossPos.z);

	l_Radius = atan2f(l_Position.x, l_Position.z);// *PI / 180.0f;
	m_Circle.x = (sin(-l_Radius) * 150.0f) + WinApp::window_width / 2; //*0.2251f;
	m_Circle.y = (cos(-l_Radius) * 150.0f) + WinApp::window_height / 2; //*0.2251f;
	if (l_LookLength < Helper::GetInstance()->ChechLength({ l_PlaPos.x,0.0f,l_PlaPos.z }, { l_bossPos.x,0.0f,l_bossPos.z })) {
		TexList[ArrowBoss].IsVisible = true;
	}
	else {
		TexList[ArrowBoss].IsVisible = false;
	}
	TexList[ArrowBoss].Rotation = (l_Radius * (PI_180 / XM_PI)) + PI_180;//-radius * PI / 180.0f);
	TexList[ArrowBoss].Position = m_Circle;
	//TexList[ArrowBoss].Size = { 62.0f,62.0f };
}