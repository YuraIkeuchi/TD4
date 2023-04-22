#pragma once
#include "BaseActor.h"
#include <vector>
#include<windows.h>
#include"Player.h"
#include "UI.h"
#include "EnemyManager.h"
#include "FirstBoss.h"
#include "BackObj.h"
/// タイトルシーン
class PlaySceneActor : public BaseActor {
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
	//クラス
	unique_ptr<Player>player;
	unique_ptr<EnemyManager> enemymanager;
	unique_ptr<InterBoss> boss;
	unique_ptr<UI>ui;
	unique_ptr<BackObj> backobj;
	std::vector<std::vector<int>> map; //マップチップ(1マップ)
};