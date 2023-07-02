#pragma once
#include "InterBoss.h"
#include "Shake.h"
#include "Poltergeist.h"

class AvatarBoss :
	public InterBoss {
public:
	AvatarBoss();

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

	void BirthPolter(const std::string& PolterName);//ポルターガイストの生成
private:
	//キャラの状態
	enum CharaState
	{
		STATE_INTER,
		STATE_POLTER,
		STATE_BOUND,
	}_charaState;

	//関数ポインタ
	static void(AvatarBoss::* stateTable[])();
private:
	static const int POLTER_NUM = 2;
private:
	vector<Poltergeist*> poltergeist;//ポルターガイスト

	int m_InterVal = {};

	int m_MoveTimer = {};

	//攻撃の乱数
	int m_AttackRand = {};
};
