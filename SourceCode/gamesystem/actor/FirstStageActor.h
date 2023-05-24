#pragma once
#include "BaseActor.h"
#include <vector>
#include<windows.h>
#include "UI.h"
#include "EnemyManager.h"
#include "FirstBoss.h"
#include "BackObj.h"
#include "LoadStageObj.h"
#include"Font.h"
#include"Feed.h"

/// タイトルシーン
class FirstStageActor : public BaseActor {
private:
	enum state {
		CONVERSATION=0,
		FIGHT,
		NONE,
	};
	unique_ptr<Feed> feed;
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
private:

	float maxframe = 20.f;
	float nowframe = 0.f;
	float frame = 0.f;
	int nowstate = NONE;
	bool test = false;
};