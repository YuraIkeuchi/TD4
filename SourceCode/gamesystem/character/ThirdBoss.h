#pragma once
#include "IKESprite.h"
#include "InterBoss.h"
#include "Shake.h"
#include "Player.h"
class ThirdBoss :
	public InterBoss {
public:
	ThirdBoss();

	bool Initialize() override;//初期化

	void SkipInitialize() override;//スキップ時の初期化

	void Pause() override;//ポーズ

	void Action() override;//行動

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




};
