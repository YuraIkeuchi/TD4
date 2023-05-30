﻿#pragma once
#include "BaseActor.h"
#include <vector>
#include<windows.h>
#include "BossText.h"
/// タイトルシーン
class SecondStageActor : public BaseActor {
public:
	/// 初期化
	void Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) override;
	/// 終了
	void Finalize() override;
	/// 毎フレーム更新
	void Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) override;
	/// 描画
	void Draw(DirectXCommon* dxCommon) override;
	void FrontDraw(DirectXCommon* dxCommon);
	void BackDraw(DirectXCommon* dxCommon);
	void ImGuiDraw(DirectXCommon* dxCommon);
private:

	void IntroUpdate(DebugCamera* camera)override;		//登場シーン
	void MainUpdate(DebugCamera* camera)override;		//バトルシーン
	void FinishUpdate(DebugCamera* camera)override;		//撃破シーン

	unique_ptr<BossText> text_;

private:

	//導入シーン
	enum class AppState : int {
		ANGER_START,
		ANGER_SECOND,
		JOY_START,
		JOY_SECOND,
		JOY_THIRD,
		SELECT_EMO,
		EMO_JOY,
		EMO_JOY2,
		EMO_ANGER,
		EMO_ANGER2,
	};

	//シーンでの遷移
	AppState m_AppState = AppState::ANGER_START;
};