#pragma once
#include "InterBoss.h"
#include "Shake.h"
#include "CD.h"
#include "AttackNote.h"
#include "DamageArea.h"
#include "Spline.h"
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
	//動きの選択
	void Choice();
	//ふつうの弾幕
	void NormalBarrage();
	//交互の弾
	void AlterBarrage();
	//ランダムの状態
	void RandomBarrage();
	//ダメージエリアのセット
	void DamageAeraSet();
	//CSV読み込み系
	void CSVLoad();
	//ノーツの生成
	void BirthNote(const std::string& BarrageName);

private:
	static const int BULLET_NUM = 4;
private:
	//各クラス
	unique_ptr<CD> cd;
	vector<AttackNote*> attacknotes;//怒りのスタンプ
	unique_ptr<DamageArea> damagearea;//ダメージエリア
	vector<XMFLOAT3>pointsList;
	Spline* spline;//スプライン
	//キャラの状態
	enum CharaState
	{
		STATE_CHOICE,
		STATE_NORMAL,
		STATE_ALTER,
		STATE_RANDOM,
		STATE_AREA
	}_charaState;

	//停止時間
	int m_StopTimer = 0;
	//どの行動にするか
	int m_MoveState = {};

	//関数ポインタ
	static void(FourthBoss::* stateTable[])();
	

	//CSV系
	int m_ChoiceInterval = {};

	//イージング後の位置
	XMFLOAT3 m_AfterPos = {};
	//X方向の回転
	XMFLOAT3 m_AfterRot = { 0.0f,0.0f,0.0f };
	float m_Frame = {};

	enum BarrageState {
		BARRA_SET,
		BARRA_BIRTH,
		BARRA_END,
	};

	int m_BarraState = {};

	int m_RotCount = 0;
	int m_RotTimer = 0;

	enum AreaState {
		AREA_SET,
		AREA_STOP,
		AREA_END,
	};

	int m_AreaState = AREA_SET;
	float SplineSpeed = false;
};
