#pragma once
#include"IKESprite.h"
#include "InterBoss.h"
#include "JoyStamp.h"
#include "AngerStamp.h"
class SecondBoss :
	public InterBoss {
public:
	SecondBoss();
	bool Initialize() override;//初期化

	void Pause() override;//ポーズ
private:
	void Action() override;//行動


	void ImGui_Origin() override;

	//エフェクト
	void EffecttexDraw(DirectXCommon* dxCommon) override;

	//基本移動
	void Move();
	//攻撃
	void Attack();

private:

	static void (SecondBoss::* stateTable[])();
private:
	//ダメージ食らった処理
	void DamAction();
	//スタンプの生成
	void BirthStamp(const std::string& stampName);
	//スタンプの更新
	void StampUpdate(std::vector<InterStamp*> stamps);
	//スタンプの描画
	void StampDraw(std::vector<InterStamp*> stamps, DirectXCommon* dxCommon);
	//スタンプのImGui
	void StampImGui(std::vector<InterStamp*> stamps);
	
	//　動き方それぞれ //
	void AlterMove();//交互
	void AngerMove();//怒り
	void JoyMove();//喜び
public:

private:
	int m_InterValCount = 0;
	//バトルしているかどうか
	bool m_Buttle = false;
	//イージング
	float m_Frame = {};
	//Y方向に加わる力
	float m_AddPowerY = {};
	//前座標
	XMFLOAT3 m_OldPos = {};
	//インターバル時の座標
	float m_AfterPower = 0.0f;
	float m_AfterRotX = 180.0f;

	//追従関係に使う
	float m_FollowSpeed = 0.0f;
	float m_AfterFollowSpeed = 0.0f;

	//停止時間
	int m_StopTimer = 0;

	//どの行動にするか
	int m_MoveState = {};
private:
	enum InterValState {
		DownState,
		UpState,
	}_InterValState = DownState;
private:
	inline void IsOldPos()
	{
		m_Position = m_OldPos;
	}

private:
	vector<InterStamp*> angerstamps;//怒りのスタンプ
	vector<InterStamp*> joystamps;//喜びのスタンプ

private:
	//キャラの状態
	enum CharaState
	{
		STATE_MOVE,
		STATE_ATTACK,
	}_charaState;

	//動き方の種類
	enum MoveState {
		MOVE_ALTER,//交互
		MOVE_ANGER,//怒りのみ
		MOVE_JOY,//喜びのみ
	};
};