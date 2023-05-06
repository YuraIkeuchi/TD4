#pragma once
#include"IKESprite.h"
#include "InterBoss.h"
class FirstBoss :
	public InterBoss {
public:
	FirstBoss();
	bool Initialize() override;//������

	void Pause() override;//�|�[�Y
private:
	void Action() override;//�s��
	void EffecttexDraw(DirectXCommon* dxCommon) override;
private:
	float PosYMovingT;
	float RotEaseT;

	bool isRot;
	int RTime;
	void Rot();

	float BossAngle = 1.f;
	void FrontAttack();
	void ImpactAttack();
	void NormalAttack();
	void RushAttack();

	void RottoPlayer();
	float RotEaseTime;
	float OldRotY;
	void Move();

	bool SearchPlayer;
	bool ImpactF;
private:
	unique_ptr<IKETexture>impact1;
	unique_ptr<IKETexture>impact2;

	unique_ptr<IKESprite>SearchPlayerTex;
	float SearchtexAlpha;

	struct FrontAttackTex
	{
		unique_ptr<IKETexture>Tex;
		XMFLOAT3 Pos;
		float alpha;
	};
	std::array<FrontAttackTex, 3>AttackTex;

	bool FrontAttackF;
private:

	std::array<float, 2>texalpha;
	std::array<XMFLOAT3, 2>texscl;
private:
	bool Recv;
	bool GoAway;
	float AwayRotEaseT;
	float RushOldRotY;
	bool RushRotationF;


	float RushMoveEaseT;
	XMFLOAT3 RushOldPos;
	void Move_Away();
	void ShakeAction();
	void StayMove();
	int StayCount;
	bool StayF;


	XMVECTOR m_move = { 0.f,0.f, 0.1f, 0.0f };

	XMMATRIX m_matRot;

private:
	bool NormalAttackF;
	float BackEaseT;
	bool RushF;
	float BackSpeed;
	int RushCount;
	int AfterTime;
	float DamageAreaAlpha;
	unique_ptr<IKETexture>DamageArea;

private:
	float shaketime;
	float shake, shakex, shakey;
	XMFLOAT3 BeforeShakePos;
	bool shakeend;
	float RemovePosEaseT;


	void DebShake();
	void DebRot();
	void DebAttack();

	//���̂����Ђǂ��Ȃ��H�H
	enum class Phase_Normal
	{
		ROTPLAYER_0,
		PHASE_ONE,
		ROTPLAYER_1,
		PHASE_TWO,
		ROTPLAYER_2,
		PHASE_THREE,
		ROTPLAYER_3,
		NON,
		STIFF
	}_phaseN;

	void DamAction();

	XMFLOAT4 Col;
	float ColChangeEaseT;
	bool DamColSetF;


	int actiontimer;

	XMFLOAT3 RotStartPos;
	XMFLOAT3 OldPos_Remove;
	float SPosMoveEaseT;
	float YmovEaseT;
	bool EncF;

	int MoveCount;
	bool RotChange;
	float YRotRandVal;
	float YRotOld;
	float RotEaseTime_noBat;
	void NoBattleMove();
	void RemovePos();
	XMFLOAT3 OldPos;
	float EaseT_BatStart;
	bool BattleStartF;

private:
	void CollideBul(vector<InterBullet*>bullet);
};