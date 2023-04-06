#pragma once
#include "DebugCamera.h"
#include "Camera.h"
#include "Shake.h"
#include <memory>
#include "Helper.h"
using namespace std;         //  名前空間指定
//カメラの種類
enum CameraType {
	CameraTitle,
	CameraIntroduction,
	CameraNormal,
	CameraBossApp,
	CameraBossEnd,
	CameraClear,
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

public:
	CameraWork();
	void Update(DebugCamera* camera);//更新
	void ImGuiDraw();
public:
	//getter setter

private:
	//クラス
	unique_ptr<Helper> helper;
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