#pragma once
#include "DebugCamera.h"
#include "Camera.h"
#include "Shake.h"
#include <memory>

#include "Feed.h"
#include "InterBoss.h"
using namespace std;         //  名前空間指定

enum CameraState {
	CAMERA_NORMAL,
	CAMERA_LOAD,
	CAMERA_BOSSAPPEAR,
	CAMERA_BOSSDEAD_FIRST,
	CAMERA_BOSSDEAD_SECOND
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
	void DefaultCam();//ふつうのカメラ
	void BossAppear();//登場
	void SetBossDead_Cam();//撃破
	void SetBossDead_Act();//撃破時（フェード後）
	void SetBoss(InterBoss* boss) { this->boss = boss; }
	//ゲームシーン以外で使うカメラ更新(this変数で変更可能)
	void SpecialUpdate();//更新
public:
	void EditorCamera();
	void BossDefeatcamera();
	void feedDraw();

	bool GetFeedEnd() { return FeedEndF; }
private:
	bool FeedEndF;
	int DeathTimer;
	bool FeedF;
public:
	//getter setter
	void SetEye(const XMFLOAT3& eye) { m_eyePos = eye; }
	XMFLOAT3& GetEye() { return m_eyePos; }
	void SetTarget(const XMFLOAT3& target) { m_targetPos = target; }
	XMFLOAT3& GetTarget() { return m_targetPos; }

	bool GetFeedF() { return FeedF; }
	void SetCameraState(const int CameraState) { m_CameraState = CameraState; }
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

	int m_CameraState = 0;

	unique_ptr<Feed> feed;
};