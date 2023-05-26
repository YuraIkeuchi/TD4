#pragma once
#include "DebugCamera.h"
#include "Camera.h"
#include "Shake.h"
#include <memory>

#include "InterBoss.h"
using namespace std;         //  名前空間指定

//ゲームのカメラの動き
class CameraWork {
private:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	CameraWork(XMFLOAT3 eye = { 2.0f, 30.0f, 2.0f }, XMFLOAT3 target = { 2.0f, 0.0f, 3.0f });
	void Update(DebugCamera* camera);//更新
	void ImGuiDraw();
	void DefaultCam();
	void SetBossCam(InterBoss* boss);
	//ゲームシーン以外で使うカメラ更新(this変数で変更可能)
	void SpecialUpdate(DebugCamera* camera);//更新
public:
	void EditorCamera();
public:
	//getter setter
	void SetEye(const XMFLOAT3 eye) { m_eyePos = eye; }
	XMFLOAT3 GetEye() { return m_eyePos; }
	void SetTarget(const XMFLOAT3 target) { m_targetPos = target; }
	XMFLOAT3 GetTarget() { return m_targetPos; }
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
};