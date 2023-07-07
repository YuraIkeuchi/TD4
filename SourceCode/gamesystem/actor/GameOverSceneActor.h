#pragma once
#include "BaseActor.h"
#include "IKESprite.h"
#include "IKEObject3d.h"

/// タイトルシーン
class GameOverSceneActor : public BaseActor {
public:
	/// 初期化
	void Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) override;
	/// 終了
	void Finalize() override;
	/// 毎フレーム更新
	void Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) override;
	/// 描画
	void Draw(DirectXCommon* dxCommon) override;
	void BackDraw(DirectXCommon* dxCommon);
	void ImGuiDraw(DirectXCommon* dxCommon);
	void FrontDraw();

	void IntroUpdate(DebugCamera* camera) override;
	void MainUpdate(DebugCamera* camera) override;
	void FinishUpdate(DebugCamera* camera) override;


	string NextStageName();
private://メンバ変数
	string str = "";
	//スプライト
	unique_ptr<IKESprite> ClearSprite = nullptr;
	unique_ptr<IKEObject3d> SutoponObj = nullptr;


};
