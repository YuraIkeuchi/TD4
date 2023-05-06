#pragma once
#include <any>
#include"IKEFBXObject3d.h"
#include<memory>
#include "ObjCommon.h"

#include "GhostBullet.h"
#include "AttackBullet.h"
using namespace DirectX;
class Player:public ObjCommon
{
public:
	Player(XMFLOAT3 StartPos = {0.f,0.f,0.f});
	~Player()override;

private:
	//体力
	static int HP;

	//アニメーション管理用
	bool m_LoopFlag = true;
	int m_AnimationSpeed = 1;
	bool m_StopFlag=false;

	static void (Player::* stateTable[])();
public:
	//初期化
	bool Initialize()override;
	//更新
	void Update()override;
	//描画
	void Draw(DirectXCommon* dxCommon)override;
	//ImGui
	void ImGuiDraw();


	//キャラの状態
	enum CharaState
	{
		STATE_IDLE,
		STATE_RUN,
		STATE_GHOST,
		STATE_ATTACKSHOT,
		STATE_SUPERSHOT,
	}_charaState;

private:
	//歩きまたは走り状態
	float velocity;
	//移動方向指定用
	float angle;
	//移動加算値
	float m_AddSpeed;
	//↑の値をCSVから読み込むときの格納用
	std::any sp;

	//あるき処理
	void Walk();
	XMFLOAT3 MoveVECTOR(XMVECTOR v, float angle);

private:
	//弾を打つ処理(ゴーストを捕まえる)
	void GhostShot();
	//弾を打つ処理(攻撃)
	void AttackShot();
	//弾を打つ処理(ため攻撃)
	void SuperShot();
private:
	void Idle();
	//インターバル管理
	void InterVal();
	//インターバルのセット
	void SetInterVal();
	//弾を選ぶ
	void SelectBullet();
	//弾の更新
	void BulletUpdate();
private:
	//各アニメーション
	enum class AnimeName
	{
		IDLE=7,
		WALK=5,
	}_animeName;

	void AnimationControl(AnimeName name, const bool& loop, int speed);
public:
	//当たり判定系
	bool BulletCollide(const XMFLOAT3& pos,const bool Catch);//弾との当たり判定
	bool PlayerCollide(const XMFLOAT3& pos);//プレイヤーとの当たり判定
public:
	const int& GetBulletType() { return m_BulletType; }
private://各クラス
	vector<InterBullet*> ghostbullets;//言霊
	vector<InterBullet*> attackbullets;//攻撃
	//弾関係の変数
	int m_BulletType = {};//弾の種類
	int m_InterVal = {};//弾の発射のインターバル
	int m_RigidityTime = {};//硬直時間
	int m_TargetInterVal = {};//インターバルの目標時間
	int m_TargetRigidityTime = {};//硬直時間の目標時間

	XMFLOAT3 m_FollowPos = {};

	int m_ShotTimer = {};//ショットのチャージ時間

public:
	vector<InterBullet*>GetBulllet_ghost() { return ghostbullets; }
	vector<InterBullet*>GetBulllet_attack() { return attackbullets; }

};

