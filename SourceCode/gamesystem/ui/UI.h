#pragma once
#include"IKESprite.h"
#include<memory>
#include<DirectXMath.h>
#include <array>
#include"InterBoss.h"

using namespace DirectX;

class InterBullet;

class UI {
public:
	UI() {};
	~UI();
	void SetBoss(InterBoss* boss) { this->boss = boss; }
private:
	//仮フォント
	struct SpriteData {
		//画像
		std::unique_ptr<IKESprite> Tex;
		//基礎パラメータ
		XMFLOAT2 Position = { 0,0 };
		XMFLOAT2 Size = { 100,100 };
		float Rotation = 0.f;
		XMFLOAT4 Color = { 1,1,1,1 };
		//表示なフラグ
		bool IsVisible = true;
	};
	enum  SpriteName {
		HeartOne = 0,
		HeartTwo,
		HeartThree,
		Heart,
		UnderStatusGaugeMax,
		UnderExtraGaugeMax,
		ExtraGauge,
		StatusGauge,
		ChargeGauge,
		UnderBossGauge,
		MiddleBossGauge,
		BossGauge,
		CircleCover,
		PlayerCircle,
		ArrowBoss,
		UiMax,
	};

	//スプライト
	std::array<SpriteData, UiMax> sprites = {};

	XMFLOAT2 m_GaugePos = { 430,623.f };
	XMFLOAT2 m_GaugePosMini = { 430 + 42.0f,597 + 50.0f };
	XMFLOAT2 m_GaugeSize = { 512 * 0.9f,100 * 0.9f };
	XMFLOAT2 m_GaugeSizeMini = { 377,45 };

	XMFLOAT2 m_PlayerHpPos = { 15.f, 640.f };
	XMFLOAT2 m_PlayerHpSize = { 512.f * 0.6f, 128.f * 0.6f };

	XMFLOAT2 m_PlayerCireclePos = { WinApp::window_width - 100 ,WinApp::window_height };
	XMFLOAT2 m_PlayerCircleSize = { 240.f,240.f };

	XMFLOAT2 m_UnderBossPos = { WinApp::window_width - 10,10 };
	XMFLOAT2 m_MiddleBossPos = { m_UnderBossPos.x - 279.f,m_UnderBossPos.y + 17.f };
	XMFLOAT2 m_BossPos = { m_UnderBossPos.x - 279.f,m_UnderBossPos.y + 17.f };

	float m_PlayerCircleRot = 0.f;

	float m_limit = 0.f;
	float m_frame = 0.0f;
	float m_vel = 1 / 10.0f;
	int bullet_type_ = 0;
	int oldbullet_type_ = 0;
	XMFLOAT2 m_Circle = { 640.0f,360.0f };
	std::vector<SpriteData>TexList = {};

	bool m_Look = false;

	enum class GaugeState :int {
		nom4l = 0,
		ch4nge,
		extr4,
		b4ck,
	};
	GaugeState state = GaugeState::nom4l;
	float ch4ngeTimer = 0.f;

	float bossHpOld = 0.0f;
	bool isCrush = false;
	float power = 10.0f;
	float crushTimer = 0.0f;
public:
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();
private:
	//
	SpriteData CreateUi(UINT texNumber, XMFLOAT2 pos, XMFLOAT2 size, XMFLOAT4 color);
	InterBoss* boss = nullptr;
private:
	//ボスの表示
	void SeachBoss();
	void PlayerLife();
	void PlayerGauge();
	void BulletChange();
	void BossLife();
};