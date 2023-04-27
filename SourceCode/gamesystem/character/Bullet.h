#pragma once
#include "ObjCommon.h"
//言霊クラス
class Bullet :
	public ObjCommon {
protected:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	Bullet();

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
	//弾の動く処理
	void Move();
public:
	//gettersetter
	const bool& GetAlive() { return m_Alive; }
	void SetAngle(const XMFLOAT2& Angle) { m_Angle = Angle; }
	void SetBulletType(const int BulletType) { m_BulletType = BulletType; }
private:
	XMFLOAT2 m_Angle = {};//弾の角度
	float m_AddSpeed = {};//加速度
	int m_BulletType = {};//弾の種類
	int m_Timer = {};//出現時間
	int m_TargetTimer = {};//出現時間の目標
	bool m_Alive = true;//生存フラグ
};
