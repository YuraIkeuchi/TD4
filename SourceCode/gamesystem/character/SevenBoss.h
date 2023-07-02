#pragma once
#include "InterBoss.h"
#include "Shake.h"
#include "Poltergeist.h"
#include "AvatarBoss.h"
#include "BossStunEffect.h"
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
	void BulletCatch();//弾を吸収
	void Stun();//スタン
	void BirthExplosion();

	void BirthPolter(const std::string& PolterName);//ポルターガイストの生成

	void CatchBul(vector<InterBullet*>bullet);
private:
	//キャラの状態
	enum CharaState
	{
		STATE_INTER,
		STATE_POLTER,
		STATE_BOUND,
		STATE_AVATAR,
		STATE_CATCH,
		STATE_STUN,
	}_charaState;

	//関数ポインタ
	static void(SevenBoss::* stateTable[])();
private:
	static const int POLTER_NUM = 4;
	static const int AVATAR_NUM = 2;
private:
	vector<Poltergeist*> poltergeist;//ポルターガイスト
	vector<InterBoss*> avatarboss;//偽物のボス
	unique_ptr<BossStunEffect> bossstuneffect;
	int m_InterVal = {};

	int m_MoveTimer = {};
	int m_AvatarCount = {};

	//攻撃回数
	int m_AttackCount = {};
	
	bool m_Stun = false;
};