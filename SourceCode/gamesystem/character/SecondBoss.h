#pragma once
#include "InterBoss.h"
#include "JoyStamp.h"
#include "AngerStamp.h"
#include "ShockWave.h"
#include "Predict.h"
#include "Collision.h"
#include "Shake.h"
class SecondBoss :
	public InterBoss {
public:
	SecondBoss();
	bool Initialize() override;//初期化

	void Pause() override;//ポーズ

	void Draw(DirectXCommon* dxCommon) override;//描画
private:
	void Action() override;//行動

	void AppearAction() override;//登場

	void DeadAction() override;//撃破

	void ImGui_Origin() override;

	//エフェクト
	void EffecttexDraw(DirectXCommon* dxCommon) override;

	//基本移動
	void Move();
	//攻撃(スタンプ攻撃)
	void Stamp();
	//ランダム攻撃
	void RandomStamp();
	//当たり判定
	bool Collide();
private:
	//キャラの行動繊維
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
	void ChoiceMove();//動きのチョイス
	void BirthWave();//衝撃波の生成
	void BirthPredict();//予測テクスチャの生成
	//転がるやつ
	void Rolling();
	//転がるやつの共通イージング
	void RollEaseCommn(const XMFLOAT3& AfterPos,const float AddFrame,const float AfterRot);
	//転がる際の方向転換
	void ChangeRot(const float AfterRot);
	//スタンプ攻撃の初期化
	void StampInit(const int AttackNumber,const bool Random);
	//動きの初期化
	void MoveInit(const std::string& HighState);

	//テクスチャの更新
	void MarkUpdate();

private:
	//移動回数の計算
	int m_MoveCount = 0;
	//バトルしているかどうか
	bool m_Buttle = false;
	//イージング
	float m_Frame = {};
	//前座標
	XMFLOAT3 m_OldPos = {};
	//X方向の回転
	float m_AfterRotX = 180.0f;

	//追従関係に使う
	float m_FollowSpeed = 0.0f;
	float m_AfterFollowSpeed = 0.0f;

	//停止時間
	int m_StopTimer = 0;

	//どの行動にするか
	int m_MoveState = {};

	//加算されるフレーム数
	float m_AddFrame = 0.01f;

	//イージング後の位置
	XMFLOAT3 m_AfterPos = {};

	//シェイク用変数
	XMFLOAT3 m_ShakePos = { 0.0f,0.0f,0.0f };
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
	vector<ShockWave*> shockwaves;//衝撃波
	vector<Predict*> predicts;
	unique_ptr<Shake> shake;//シェイク
private:
	//キャラの状態
	enum CharaState
	{
		STATE_MOVE,
		STATE_STAMP,
		STATE_RANDOM,
		STATE_ROLL,
	}_charaState;

	//動き方の種類
	enum MoveState {
		MOVE_ALTER,//交互
		MOVE_ANGER,//怒りのみ
		MOVE_JOY,//喜びのみ
		MOVE_CHOICE,//どの動きかの選択するターン
	};


	OBB m_OBB1 = {};
	OBB m_OBB2 = {};

private:
	//プレス
	enum PressType {
		PRESS_START,
		PRESS_SET,
		PRESS_ATTACK,
		PRESS_SHAKE,
		PRESS_RETURN,
		PRESS_END,
	};

	int m_PressType;
	//ランダム攻撃
	enum RandomType {
		RANDOM_START,
		RANDOM_SET,
		RANDOM_ATTACK,
		RANDOM_END,
	};

	int m_RandomType;

	//転がる攻撃
	enum RollType {
		ROLL_ONE,
		ROLL_SECOND,
		ROLL_THIRD,
		ROLL_FOURTH,
		ROLL_FIVE,
		ROLL_SIX,
		ROLL_END,
	};

	int m_RollType;

	//上昇度
	float m_AddPower = 0.0f;
	//重力加速度
	float m_Gravity = 0.02f;

	//方向転換するためのもの
	bool m_ChangeRot = false;
	float m_RotFrame = 0.0f;
private:
	unique_ptr<IKETexture> mark;
	XMFLOAT4 m_MarkColor = { 1.0f,1.0f,1.0f,0.0f };
private:
	enum AppearState {
		APPEAR_START,
		APPEAR_SET,
		APPEAR_JOY,
		APPEAR_ANGER,
		APPEAR_END,
	};

	int m_AppearState = APPEAR_START;

	int m_AppearTimer = {};
};