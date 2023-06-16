#pragma once
#include "InterBoss.h"
#include "Shake.h"
#include "BarrangeCD.h"
#include "ConfuCD.h"
#include "LineCD.h"
#include "DebuffCD.h"
#include "AttackNote.h"
#include "DamageArea.h"
class FourthBoss :
	public InterBoss {
public:
	FourthBoss();

	bool Initialize() override;//初期化

	void SkipInitialize() override;//スキップ時の初期化

	void Pause() override;//ポーズ

	void Action() override;//行動

	void AppearAction() override;//ボス登場の固有の処理

	void DeadAction() {};//ボス撃破の固有の処理

	void DeadAction_Throw() {};//ボス撃破の固有の処理 スロー

	void ImGui_Origin() override;//ボスそれぞれのImGui

	void EffecttexDraw(DirectXCommon* dxCommon) override;

	void Draw(DirectXCommon* dxCommon) override;//描画
private:
	//インターバル
	void InterValMove();
	//動きの選択
	void Choice();
	//ダメージエリアのセット
	void LineSet();
	//プレイヤーのデバフ
	void Debuff();
	//混乱
	void Confu();
	//弾幕
	void Barrage();
	//投げる
	void Throw();
	//行動終わり
	void EndMove();
	//CSV読み込み系
	void CSVLoad();
	//ノーツの生成
	void BirthNote(const std::string& BarrageName);

private:
	static const int BULLET_NUM = 4;
	static const int CD_NUM = 4;
private:
	//各クラス
	array<unique_ptr<InterCD>, CD_NUM> cd;
	vector<AttackNote*> attacknotes;//怒りのスタンプ
	unique_ptr<DamageArea> damagearea;//ダメージエリア
	//キャラの状態
	enum CharaState
	{
		STATE_INTER,
		STATE_CHOICE,
		STATE_LINE,
		STATE_DEBUFF,
		STATE_CONFU,
		STATE_BARRA,
		STATE_THROW,
		STATE_END
	};

	//停止時間
	int m_StopTimer = 0;
	//どの行動にするか
	int m_MoveState = {};

	//関数ポインタ
	static void(FourthBoss::* stateTable[])();
	
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

	enum CDType {
		CD_LINE,
		CD_DEBUFF,
		CD_CONFU,
		CD_BARRA,
	};

	int m_MoveInterVal = {};

	int m_ThrowTimer = {};

	enum ThrowState {
		THROW_SET,
		THROW_NOW,
		THROW_END
	};

	int m_ThrowState = THROW_SET;
};
