#include "UI.h"
#include"ImageManager.h"
#include <Player.h>
#include "InterBullet.h"
#include <HungerGauge.h>
#include "EnemyManager.h"
#include <Easing.h>
#include <Helper.h>
#include <random>
UI::~UI() {
	TexList.clear();
}

//初期化
void UI::Initialize() {
	sprites[HeartOne] = CreateUi(ImageManager::PlayerHPGauge, m_PlayerHpPos, m_PlayerHpSize, { 1.5f, 1.5f, 1.5f,1 });
	TexList.emplace_back(std::move(sprites[HeartOne]));
	for (int i = HeartTwo; i < Heart; i++) {
		sprites[i] = CreateUi(ImageManager::WHITE, { m_PlayerHpPos.x + 70.0f,m_PlayerHpPos.y + 35.0f }, m_PlayerHpSize, { 1.5f, 1.5f, 1.5f,1 });
		TexList.emplace_back(std::move(sprites[i]));
	}
	sprites[Heart] = CreateUi(ImageManager::HEART, { 50.f, 680.f }, { 69 * 0.6f,60 * 0.6f }, { 1.5f, 1.5f, 1.5f,1 });
	sprites[Heart].Tex->SetAnchorPoint({ 0.5f,0.5f });
	TexList.emplace_back(std::move(sprites[Heart]));
	{//ゲージ下敷き
		sprites[UnderStatusGaugeMax] = CreateUi(ImageManager::UnderGauge, m_GaugePos, m_GaugeSize, { 1.5f, 1.5f, 1.5f,1.f });
		sprites[UnderStatusGaugeMax].Tex->SetAnchorPoint({ 0,0 });
		TexList.emplace_back(std::move(sprites[UnderStatusGaugeMax]));

		sprites[UnderExtraGaugeMax] = CreateUi(ImageManager::EXGauge, m_GaugePos, m_GaugeSize, { 1.5f, 1.5f, 1.5f,1.f });
		sprites[UnderExtraGaugeMax].Tex->SetAnchorPoint({ 0,0 });
		TexList.emplace_back(std::move(sprites[UnderExtraGaugeMax]));
	}
	{//ゲージ
		sprites[ExtraGauge] = CreateUi(ImageManager::WHITE, m_GaugePosMini, m_GaugeSizeMini, { 0.5f, 0.5f, 1.f,1.f });
		TexList.emplace_back(std::move(sprites[ExtraGauge]));
	}
	{//ゲージ
		sprites[StatusGauge] = CreateUi(ImageManager::WHITE, m_GaugePosMini, m_GaugeSizeMini, { 0.0f,1.0f,1.0f,1.0f });
		TexList.emplace_back(std::move(sprites[StatusGauge]));
	}
	{//ゲージ
		sprites[ChargeGauge] = CreateUi(ImageManager::Gauge, m_GaugePosMini, m_GaugeSizeMini, { 1.5f, 1.5f, 1.5f,1 });
		TexList.emplace_back(std::move(sprites[ChargeGauge]));
	}
	{//ゲージ
		sprites[UnderBossGauge] = CreateUi(ImageManager::BossHPGauge, m_UnderBossPos, m_PlayerHpSize, { 1.5f, 1.5f, 1.5f,1 });
		sprites[UnderBossGauge].Tex->SetAnchorPoint({ 1.0f,0.f });
		TexList.emplace_back(std::move(sprites[UnderBossGauge]));
		XMFLOAT2 pos = TexList[UnderBossGauge].Tex->GetPosition();
		sprites[MiddleBossGauge] = CreateUi(ImageManager::WHITE, m_MiddleBossPos, { 350,40 }, { 1.f, 1.f, 0.f,1 });
		sprites[MiddleBossGauge].Tex->SetAnchorPoint({ 0,0.f });
		TexList.emplace_back(std::move(sprites[MiddleBossGauge]));
		sprites[BossGauge] = CreateUi(ImageManager::WHITE, m_BossPos, { 350,40 }, { 1.f, 0.f, 0.f,1 });
		sprites[BossGauge].Tex->SetAnchorPoint({ 0,0.f });
		TexList.emplace_back(std::move(sprites[BossGauge]));
	}
	{
		sprites[CircleCover] = CreateUi(ImageManager::CIRCLECOVER, { m_PlayerCireclePos.x,m_PlayerCireclePos.y - 40.f }, { 350.f,350.f }, { 1.f,1.f,1.f,1.f });
		sprites[CircleCover].Tex->SetAnchorPoint({ 0.5,0.5f });
		TexList.emplace_back(std::move(sprites[CircleCover]));
	}
	{
		sprites[PlayerCircle] = CreateUi(ImageManager::CIRCLE, { m_PlayerCireclePos }, m_PlayerCircleSize, { 1.2f,1.2f,1.2f,1.f });
		sprites[PlayerCircle].Tex->SetAnchorPoint({ 0.5,0.5f });
		TexList.emplace_back(std::move(sprites[PlayerCircle]));
	}
	//ボスの位置
	{
		sprites[ArrowBoss] = CreateUi(ImageManager::BOSS_ARROW, {}, { 62.0f,62.0f }, { 1.f,1.f,1.f,1.f });
		sprites[ArrowBoss].Tex->SetAnchorPoint({ 0.5,0.5f });
		TexList.emplace_back(std::move(sprites[ArrowBoss]));
		TexList[ArrowBoss].IsVisible = false;
	}
	//ウェイ
	{
		const int w = 128;
		const int h = 128;
		const int l = 4;
		for (int i = HitodamaWay1;  i <= HitodamaWay4; i++) {
			sprites[i] = CreateUi(ImageManager::HITODAMA, { WinApp::window_width / 2,WinApp::window_height / 2 }, {w,h}, {1.f,1.f,1.f,1.f});
			int number_index_y = i / l;
			int number_index_x = i % l;
			sprites[i].Tex->SetTextureRect({ static_cast<float>(number_index_x) * w,static_cast<float>(number_index_y) * h },
				{ static_cast<float>(w),static_cast<float>(h) });
			sprites[i].Tex->SetScale(1.f);
			sprites[i].Tex->SetAnchorPoint({ 0.5f,0.5f });

		}
	}
	if (boss) {
		bossHpOld = boss->GetHP();
	}
}

