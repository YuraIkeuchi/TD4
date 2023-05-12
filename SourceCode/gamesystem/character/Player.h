#pragma once
#include <any>
#include"IKEFBXObject3d.h"
#include<memory>
#include "ObjCommon.h"
#include "ViewBullet.h"
#include "GhostBullet.h"
#include "AttackBullet.h"
using namespace DirectX;
class Player:public ObjCommon
{
public:
	static Player* GetInstance();

private:
	
	//アニメーション管理用
	bool m_LoopFlag = true;
	int m_AnimationSpeed = 1;
	bool m_StopFlag=false;

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
	//弾の削除
	void DeleteBullet();


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
	//gettersetter
	const int& GetBulletType() { return m_BulletType; }

	void SetHP(float hp) { m_HP = hp; };
	float GetHP() { return m_HP; }
private://各クラス
	vector<InterBullet*> ghostbullets;//言霊
	vector<InterBullet*> attackbullets;//攻撃
	unique_ptr<InterBullet> viewbullet;//可視化の弾
	//弾関係の変数
	int m_BulletType = {};//弾の種類
	int m_InterVal = {};//弾の発射のインターバル
	int m_RigidityTime = {};//硬直時間
	int m_TargetInterVal = {};//インターバルの目標時間
	int m_TargetRigidityTime = {};//硬直時間の目標時間
	//体力
	float m_HP = 0.0f;

	XMFLOAT3 m_FollowPos = {};

	int m_ShotTimer = {};//ショットのチャージ時間

public:
	vector<InterBullet*>GetBulllet_ghost() { return ghostbullets; }
	vector<InterBullet*>GetBulllet_attack() { return attackbullets; }

private:
	bool isStop;
	XMFLOAT3 OldPos;
public:
	void MoveStop(bool f) { isStop = f;; }
	void isOldPos();

	void RecvDamage(int Damage);
};

