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
	//インターバル
	void InterVal();
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

	//棘の的に使う
	float m_Angle = 0.0f;
	float m_Angle2 = 0.0f;
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
};