//更新
void UI::Update() {
	PlayerGauge();//チャージゲージ関連
	PlayerLife();//プレイヤーのHP
	BulletChange();//サークルの処理
	BossLife();	//ボスの探索とHP

	//テキスト更新
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
	} else {
		TexList[ArrowBoss].IsVisible = false;
	}
	TexList[ArrowBoss].Rotation = (l_Radius * (PI_180 / XM_PI)) + PI_180;//-radius * PI / 180.0f);
	TexList[ArrowBoss].Position = m_Circle;
	//TexList[ArrowBoss].Size = { 62.0f,62.0f };
}

void UI::PlayerLife() {
	float percent = (Player::GetInstance()->GetHP() / Player::GetInstance()->GetMaxHP());
	m_frame += m_vel;
	TexList[Heart].Size = { TexList[Heart].Size.x - (sinf(m_frame) * 0.5f) , TexList[Heart].Size.y - (sinf(m_frame) * 0.5f) };

	//ライフ処理
	TexList[HeartThree].Size = { percent * m_PlayerHpSize.x * 0.68f ,m_PlayerHpSize.y * 0.32f };
	TexList[HeartTwo].Size = {
		Ease(In,Quad,0.3f,TexList[HeartTwo].Size.x,TexList[HeartThree].Size.x),
		Ease(In,Quad,0.3f,TexList[HeartTwo].Size.y,TexList[HeartThree].Size.y),
	};
	if (percent >= 0.5f) {
		m_vel = 1 / 8.0f;
		TexList[HeartThree].Color = { 0,1,0,1 };
		TexList[HeartTwo].Color = { 1,0,0,1 };
	} else if (percent >= 0.25f) {
		m_vel = 1 / 8.0f;
		TexList[HeartThree].Color = { 1,1,0,1 };
		TexList[HeartTwo].Color = { 0.5f,0,0,1 };
	} else {
		m_vel = 1 / 4.0f;
		TexList[HeartThree].Color = { 1,0,0,1 };
		TexList[HeartTwo].Color = { 0,0,0,1 };
	}

}

