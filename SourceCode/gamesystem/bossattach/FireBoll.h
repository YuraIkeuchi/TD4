#pragma once
#include "ObjCommon.h"
//火の玉クラス
class FireBoll :
	public ObjCommon {
protected:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	FireBoll();
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

	bool Collide();	//当たり判定

	void Move();

	void BirthParticle();
public:
	//gettersetter
	const bool& GetAlive() { return m_Alive; }

	void SetCircleSpeed(const float CircleSpeed) { m_CircleSpeed = CircleSpeed; }
private:
	bool m_Alive = 0.0f;
	int m_MoveTimer = {};
	float m_CircleSpeed = {};
	float m_CircleScale = {};
	float m_Damage = {};
	float m_Frame = {};
	XMFLOAT3 m_TargetPos = {};
	enum MoveState {
		MOVE_SET,
		MOVE_PLAY,
		MOVE_OMEN,
		MOVE_ATTACK,
		MOVE_END,
	}_MoveState;
};