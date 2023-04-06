#pragma once
#include "BaseActor.h"
#include "IKESprite.h"
#include "Helper.h"
/// タイトルシーン
class IntroductionSceneActor : public BaseActor {
public:
	/// 初期化
	void Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) override;
	/// 終了
	void Finalize() override;
	/// 毎フレーム更新
	void Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) override;
	/// 描画
	void Draw(DirectXCommon* dxCommon) override;
	void ModelDraw(DirectXCommon* dxCommon);
	void GameDraw(DirectXCommon* dxCommon);
	void ImGuiDraw(DirectXCommon* dxCommon);
	void FrontDraw();
private://メンバ変数
	unique_ptr<Helper> helper;
};