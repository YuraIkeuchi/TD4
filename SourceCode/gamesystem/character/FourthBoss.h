#pragma once
#include "Note.h"
#include "InterBoss.h"
#include "Shake.h"
#include "Player.h"

class FourthBoss :
	public InterBoss {
public:
	FourthBoss();

	bool Initialize() override;//初期化

	void SkipInitialize() override;//スキップ時の初期化

	void Pause() override;//ポーズ

	void Action() override;//行動

	void AppearAction() {};//ボス登場の固有の処理

	void DeadAction() {};//ボス撃破の固有の処理

	void DeadAction_Throw() {};//ボス撃破の固有の処理 スロー

	void ImGui_Origin() {};//ボスそれぞれのImGui

	void EffecttexDraw(DirectXCommon* dxCommon) override;

	void Draw(DirectXCommon* dxCommon) override;//描画
private:
	enum class commandState : int {
		WaitCommand = 0,
		MoveCommand,
		ControlCommand,
	};

	//関数ポインタ
	static void(FourthBoss::* stateTable[])();
	//メンバ関数
	void WaitUpdate();
	void MoveUpdate();
	void ControlUpdate();

	unique_ptr<Note> note;
};
