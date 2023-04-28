#pragma once
#include "ObjCommon.h"
#include "Player.h"
//ゴーストクラス
class Ghost :
	public ObjCommon {
protected:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	Ghost();

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
private://ステート
	static void (Ghost::* stateTable[])();
private:
	//パーティクル
	void Particle();
	//当たり判定(弾)
	bool BulletCollision();
	//当たり判定(プレイヤー)
	bool PlayerCollision();
	//ゴースト同士の当たり判定
	bool GhostCollision(const XMFLOAT3& pos);
	//食料生産
	void BirthGhost();
	//何もない状態
	void None();
	//追従
	void Follow();
	//探索
	void Search();

public://getter setter
	void SetPlayer(Player* player) { this->player.reset(player); }
public:
	//gettersetter
	const bool& GetAlive() { return m_Alive; }
	void Setma(const bool ma) { m_ma = ma; }
private:
	unique_ptr<Player> player;
	bool m_Alive = true;//生存フラグ
	bool m_Catch = true;//捕獲フラグ
	int m_Timer = 0;
	XMFLOAT3 m_BasePos = {};
	bool m_ma = false;
private:
	//キャラの状態
	enum CharaState
	{
		STATE_NONE,
		STATE_FOLLOW,
		STATE_SEARCH,
	}_charaState;

	//円運動(仮)
	float m_CircleSpeed = 0.0f;
	float m_CircleScale = 5.0f;

private:
	XMFLOAT3 m_OldPos = {};
	bool m_Follow = false;
};
