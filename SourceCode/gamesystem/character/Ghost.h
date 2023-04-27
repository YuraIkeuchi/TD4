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
	//当たり判定
	bool Collision();
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
private:
	unique_ptr<Player> player;
	bool m_Alive = true;//生存フラグ
	int m_Timer = 0;
	int m_Type = 0;
private:
	//キャラの状態
	enum CharaState
	{
		STATE_NONE,
		STATE_FOLLOW,
		STATE_SEARCH,
	}_charaState;
};
