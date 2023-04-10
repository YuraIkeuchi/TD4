#pragma once
#include <any>

#include"DirectXCommon.h"
#include <DirectXMath.h>
#include"IKEFBXObject3d.h"
#include<memory>
#include "Input.h"
using namespace DirectX;
class Player
{
public:
	Player(XMFLOAT3 StartPos = {0.f,0.f,0.f});
	~Player();

private:
	//キーのインスタンス取得用
	Input* input;

private:
	//基礎パラメータ
	XMFLOAT3 Position;
	XMFLOAT3 Rotation;
	XMFLOAT3 Scale;

	//体力
	static int HP;

	//アニメーション管理用
	bool m_LoopFlag = true;
	int m_AnimationSpeed = 1;
	bool m_StopFlag=false;

private:
	std::unique_ptr<IKEFBXObject3d>Model;
public:
	void Init();
	void Upda();
	void Draw(DirectXCommon* dxCommon);

	//キャラの状態
	enum CharaState
	{
		STATE_IDLE,
		STATE_RUN,
		STATE_ATTACK
	}_charaState;

	static void (Player::* stateTable[])();

private:
	//歩きまたは走り状態
	float velocity;
	//移動方向指定用
	float angle;
	//あるき処理
	void Walk();
	XMFLOAT3 MoveVECTOR(XMVECTOR v, float angle);
	//移動加算値
	float m_AddSpeed;
	//↑の値をCSVから読み込むときの格納用
	std::any sp;
private:
	//攻撃諸々
	void Attack();
private:
	void Idle();
private:
	//各アニメーション
	enum class AnimeName
	{
		IDLE=7,
		ATTACK=0
	}_animeName;

	void AnimationControl(AnimeName name, const bool& loop, int speed);
};

