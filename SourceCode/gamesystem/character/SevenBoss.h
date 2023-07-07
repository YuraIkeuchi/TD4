#pragma once
#include "InterBoss.h"
#include "Shake.h"
#include "BossStunEffect.h"
#include "Poltergeist.h"
#include "AvatarBoss.h"
#include "FireBoll.h"
#include "AbsorptionEffect.h"
class SevenBoss :
	public InterBoss {
public:
	SevenBoss();

	bool Initialize() override;//初期化

	void SkipInitialize() override;//スキップ時の初期化

	void Pause() override;//ポーズ

	void Action() override;//行動

	void AppearAction() override;//ボス登場の固有の処理

	void DeadAction() override;//ボス撃破の固有の処理

	void DeadAction_Throw() override;//ボス撃破の固有の処理 スロー

	void ImGui_Origin() override;//ボスそれぞれのImGui

	void EffecttexDraw(DirectXCommon* dxCommon) override;

	void Draw(DirectXCommon* dxCommon) override;//描画
private:
	void CSVLoad();
	
	//各ボスの行動
	void InterValMove();//インターバル
	void Polter();//ポルターガイスト
	void ThrowBound();//バウンド弾
	void BirthAvatar();//偽物のボス
	void Manipulate();//操る
	void BulletCatch();//弾を吸収
	void FireAttack();//火の玉攻撃
	void BirthFire();//炎生成
	void Stun();//スタン
	void BirthExplosion();

	void BirthPolter(const std::string& PolterName);//ポルターガイストの生成

	void CatchBul(vector<InterBullet*>bullet);

	void BirthParticle();

	//一瞬ボスが消える
	void VanishBoss();

	//モンスターが消える範囲設定
	void VanishCollide(vector<InterBullet*>bullet);

	void DeleteObj();

	//ランダムに動き
	void RandMove();
	//ボスが戻る
	void ReturnBoss();
public:

private:
	//キャラの状態
	enum CharaState
	{
		STATE_INTER,
		STATE_POLTER,
		STATE_BOUND,
		STATE_AVATAR,
		STATE_MANIPULATE,
		STATE_FIRE,
		STATE_CATCH,
		STATE_STUN,
	}_charaState;

	//関数ポインタ
	static void(SevenBoss::* stateTable[])();
private:
	static const int POLTER_NUM = 4;
	static const int FIRE_NUM = 4;
	static const int AVATAR_NUM = 2;
private:
	vector<Poltergeist*> poltergeist;//ポルターガイスト
	vector<FireBoll*> fireboll;//火の玉
	vector<InterBoss*> avatarboss;//偽物のボス
	vector<AbsorptionEffect*> abseffect;//弾幕
	unique_ptr<BossStunEffect> bossstuneffect;

	int m_InterVal = {};

	int m_MoveTimer = {};
	int m_AvatarCount = {};

	//攻撃回数
	int m_AttackCount = {};
	//スタンしたかどうか
	bool m_Stun = false;
	//攻撃の乱数
	int m_AttackRand = {};

	//敵が弾を避けるかどうか
	bool m_Vanish = false;

	//透明のステート
	enum VanishState {
		VANISH_SET,
		VANISH_END,
	}_vanishState;
	//透明化する時間
	float m_VanishFrame = {};
	//糖度
	float m_AfterAlpha = {};
	//透明化する確率
	int m_VanishTarget = {};
	XMFLOAT3 m_AfterPos = {};

	int m_RotTimer = {};
	bool m_StartMani = false;

	//CSV系
	//各インターバルやリミット時間
	vector<int>m_RandAct;

	enum RandState {
		RAND_POLTER,
		RAND_BOUND,
		RAND_AVATAR,
		RAND_MANIPULATE,
		RAND_FIRE,
	};

	int m_ChangeTimer = {};
	float m_AddSpeed = {};
	float m_AddScale = {};

	bool m_Return = false;

	enum ReturnState {
		RETURN_SET,
		RETURN_PLAY,
		RETURN_END,
	}_ReturnState;
};