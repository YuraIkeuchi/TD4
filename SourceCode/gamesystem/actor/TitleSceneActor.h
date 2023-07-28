#pragma once
#include "BaseActor.h"
#include "IKESprite.h"
#include"Font.h"
#include "Helper.h"
#include "Feed.h"
/// タイトルシーン
class TitleSceneActor : public BaseActor {
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
private:
	static const int TITLE_MAX = 3;//タイトルパーツの最大数
	static const int MODE_MAX = 3;//モードパーツの最大数
	static const int EXPLAIN_MAX = 3;//説明文の最大数
private://メンバ変数
	//スプライト
	unique_ptr<IKESprite> TitleSprite;
	unique_ptr<IKESprite> TitleWordSprite;
	Feed* feed;
	bool feedF;
	XMFLOAT2 size = { 708.0f,354.0f };
	XMFLOAT2 pos = { 50.0f,110.0f };
	float frame = 0.0f;
};