#pragma once
#include "ObjCommon.h"
#include "ViewBullet.h"
#include "GhostBullet.h"
#include "AttackBullet.h"
#include "PlayerAttach.h"
#include "CollisionPrimitive.h"
#include "BreakEffect.h"
#include <any>
using namespace DirectX;
class Player:public ObjCommon
{
public:
	static Player* GetInstance();

private:

	static void (Player::* stateTable[])();
public:
	void InitState(const XMFLOAT3& pos);
	//初期化
	bool Initialize()override;
	//更新
	void Update()override;
	//描画
	void Draw(DirectXCommon* dxCommon)override;
	//ImGui
	void ImGuiDraw();
	//ボス登場シーンの動き
	void AppearUpdate();
	//ボス撃破シーンの動き
	void DeathUpdate();
	//キャラの状態
	enum CharaState
	{
		STATE_IDLE,
		STATE_RUN,
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
	//弾の生成
	void BirthShot(const std::string& bulletName, bool Super);
private:
	void Idle();
	//インターバル管理
	void InterVal();
	//弾のリセット
	void ResetBullet();
	//弾の管理
	void Bullet_Management();
	//弾の更新
	void BulletUpdate(std::vector<InterBullet*> bullets);
	//弾の描画
	void BulletDraw(std::vector<InterBullet*> bullets, DirectXCommon* dxCommon);
	//銃の処理
	void SutoponUpdate();
	//ダメージパーティクル
	void BirthParticle();
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
	bool PlayerCollide(const XMFLOAT3& pos);//プレイヤーとの当たり判定
	//プレイヤーの当たった瞬間
	void PlayerHit(const XMFLOAT3& pos);
	//弾かれる処理
	void ReBound();
	//
	float GetPercentage();
public:
	//gettersetter
	const int& GetBulletType() { return m_BulletType; }
	const int& GetDamageInterVal() { return m_DamageInterVal; }
	const bool& GetIsShotNow() { return isShotNow; }
	const bool& GetSkip() { return m_Skip; }
	const bool& GetConfu() { return m_Confu; }

	void SetHP(float hp) { m_HP = hp; };
	float GetHP() { return m_HP; }
	float GetMaxHP() { return m_MaxHP; }

	void SetCanShot(const bool m_canShot) {this->m_canShot = m_canShot;}
	void SetConfu(const bool Confu) { this->m_Confu = Confu; }
	void SetConfuTimer(const int ConfuTimer) { this->m_ConfuTimer = ConfuTimer; }
	void SetSkip(const bool Skip) { this->m_Skip = Skip; }

private://各クラス
	vector<InterBullet*> ghostbullets;//言霊
	vector<InterBullet*> attackbullets;//攻撃
	unique_ptr<InterBullet> viewbullet;//可視化の弾
	unique_ptr<PlayerAttach> playerattach;//プレイヤーの装備
	vector<InterEffect*> effects;//エフェクト
	//弾関係の変数
	int m_BulletType = {};//弾の種類
	int m_InterVal = {};//弾の発射のインターバル
	int m_RigidityTime = {};//硬直時間
	int m_TargetInterVal = {};//インターバルの目標時間
	int m_TargetRigidityTime = {};//硬直時間の目標時間
	//体力
	float m_HP = 0.0f;
	float m_MaxHP = 0.0f;

	XMFLOAT3 m_FollowPos = {};

	float m_ChargePower = {};//弾のチャージ
	int m_ChargeType = {};
	bool m_canShot = true;
	//ダメージ
	bool m_Damage = false;
	int m_DamageInterVal = {};

	XMFLOAT2 m_BoundPower = {};
	
	//弾の数
	int m_BulletNum = 0;

	bool m_Confu = false;
	int m_ConfuTimer = 0;

	bool m_Skip = false;

public:
	vector<InterBullet*>GetBulllet_ghost() { return ghostbullets; }
	vector<InterBullet*>GetBulllet_attack() { return attackbullets; }

private:
	bool isStop;
	bool isShotNow=false;
	XMFLOAT3 OldPos;
public:
	void MoveStop(bool f) { isStop = f; }
	void isOldPos();

	void RecvDamage(float Damage);
	//弾の全削除
	void BulletDelete();
};