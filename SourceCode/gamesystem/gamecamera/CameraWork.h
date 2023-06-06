#pragma once
#include "DebugCamera.h"
#include "Shake.h"
#include <memory>
#include "Feed.h"
#include "InterBoss.h"
#include"Spline.h"
using namespace std;         //  名前空間指定

//カメラの種類
enum CameraState {
	CAMERA_NORMAL,
	CAMERA_LOAD,
	CAMERA_BOSSAPPEAR,
	CAMERA_BOSSDEAD_BEFORE,
	CAMERA_BOSSDEAD_AFTER_FIRST,
	CAMERA_BOSSDEAD_AFTER_SECOND,
};

//ボス登場シーンのカメラの動き(2個目のボスがこの量なだけでボスによってはへらしてもいいよ)
enum AppearCameraType {
	APPEAR_START,
	APPEAR_SECOND,
	APPEAR_THIRD,
	APPEAR_FOURTH,
	APPEAR_FIVE,
	APPEAR_SIX,
	APPEAR_SEVEN,
	APPEAR_EIGHT,
	APPEAR_END,
};

//ゲームのカメラの動き
class CameraWork {
private:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

private:
	static void (CameraWork::* stateTable[])();
public:
	CameraWork(XMFLOAT3 eye = { 2.0f, 30.0f, 2.0f }, XMFLOAT3 target = { 2.0f, 0.0f, 3.0f });
	void Update(DebugCamera* camera);//更新
	void ImGuiDraw();

public:
	void EditorCamera();

private:
	void DefaultCam();//ふつうのカメラ
	void BossAppear();//登場
	void SetBossDead_Before();//撃破
	void SetBossDead_AfterFirst();//撃破時（フェード後）
	void SetBossDead_AfterSecond();//撃破時（フェード後）
	//void SetBoss(InterBoss* boss) { this->boss = boss; }
	//ゲームシーン以外で使うカメラ更新(this変数で変更可能)
	void SpecialUpdate();//更新
public:
	//void EditorCamera();
	void BossDefeatcamera();
	void feedDraw();

	bool GetFeedEnd() { return FeedEndF; }
private:
	bool FeedEndF;
	int DeathTimer;
	bool FeedF;
	bool AppearEndF;
	void SetBossDead();//撃破

	//ゲームシーン以外で使うカメラ更新(this変数で変更可能)
//	void SpecialUpdate();//更新

private://各ボスの登場カメラ
	Spline* spline;
	vector<XMFLOAT3>pointsList;
	void FirstBossAppear();

	void SecondBossAppear();

	void FirstBossDead_AfterFeed();
	//円運動のカメラセットの際のやつ
	void SetCircleCamera();
public:
	bool Finish;
	int Timer_first=1;
	bool StartSpl;
	float RadEffect;
	bool PlusRad;
	float SplineSpeed;
	float GetEffectPower() { return RadEffect; }
	enum FirstBossCamState
	{
		ONE,
		TWO,
		THREE

	}_firstState;
	//getter setter
	bool Feed_Spline;
bool FinishAppear() { if (spline->GetIndex() >= static_cast<int>(pointsList.size() -1))return true; return false; }

	void SetBoss(InterBoss* boss) { this->boss = boss; }

	void SetEye(const XMFLOAT3& eye) { m_eyePos = eye; }
	XMFLOAT3& GetEye() { return m_eyePos; }
	void SetTarget(const XMFLOAT3& target) { m_targetPos = target; }
	XMFLOAT3& GetTarget() { return m_targetPos; }

	bool GetFeedF() { return FeedF; }

	bool GetEndDeath() { return m_EndDeath; }

	bool GetAppearEndF() { return AppearEndF; }

	int GetAppearType() { return m_AppearType; }
	void SetCameraState(const int CameraState) { m_CameraState = CameraState; }

	//シーンネームの取得
	void SetSceneName(std::string name) { SceneName = name; }

	void SetApproach(const bool Approach) { m_Approach = Approach; }

	void SetCameraSkip(const bool CameraSkip) { m_CameraSkip = CameraSkip; }

	bool CameraStateisNormal() { if (m_CameraState != CAMERA_BOSSAPPEAR)return true; return false; }

	int GetCameraState() { return m_CameraState; }
private:
	//クラス
	unique_ptr<Shake> shake = nullptr;
	//イージングの変数
	float m_Frame = 0.0f;
	XMFLOAT3 m_AfterEye = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_AfterTarget = { 0.0f,0.0f,0.0f };
	//視点座標
	XMFLOAT3 m_eyePos = { 0.0f,0.0f,0.0f };
	//注視点座標
	XMFLOAT3 m_targetPos = { 0.0f,0.0f,0.0f };

	InterBoss* boss = nullptr;

	//カメラの状態
	int m_CameraState = 0;

	unique_ptr<Feed> feed;
	//シーンネーム
	std::string SceneName;

	//カメラのタイマー
	int m_CameraTimer = 0;

	//円運動のため
	float m_PI = 3.14f;
	float m_CameraRadius = 0.0f;
	float m_CameraSpeed = 90.0f;
	float m_CameraScale = 20.0f;
	float m_CameraCircleX = 0.0f;
	float m_CameraCircleZ = 0.0f;

	float m_AfterSpeed = {};
	float m_AfterScale = 5.0f;

	int m_AppearType = {};

	//カメラが寄る
	bool m_Approach = false;

	//撃破カメラ終了
	bool m_EndDeath = false;

	//カメラをスキップするか
	bool m_CameraSkip = false;
};