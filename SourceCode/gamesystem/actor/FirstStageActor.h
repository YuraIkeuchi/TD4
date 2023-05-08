#pragma once
#include "BaseActor.h"
#include <vector>
#include<windows.h>
#include"Player.h"
#include "UI.h"
#include "EnemyManager.h"
#include "FirstBoss.h"
#include "BackObj.h"
#include "LoadStageObj.h"
/// タイトルシーン
class FirstStageActor : public BaseActor {
private:
	enum state {
		CONVERSATION=0,
		FIGHT,
		NONE,
	};
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
	
	unique_ptr<EnemyManager> enemymanager;
	unique_ptr<InterBoss> boss;
	unique_ptr<UI>ui;
	unique_ptr<BackObj> backobj;
	unique_ptr<LoadStageObj> loadobj;
	unique_ptr<IKESprite> conversationwindow;
	unique_ptr<IKESprite> blackwindow;
	//��W
	XMFLOAT2 window_pos{ WinApp::window_width/2.f,WinApp::window_height+100 };
	XMFLOAT2 window_size{ 0.f,0.f };

	XMFLOAT4 black_color{ 1.f,1.f,1.f,0.f };

	float maxframe = 20.f;
	float nowframe = 0.f;
	float frame = 0.f;
	int nowstate = NONE;
	bool test = false;
};