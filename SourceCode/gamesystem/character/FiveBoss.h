#pragma once
#include "InterBoss.h"
#include "JoyStamp.h"
#include "AngerStamp.h"
#include "ShockWave.h"
#include "Predict.h"
#include "Collision.h"
#include "Shake.h"
#include "ConfuEffect.h"
#include "NormalAttack.h"
#include "NoteEffect.h"
#include "ShotAttack.h"
#include"SmashShotAttack.h"
#include"ShadowSlashAttack.h"
class Spline;

class FiveBoss : 
	public InterBoss
{
public:
	FiveBoss();

	bool Initialize() override;//初期化

	void SkipInitialize() override;//スキップ時の初期化

	void Pause() override;//ポーズ

	void Action() override;//行動

	void AppearAction() override;//ボス登場の固有の処理

	void DeadAction() override;//ボス撃破の固有の処理

	void DeadAction_Throw() override;//ボス撃破の固有の処理 スロー

	void ImGui_Origin() override;//ボスそれぞれのImGui

	void InitAwake() override;//ボスの覚醒

	void EffecttexDraw(DirectXCommon* dxCommon) override;

	void Draw(DirectXCommon* dxCommon) override;//描画

	void EndRollAction() override;//エンドロールの更新
private:

private:
	//CSV読み込み系
	void CSVLoad();
	//死んだときのパーティクル
	void DeathParticle();
private:
	static const int CD_NUM = 4;
private:
	//各クラス
	unique_ptr<ConfuEffect> confueffect;
	unique_ptr<NoteEffect> noteeffect;
	//キャラの状態
	enum CharaState
	{
		STATE_INTER,
		STATE_CHOICE,
		STATE_ROCKON,
		STATE_RAND,
		STATE_HIT,
		STATE_END
	};

	//停止時間
	int m_StopTimer = 0;
	//どの行動にするか
	int m_MoveState = {};


	int _charaState = STATE_INTER;

	//CSV系
	int m_ChoiceInterval = {};

	//イージング後の位置
	XMFLOAT3 m_AfterPos = {};
	//X方向の回転
	XMFLOAT3 m_AfterRot = { 0.0f,0.0f,0.0f };
	float m_Frame = {};

	int m_RotCount = 0;
	int m_RotTimer = 0;

	enum AreaState {
		AREA_SET,
		AREA_STOP,
		AREA_END,
	};

	int m_AreaState = AREA_SET;
	float SplineSpeed = false;

	//動きのインターバル
	int m_MoveInterVal = {};
	//行動終了の数
	int m_EndCount = {};
	//キャッチしたCDの数
	int m_CatchCount = {};
	//ボスがプレイヤーから逃げる時間
	int m_EndTimer = {};

	//棘の的に使う
	float m_Angle = 0.0f;
	float m_Angle2 = 0.0f;
	//二点の距離
	float m_Length = {};

	//円運動
	float m_CircleScale = 30.0f;
	float m_CircleSpeed = {};

	//弾幕の種類
	int m_BarraRand = {};

	int m_AttackRand = {};

	//CSV系
	//各インターバルやリミット時間
	vector<int>m_Limit;

	enum LimitState {
		LIMIT_BASE,
		LIMIT_CONFU,
		LIMIT_STRONG_CONFU,
		LIMIT_BARRA,
	};

	//移動力
	float m_FollowSpeed = {};
	ShotAttack* shot;
	NormalAttack* normal;
	SmashShotAttack* smash;
	ShadowSlashAttack* slash;
	static void (FiveBoss::* attackTable[])();
	enum ActionPhase
	{
		MOVE,
		ATTACK_SHOT,
		ATTACK_NORMAL,
		ATTACK_IMPACT,
		ATTACK_SLASH,
	}_aPhase=ATTACK_SHOT;

	void ActionSet(ActionPhase phase,InterAttack*attack);
	int RandAction;
	int ActionTimer;
	size_t bonesize;
	std::vector<XMFLOAT3> bonepos;;
	std::vector<XMMATRIX> bonemat;;
	std::vector<XMFLOAT4> s_color;
	std::vector<XMFLOAT4>e_color;
	std::vector<float> s_scale;
	std::vector<float> e_scale;
	std::vector<int> m_Life;

	int JudgAttack;
	inline void Shot() { shot->Upda(); }
	inline void Normal() { normal ->Upda(); }
	inline void Smash() {smash->Upda(); }
	inline void Slash() { slash->Upda(); }

	void MatTranstoPos(XMMATRIX trans, XMFLOAT3& m_Pos);
};


