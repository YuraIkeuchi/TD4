#pragma once
#include "ObjCommon.h"
using namespace std;         //  名前空間指定

//弾の基底クラス
class PlayerAttach :
	public ObjCommon {
protected:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	PlayerAttach();
	//初期化
	bool Initialize()override;
	//更新
	void Update()override;
	//描画
	void Draw(DirectXCommon* dxCommon)override;
	//ImGui既定
	void ImGuiDraw();

public:
	//gettersetter
	void SetAfterAngle(const XMFLOAT2& AfterAngle) { m_AfterAngle = AfterAngle; }
	//void SetAfterRot(const float AfterRotY) { m_AfterRotY = AfterRotY; }
public:
	XMFLOAT2 m_AfterAngle = {};//弾の角度
	XMFLOAT2 m_Angle = {};//弾の角度
	float m_AfterRotY = {};
	float m_AddSpeed = {};//加速度
	float m_SinAngle = {};
	float m_SinAngle2 = {};
};