#pragma once
#include "BaseActor.h"
#include "IKESprite.h"
#include "IKEObject3d.h"
#include "EndRollObj.h"
/// タイトルシーン
class EndRollActor : public BaseActor {
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

private://メンバ変数
	unique_ptr<EndRollObj> endobj;
	//ポストエフェクトの値
	float m_Sepia = 0.0f;
	//フレーム
	float m_Frame = 0.0f;

	int m_EndTimer = {};
};
