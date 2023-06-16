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
		HeartOne=0,
		HeartTwo,
		HeartThree,
		UnderStatusGaugeMax,
		UnderStatusGauge,
		StatusGauge,
		UnderBossGauge,
		BossGauge,
		PlayerCircle,
		CircleCover,
		UiMax,
	};

	//スプライト
	std::array<SpriteData, UiMax> sprites = {};

	//std::array<XMFLOAT2, UnderStatusGauge> pos = {
	//XMFLOAT2(15.f,650.f),
	//XMFLOAT2(65.f,650.f),
	//XMFLOAT2(115.f,650.f),
	//XMFLOAT2(165.f,650.f),
	//XMFLOAT2(215.f,650.f)
	//};


	XMFLOAT2 m_GaugePos = { 340,665.f };
	XMFLOAT2 m_GaugeSize = { 600.f,50.f };
	XMFLOAT2 m_GaugeSizeMini = { 600.f,40.f };

	XMFLOAT2 m_PlayerHpPos = { 15.f, 680.f };
	XMFLOAT2 m_PlayerHpSize = { 300.f, 30.f };

	XMFLOAT2 m_PlayerCireclePos = { WinApp::window_width-200 ,WinApp::window_height };
	XMFLOAT2 m_PlayerCircleSize = { 240.f,240.f };
	float m_PlayerCircleRot = 0.f;
	float m_limit = 0.f;
	int bullet_type_ = 0;
	int oldbullet_type_ = 0;

	std::vector<SpriteData>TexList;
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
};

