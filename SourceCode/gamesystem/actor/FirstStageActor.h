
#pragma once
#include "BaseActor.h"
#include "LoadStageObj.h"
#include"Font.h"
#include"Feed.h"

/// タイトルシーン
class FirstStageActor : public BaseActor {
	Feed*feedn;
	bool feedF;
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
	void ColEnemy(std::vector<InterEnemy*> enelist);
private:
};