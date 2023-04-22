#pragma once
#include "BaseActor.h"
#include "IKESprite.h"
#include "IKEObject3d.h"
#include "IKEModel.h"
#include <vector>
#include<windows.h>
#include"Player.h"
#include "UI.h"
#include "EnemyManager.h"
#include "FirstBoss.h"

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
	void PlaySceneInitialize();//プレイシーンのみの初期化
	void ModelDraw(DirectXCommon* dxCommon);
	void FrontDraw(DirectXCommon* dxCommon);
	void BackDraw(DirectXCommon* dxCommon);
	void NormalDraw(DirectXCommon* dxCommon);//普通の描画
	void NormalUpdate();//普通の更新
	void ImGuiDraw(DirectXCommon* dxCommon);
private:
	//OBJ
	unique_ptr<IKEObject3d> objGround;
	unique_ptr<Player>player;
	unique_ptr<EnemyManager> enemymanager;
	unique_ptr<InterBoss> boss;
	unique_ptr<UI>ui;
	std::vector<std::vector<int>> map; //マップチップ(1マップ)
	IKEModel* modelGround = nullptr;
};