void UI::PlayerGauge()	 {
	HungerGauge* hungerGauge = HungerGauge::GetInstance();

	for (int i = HitodamaWay1; i < HitodamaWay4; i++) {
		TexList[i].IsVisible = false;
	}

	//Gauge処理
	if (hungerGauge->GetCatchCount() == 0) {
		TexList[StatusGauge].IsVisible = false;
		TexList[ExtraGauge].IsVisible = false;
		TexList[ChargeGauge].IsVisible = false;
	} else {
		TexList[StatusGauge].Size = { hungerGauge->GetPercentage() * m_GaugeSizeMini.x,m_GaugeSizeMini.y };
		TexList[ExtraGauge].Size = { hungerGauge->GetPercentageExtra() * m_GaugeSizeMini.x,m_GaugeSizeMini.y };
		TexList[ChargeGauge].Size = { Player::GetInstance()->GetPercentage() * m_GaugeSizeMini.x,m_GaugeSizeMini.y };
		TexList[StatusGauge].IsVisible = true;
		TexList[ExtraGauge].IsVisible = true;
		TexList[ChargeGauge].IsVisible = true;
		//HungerGauge::GetInstance()->GetAdditional()
		if (Player::GetInstance()->GetChargeType() == 0) {
			TexList[ChargeGauge].Color = { 1.0f,1.0f,0.2f,1.0f };
		} else if (Player::GetInstance()->GetChargeType() == 1) {
			TexList[ChargeGauge].Color = { 1.0f,0.6f,0.0f,1.0f };
		} else if (Player::GetInstance()->GetChargeType() == 2) {
			TexList[ChargeGauge].Color = { 1.0f,0.3f,0.0f,1.0f };
		} else {
			TexList[ChargeGauge].Color = { 1.0f,0.0f,0.0f,1.0f };
		}
	}
	if (hungerGauge->GetCatchCount() > 0 && hungerGauge->GetAdditional() > 0.f) {
		if (state == GaugeState::nom4l) {
			state = GaugeState::ch4nge;
		}
	} else {
		if (state == GaugeState::extr4) {
			state = GaugeState::b4ck;
		}
	}
	switch (state) {
	case UI::GaugeState::nom4l:
		TexList[UnderStatusGaugeMax].IsVisible = true;
		TexList[UnderExtraGaugeMax].IsVisible = false;
		TexList[UnderExtraGaugeMax].Color = { 1.5f, 1.5f, 1.5f,1.f };
		break;
	case UI::GaugeState::ch4nge:
		ch4ngeTimer += 1.f / 15.f;
		TexList[UnderStatusGaugeMax].Color
			= {
			Ease(Out,Quad,ch4ngeTimer,1.5f,5.f),
			Ease(Out,Quad,ch4ngeTimer,1.5f,5.f),
			Ease(Out,Quad,ch4ngeTimer,1.5f,5.f),
			1.f };
		Helper::GetInstance()->Clamp(ch4ngeTimer, 0.0f, 1.0f);
		if (ch4ngeTimer == 1.0f) {
			state = GaugeState::extr4;
			ch4ngeTimer = 0.0f;
		}
		break;
	case UI::GaugeState::extr4:
		TexList[UnderStatusGaugeMax].IsVisible = false;
		TexList[UnderStatusGaugeMax].Color = { 1.5f, 1.5f, 1.5f,1.f };
		TexList[UnderExtraGaugeMax].IsVisible = true;
		break;
	case UI::GaugeState::b4ck:
		ch4ngeTimer += 1.f / 15.f;
		TexList[UnderExtraGaugeMax].Color
			= {
			Ease(Out,Quad,ch4ngeTimer,1.5f,5.f),
			Ease(Out,Quad,ch4ngeTimer,1.5f,5.f),
			Ease(Out,Quad,ch4ngeTimer,1.5f,5.f),
			1.f };
		Helper::GetInstance()->Clamp(ch4ngeTimer, 0.0f, 1.0f);
		if (ch4ngeTimer == 1.0f) {
			state = GaugeState::nom4l;
			ch4ngeTimer = 0.0f;
		}
		break;
	default:
		break;
	}
}

