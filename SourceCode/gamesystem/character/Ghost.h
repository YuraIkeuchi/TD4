#pragma once
#include "ObjCommon.h"
#include "InterBullet.h"
#include "Collision.h"
#include "CollisionPrimitive.h"
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
	void Particle();
	//当たり判定(プレイヤー)
	bool PlayerCollision();
	//食料生産
	void BirthGhost();
	//何もない状態
	void None();
	//生まれる状態
	void Spawm();
	//追従
	void Follow();
	//探索
	void Search();
	//食べ物を運ぶ
	void CarryFood();
	bool CollideBullet(vector<InterBullet*>bullet);
public:
	//ゴースト同士の当たり判定
	void GhostCollision(const XMFLOAT3& pos);
public://getter setter
public:
	//gettersetter
	const bool& GetAlive() { return m_Alive; }
	const bool& GetCatch() { return m_Catch; }
	const bool& GetFollow() { return m_Follow; }
	const bool& GetSearch() { return m_Search; }
	const bool& GetIsVerse() { return isVerse; }
	const float& GetLimit() { return m_Limit; }
	void SetIsVerse(const bool isVerse) { this->isVerse = isVerse; }
	void SetCatch(const bool Catch) { m_Catch = Catch; }
	void SetAlive(const bool Alive) { m_Alive = Alive; }
	void SetLimit(const float Limit) { m_Limit = Limit; }
private:
	bool m_Alive = true;//生存フラグ
	bool m_Catch = false;//捕獲フラグ
	bool isVerse = true;//リスポーンフラグ
	int m_ResPornTimer = 0;//復活の時間
	XMFLOAT3 m_FollowPos = {};//追従先
	XMFLOAT3 m_OBBScale = {};//OBB用の大きさ
private:
	//キャラの状態
	enum CharaState {
		STATE_NONE,
		STATE_SPAWN,
		STATE_FOLLOW,
		STATE_SEARCH,
	}_charaState = CharaState::STATE_NONE;

private:
	IKEModel* model_follow = nullptr;
	IKEModel* model_seach = nullptr;

	XMFLOAT3 m_OldPos = {};
	bool m_Follow = false;
	//追従状態
	enum FollowState {
		Follow_NO,
		Follow_START,
		Follow_END,
	}_followState = FollowState::Follow_NO;
private://探索
	bool m_Search = false;
	XMFLOAT3 m_SearchPos = {};

	float noneTimer = 0.0f;

	float m_SpawnTimer = 0.f;
	float kSpawnTimerMax = 60.f;

	//探索状態
	enum SearchState {
		SEARCH_NO,
		SEARCH_START,
		SEARCH_END,
	}_searchState = SearchState::SEARCH_NO;

	int m_SearchTimer = 0;
private:
	//探索するものの範囲
	float m_Limit = {};

	bool m_Hit = false;

	OBB m_OBB1 = {}, m_OBB2 = {};
};
