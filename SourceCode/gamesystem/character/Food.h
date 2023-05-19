#pragma once
#include "ObjCommon.h"
//食べ物のクラス
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
	//パーティクル
	void Particle();
	//当たり判定
	bool Collision();
	//食料生産
	void BirthFood();
	//食料削除
	void DeleteFood();
public://getter setter
public:
	//gettersetter
	const bool& GetAlive() { return m_Alive; }
	const bool& GetLockOn() { return m_LockOn; }
	void SetAlive(const bool Alive) { m_Alive = Alive; }
	void SetLockOn(const bool LockOn) { m_LockOn = LockOn; }
	void SetLimit(const bool Limit) { m_Limit = Limit; }
private:
	bool m_Alive = true;//生存フラグ
	bool m_LockOn = false;
	int m_Timer = 0;

	//制限時間を超えたかどうか
	bool m_Limit = false;
	//削除までの時間
	int m_DeleteTimer = 0;
};
