#pragma once
#include "BaseActor.h"
#include "IKESprite.h"
#include "Helper.h"
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
	void ModelDraw(DirectXCommon* dxCommon);
	void GameDraw(DirectXCommon* dxCommon);
	void ImGuiDraw(DirectXCommon* dxCommon);
	void FrontDraw();
private:
	static const int TITLE_MAX = 3;//タイトルパーツの最大数
	static const int MODE_MAX = 3;//モードパーツの最大数
	static const int EXPLAIN_MAX = 3;//説明文の最大数
private://メンバ変数
	//クラス
	unique_ptr<Helper> helper;
	//スプライト
	unique_ptr<IKESprite> TitleSprite;
};