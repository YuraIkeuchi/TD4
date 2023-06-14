#pragma once
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
	array<XMFLOAT3, kPhotoSpotMax> spotPos = {
		XMFLOAT3({-48,0,-55}),
		XMFLOAT3({58,0,-55}),
		XMFLOAT3({-48,0,55}),
		XMFLOAT3({58,0,55}),
		XMFLOAT3({0,0,0})
	};
	XMFLOAT3 rot = { 90.0f,0.0f,0.0f };


};
