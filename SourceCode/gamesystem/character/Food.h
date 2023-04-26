#pragma once
#include "ObjCommon.h"
//背景の岩など共通背景のクラス
class Food :
	public ObjCommon {
protected:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	Food();

	bool Initialize() override;//初期化
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(DirectXCommon* dxCommon) override;

	/// <summary>
	/// ImGui描画
	/// </summary>
	void ImGuiDraw();
private:

public:
	//gettersetter
	const bool& GetAlive() { return m_Alive; }
	void SetAngle(const XMFLOAT2& Angle) { m_Angle = Angle; }
	void SetFoodType(const int FoodType) { m_FoodType = FoodType; }
private:
	XMFLOAT2 m_Angle = {};//弾の角度
	float m_AddSpeed = {};//加速度
	int m_FoodType = {};//弾の種類
	int m_Timer = {};//出現時間
	int m_TargetTimer = {};//出現時間の目標
	bool m_Alive = true;//生存フラグ
};