void UI::BulletChange() {
	TexList[PlayerCircle].Rotation = m_PlayerCircleRot;
	bullet_type_ = Player::GetInstance()->GetBulletType();
	float nowhunger = HungerGauge::GetInstance()->GetNowHunger();
	bool skip = Player::GetInstance()->GetSkip();
	if (bullet_type_ == Bullettype::BULLET_FORROW) {
		m_limit = 0.f;
		if (skip == true) {
			m_limit = 360.f;
		}
	} else if (bullet_type_ == Bullettype::BULLET_SEARCH) {
		m_limit = 120.f;
		if (skip == true) {
			m_limit = -240.f;
		}
	} else if (bullet_type_ == Bullettype::BULLET_ATTACK) {
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
			} else if (skip == true) {
				m_PlayerCircleRot -= 30.f;
				if (m_PlayerCircleRot < -240.f) {
					m_PlayerCircleRot = 120.f;
					skip = false;
					Player::GetInstance()->SetSkip(skip);
				}
			}
		} else if (ans == -1 || ans == 2) {
			if (skip == false) {
				m_PlayerCircleRot -= 30.f;
				if (m_PlayerCircleRot < 0.f) {
					m_PlayerCircleRot = 360;
				}
			} else if (skip == true) {
				m_PlayerCircleRot += 30.f;
				if (m_PlayerCircleRot > 360.f) {
					m_PlayerCircleRot = 0.f;
					skip = false;
					Player::GetInstance()->SetSkip(skip);
				}
			}
		}
	} else {
		oldbullet_type_ = bullet_type_;
	}
}

void UI::BossLife() {
	if (boss) {
		SeachBoss();
		TexList[BossGauge].Size = { boss->HpPercent() * m_PlayerHpSize.x * 0.67f ,m_PlayerHpSize.y * 0.33f };
		TexList[MiddleBossGauge].Size = {
		Ease(In,Quad,0.3f,TexList[MiddleBossGauge].Size.x,TexList[BossGauge].Size.x),
		Ease(In,Quad,0.3f,TexList[MiddleBossGauge].Size.y,TexList[BossGauge].Size.y),
		};

		if (bossHpOld != boss->GetHP()) {
			if (!isCrush) {
				isCrush = true;
			}
		}
		bossHpOld = boss->GetHP();


		if (isCrush) {
			TexList[UnderBossGauge].Position = {
					m_UnderBossPos.x + power,
					m_UnderBossPos.y + power
			};
			for (int i = MiddleBossGauge; i <= BossGauge; i++) {
				TexList[i].Position = {
					m_MiddleBossPos.x + power,
					m_MiddleBossPos.y + power
				};
			}
			crushTimer += 1 / 15.0f;

			power = power - 1.0f;
			power *= -1.0f;
			Helper::GetInstance()->Clamp(power, -18.0f, 20.0f);
			Helper::GetInstance()->Clamp(crushTimer, 0.0f, 1.0f);

			if (crushTimer == 1.0f) {
				isCrush = false;
				TexList[UnderBossGauge].Position = m_UnderBossPos;
				TexList[MiddleBossGauge].Position = m_MiddleBossPos;
				TexList[BossGauge].Position = m_BossPos;
				power = 10.0f;
				crushTimer = 0.0f;
			}
		}

	} else {
		TexList[UnderBossGauge].IsVisible = false;
		TexList[MiddleBossGauge].IsVisible = false;
		TexList[BossGauge].IsVisible = false;
	}
}
