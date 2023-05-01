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
	//探索のはじめ
	void StartSearch(const XMFLOAT3& pos);
	//探索の終わり
	void EndSearch();

private://ステート
	static void (Ghost::* stateTable[])();
private:
	//パーティクル
	void Particle();
	//当たり判定(弾)
	bool BulletCollision();
	//当たり判定(プレイヤー)
	bool PlayerCollision();
	//食料生産
	void BirthGhost();
	//何もない状態
	void None();
	//追従
	void Follow();
	//探索
	void Search();
	//食べ物を運ぶ
	void CarryFood();
public:
	//ゴースト同士の当たり判定
	void GhostCollision(const XMFLOAT3& pos);
	//ゴースト同士の当たり判定
	void NotGhostCollision(const XMFLOAT3& pos);
public://getter setter
	void SetPlayer(Player* player) { this->player.reset(player); }
public:
	//gettersetter
	const bool& GetAlive() { return m_Alive; }
	const bool& GetCatch() { return m_Catch; }
	const bool& GetFollow() { return m_Follow; }
	const bool& GetSearch() { return m_Search; }
	const float& GetLimit() { return m_Limit; }
	void SetCatch(const bool Catch) { m_Catch = Catch; }
	void SetHit(const bool Hit) { m_Hit = Hit; }
	void SetLimit(const float Limit) { m_Limit = Limit; }
private:
	unique_ptr<Player> player;
	bool m_Alive = true;//生存フラグ
	bool m_Catch =false;//捕獲フラグ
	int m_ResPornTimer = 0;//復活の時間
	XMFLOAT3 m_FollowPos = {};//追従先
private:
	//キャラの状態
	enum CharaState
	{
		STATE_NONE,
		STATE_FOLLOW,
		STATE_SEARCH,
	}_charaState;

private:
	XMFLOAT3 m_OldPos = {};
	bool m_Follow = false;
	//追従状態
	enum FollowState {
		Follow_NO,
		Follow_START,
		Follow_END,
	}_followState;
private://探索
	bool m_Search = false;
	XMFLOAT3 m_SearchPos = {};

	//探索状態
	enum SearchState {
		SEARCH_NO,
		SEARCH_START,
		SEARCH_END,
	}_searchState;

private:
	//探索するものの範囲
	float m_Limit = {};

	//円運動に使う変数(追従先のため)
	float m_CircleRadius = 0.0f;
	float m_CircleSpeed = 0.0f;
	float m_CircleScale = 5.0f;
	float m_CirclePosX = 0.0f;
	float m_CirclePosZ = 0.0f;

	bool m_Hit = false;
};
