﻿#pragma once
#include "BaseActor.h"
#include <vector>
#include<windows.h>
#include "UI.h"
#include "EnemyManager.h"
#include "FirstBoss.h"
#include "BackObj.h"
#include "LoadStageObj.h"
#include"Font.h"
/// タイトルシーン
class TutorialSceneActor : public BaseActor {
private:
	enum class state {
		INTORO = 0,
		MOVE,
		CONVERSATION_CATCH,
		CATCHGHORST,
		COMPLETE,
	}nowstate_;

	static void (TutorialSceneActor::* stateTable[])();
	void IntroState();
	void MoveState();
	void ConversationCatchState();
	void CatchGhorstState();
	void CompleteState();


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
	Input* input = Input::GetInstance();

	unique_ptr<EnemyManager> enemymanager;
	unique_ptr<InterBoss> boss;
	unique_ptr<UI>ui;
	unique_ptr<BackObj> backobj;
	unique_ptr<LoadStageObj> loadobj;
	unique_ptr<IKESprite> conversationwindow;
	unique_ptr<IKESprite> blackwindow;
	unique_ptr<IKESprite> girl;
	unique_ptr<Font> font_;
	unique_ptr<Font> secondrow_;
	//��W
	XMFLOAT2 window_pos{ WinApp::window_width / 2.f,WinApp::window_height + 100 };
	XMFLOAT2 window_size{ 0.f,0.f };

	XMFLOAT4 black_color{ 1.f,1.f,1.f,0.f };
	XMFLOAT4 girl_color{ 1.5f,1.5f,1.5f,0.f };

	float maxframe = 20.f;
	float nowframe = 0.f;
	float frame = 0.f;
	bool test = false;
	wchar_t* girlward;
	wchar_t* ward;
	int conversation = 0;
};