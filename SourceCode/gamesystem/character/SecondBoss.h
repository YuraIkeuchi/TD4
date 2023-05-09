#pragma once
#include"IKESprite.h"
#include "InterBoss.h"
class SecondBoss :
	public InterBoss {
public:
	SecondBoss();
	bool Initialize() override;//初期化

	void Pause() override;//ポーズ
private:
	void Action() override;//行動

	void EffecttexDraw(DirectXCommon* dxCommon) override;

	//基本移動
	void Move();

public:
#pragma region 攻撃
	struct Attack
	{
	private:
		bool AttackFlag;
	public:
		void Action();
	};

	Attack _charge;
#pragma endregion

private:
	//プレイヤー検知
	bool SearchPlayer;
	unique_ptr<IKETexture>impact1;


	unique_ptr<IKESprite>SearchPlayerTex;
	float SearchtexAlpha;
private:
	//画像使うのであれば
	std::array<float, 2>texalpha;
	std::array<XMFLOAT3, 2>texscl;
private:
	//被ダメージ判定
	bool Recv;
	//色変えるよう
	bool DamColSetF;

	//待機時間
	int StayCount;
	//待機フラグ
	bool StayF;

	//向いた方向に移動するよう
	XMVECTOR m_move = { 0.f,0.f, 0.1f, 0.0f };

	XMMATRIX m_matRot;
private:
	//ダメージ食らった処理
	void DamAction();

	//元の位置に
	void RemovePos();

	//非戦闘時
	void NoBattleMove();

private:
	//攻撃
	bool NormalAttackF;
	//前座標
	XMFLOAT3 OldPos;

	float EaseT_BatStart;
	//戦闘開始
	bool BattleStartF;
	//エンカウント
	bool EncF;
private:
	void CollideBul(vector<InterBullet*>bullet);
};