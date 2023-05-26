﻿#pragma once
#include "BaseActor.h"
#include "EnemyManager.h"
#include "UI.h"
#include "BackObj.h"
#include "LoadStageObj.h"
#include "Conversation.h"
#include"MessageWindow.h"
#include <random>
#include <vector>
#include <windows.h>

/// タイトルシーン
class TutorialSceneActor : public BaseActor {
private:

	enum class state {
		INTORO = 0,
		MOVE,
		TEXT_TALK,
		SPAWNENEMY,
		TEXT_CATCHFOLLOW,
		CATCHFOLLOW,
		TEXT_SHOT,
		SHOT,
		TEXT_CATCHSEACH,
		CATCHSEACH,
		TEXT_CLEAR,
		SPAWNALLENEMY,
		TEXT_LAST,
		MAINTUTORIAL,
		COMPLETE,
	}nowstate_;

	static void (TutorialSceneActor::* stateTable[])();
	void IntroState();
	void MoveState();
	void TextTalkState();
	void SpawnEnemyState();
	void TextCatchFollowState();
	void CatchFollowState();
	void TextShotState();
	void ShotState();
	void TextCatchSeachState();
	void CatchSeachState();
	void TextClearState();
	void SpawnAllEnemyState();
	void TextLastState();
	void MainTutorialState();
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
	void CameraUpdate(DebugCamera* camera);

	bool DebugButton();

	XMFLOAT3 RandomShake(XMFLOAT3 pos,float shakeTimer);
private:
	//クラス
	Input* input = Input::GetInstance();
	unique_ptr<EnemyManager> enemymanager;
	InterEnemy* firstEnemy = nullptr;
	unique_ptr<UI>ui;
	unique_ptr<IKEObject3d> sutepon = nullptr;
	IKEModel* m_Model = nullptr;
	unique_ptr<BackObj> backobj;
	unique_ptr<LoadStageObj> loadobj;
	unique_ptr<Conversation> conversation_;
	unique_ptr<MessageWindow> messagewindow_;

	float shakeTimer = 0;
	int shakeCount = 0;
	bool isShake = false;

	static bool isDebug;

	float maxframe = 20.f;
	float nowframe = 0.f;
	float frame = 0.f;
	int conversation = 0;
	int old_conversation = 0;
	
	int waitTimer = 0;
	float cameraframe = 0.0f;
	const float kCameraFrameMax = 90.0f;

	XMFLOAT3 s_eyepos = {};
	XMFLOAT3 e_eyepos = { 0,0,30.0f};
	XMFLOAT3 s_targetpos = {};
	XMFLOAT3 e_targetpos = { 0,0,50.0f };
private://便利関数
	bool Clear(bool mission,int waitTimerMax);

};