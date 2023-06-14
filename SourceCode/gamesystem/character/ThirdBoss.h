﻿#pragma once
#include "IKESprite.h"
#include "InterBoss.h"
#include "Shake.h"
#include "Player.h"
#include "IKETexture.h"

class ThirdBoss :
	public InterBoss {
public:
	ThirdBoss();

	bool Initialize() override;//初期化

	void SkipInitialize() override;//スキップ時の初期化

	void Pause() override;//ポーズ

	void Action() override;//行動

	void ColPlayer(XMFLOAT3& Pos);

	void AppearAction() override {};//ボス登場の固有の処理

	void DeadAction() override {};//ボス撃破の固有の処理

	void DeadAction_Throw() override {};//ボス撃破の固有の処理 スロー

	void ImGui_Origin() override;//ボスそれぞれのImGui

	void EffecttexDraw(DirectXCommon* dxCommon) override;

	void Draw(DirectXCommon* dxCommon) override;//描画
private:
	enum class commandState : int {
		WaitCommand = 0,
		MoveCommand,
		ControlCommand,
		COMMANDMAX
	};

	//関数ポインタ
	static void(ThirdBoss::* stateTable[])();
	//メンバ関数
	void WaitUpdate();
	void MoveUpdate();
	void ControlUpdate();


private:
	static const int kPhotoSpotMax = 5;
	array<unique_ptr<IKETexture>, kPhotoSpotMax> photoSpot = {};

	enum {
		Photo_In,
		Photo_Out_Top,
		Photo_Out_Under,
		SpriteMax,
	};


	array<unique_ptr<IKESprite>, 3> photo = {};


	array<XMFLOAT3, kPhotoSpotMax> spotPos = {
		XMFLOAT3({-48,0,-55}),
		XMFLOAT3({58,0,-55}),
		XMFLOAT3({-48,0,55}),
		XMFLOAT3({58,0,55}),
		XMFLOAT3({0,0,0})
	};
	XMFLOAT3 rot = { 90.0f,0.0f,0.0f };

private:
	array<int, (size_t)commandState::COMMANDMAX> ActionTimerMax = {60,120,60};
	int moveSpawn = 0;
	int nowSpawn = 0;
	commandState phase = commandState::WaitCommand;

	bool isShutter = false;
	float shutterTime = 0.0f;
	float shutterTimeMax = 15.0f;
	float feedTimer = 0.0f;
	float feedTimeMax = 15.0f;
	float shutterHight[2] = { 0,0 };

};