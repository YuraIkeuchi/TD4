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



private:
	//クラス
	Input* input = Input::GetInstance();

	InterEnemy* firstEnemy = nullptr;

	unique_ptr<EnemyManager> enemymanager;
	unique_ptr<InterBoss> boss;
	unique_ptr<UI>ui;
	unique_ptr<BackObj> backobj;
	unique_ptr<LoadStageObj> loadobj;
	unique_ptr<IKESprite> conversationwindow;
	unique_ptr<IKESprite> blackwindow;
	unique_ptr<IKESprite> girl;
	unique_ptr<IKESprite> megahon;
	unique_ptr<Font> firstrow_;
	unique_ptr<Font> secondrow_;
	unique_ptr<Font> thardrow_;

	//��W
	XMFLOAT2 window_pos{ WinApp::window_width / 2.f,WinApp::window_height + 100 };
	XMFLOAT2 window_size{ 0.f,0.f };

	XMFLOAT4 black_color{ 1.f,1.f,1.f,0.f };
	XMFLOAT4 girl_color{ 1.5f,1.5f,1.5f,0.f };
	XMFLOAT4 sutopon_color{ 1.f,1.f,1.f,0.f };

	float maxframe = 20.f;
	float nowframe = 0.f;
	float frame = 0.f;
	bool test = false;
	wchar_t* ward;
	int conversation = 0;
	int old_conversation = 0;
	
	int waitTimer = 0;
	float cameraframe = 0.0f;
	const float kCameraFrameMax = 90.0f;
private://便利関数
	bool Clear(bool mission,int waitTimerMax);

};