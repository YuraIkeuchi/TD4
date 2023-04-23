#pragma once
#include <any>
#include"IKEFBXObject3d.h"
#include<memory>
#include "CharactorManager.h"
#include "Input.h"
#include "Bullet.h"
using namespace DirectX;
class Player:public CharactorManager
{
public:
	Player(XMFLOAT3 StartPos = {0.f,0.f,0.f});
	~Player()override;

private:
	//キーのインスタンス取得用
	Input* input;
private:
	//体力
	static int HP;

	//アニメーション管理用
	bool m_LoopFlag = true;
	int m_AnimationSpeed = 1;
	bool m_StopFlag=false;

	static void (Player::* stateTable[])();
private:
	std::unique_ptr<IKEFBXObject3d>Model;
public:
	//初期化
	void Initialize()override;
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
		STATE_ATTACK,
		STATE_SHOT
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
	//攻撃諸々
	void Attack();
	//弾を打つ処理
	void Shot();
private:
	void Idle();
private:
	//各アニメーション
	enum class AnimeName
	{
		IDLE=7,
		WALK=5,
		ATTACK=0
	}_animeName;

	void AnimationControl(AnimeName name, const bool& loop, int speed);

private://各クラス
	vector<Bullet*> bullets;
};

