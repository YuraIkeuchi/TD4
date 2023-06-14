#pragma once
#include "InterBoss.h"
#include "Shake.h"
#include "Note.h"
#include "AttackNote.h"
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
	//追従の音符
	void Follow();
	//拡散の弾
	void Diffusion();
	//混乱の状態
	void Confusion();
	//CSV読み込み系
	void CSVLoad();
	//ノーツの生成
	void BirthNote(const int NoteType);
private:
	//各クラス
	unique_ptr<Note> note;
	vector<AttackNote*> attacknotes;//怒りのスタンプ
	//キャラの状態
	enum CharaState
	{
		STATE_CHOICE,
		STATE_FOLLOW,
		STATE_DIFF,
		STATE_CONFU,
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

	enum FollowState {
		FOLLOW_SET,
		FOLLOW_BIRTH,
		FOLLOW_END,
	};
	int m_FollowState = {};

	enum DiffusionState {
		DIFFU_SET,
		DIFFU_BIRTH,
		DIFFU_END,
	};

	int m_DiffuState = {};

	int m_RotCount = 0;
	int m_RotTimer = 0;
};
