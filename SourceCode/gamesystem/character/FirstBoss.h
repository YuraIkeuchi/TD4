#pragma once
#include"IKESprite.h"
#include "InterBoss.h"
#include "Player.h"
#include "Shake.h"

class FirstBoss :
	public InterBoss {
public:
	FirstBoss();
	bool Initialize() override;//初期化

	void Pause() override;//ポーズ

	void Action() override;//行動

	void EffecttexDraw(DirectXCommon* dxCommon) override;

	void ImGui_Origin() override;

private:
	struct SummonEnemy
	{
	private:
		bool AttackF;

	public:
		//通常関数
		void Update(XMFLOAT3& Pos, XMFLOAT3& Rot);
		void Attack(XMFLOAT3& Pos, XMFLOAT3& Rot);
		void StayAction();
		void TexScling();
		void Draw();
	public:
		bool GetAttackF() { return AttackF; }
		void SetAttackF(bool f) { AttackF = f; }
	};
	SummonEnemy _sumattack;

private:
	//ため攻撃
	struct ChargeAttack
	{
	private:
		//ため時間
		int ChargeTime;
		int Damage;
		float RotSpeed;
		enum class Phase_Charge
		{
			NON,
			CHARGE,
			JUMP,
			ATTACK,
			END
		}_phase;

		//範囲テクスチャ
		array<unique_ptr<IKETexture>, 2>impacttex;
		array<float, 2>texAlpha;
		array<XMFLOAT2, 2>texScl;

		bool AttackF;

		float JFrame;

	public:
		void Initialize();
		void Update(XMFLOAT3& Pos, XMFLOAT3& Rot);
		void Attack(XMFLOAT3& Pos, XMFLOAT3& Rot);
		void ChargeAction();
		void JumpAction(XMFLOAT3& Pos);
		void TexScling();
		void Draw();
	public:
		void SetAttackF(bool f) { AttackF = f; }
		bool GetAttackF() { return AttackF; }

	};

	ChargeAttack _cattack;
	//使わない変数大量にあるのであとでけす

	//通常突進3回
	struct NormalAttak
	{
	private:
		float RushOldRotY;
		bool RushRotationF;

		XMVECTOR m_move = { 0.f,0.f, 0.1f, 0.0f };

		XMMATRIX m_matRot;

		XMFLOAT3 RotStartPos;
		float RushMoveEaseT;
		XMFLOAT3 RushOldPos;
		float SetCircleMoveSPos;
		int StayCount;
		bool StayF;

		XMFLOAT3 OldPos_Remove;
		float SPosMoveEaseT;
		float shakeX, shakeZ;

		XMFLOAT3 BeforeShakePos;
		bool shakeend;
		float RemovePosEaseT;
		float RotEaseTime;
		bool NormalAttackF;
		float BackEaseT;
		float BackSpeed;
		float RePosAngle;
		bool HitF;
		XMFLOAT3 ColPos;

		Shake* shake;
		//このやり方ひどくない？？
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
		}_phaseN = Phase_Normal::NON;
	private:

		void Idle(XMFLOAT3& Pos, XMFLOAT3 Rot, bool& Enf);
		void ShakeAction(XMFLOAT3& Pos, XMFLOAT3& Rot);
		void Rot(XMFLOAT3& Pos, XMFLOAT3& Rot);
		void Attack();
		void Attack(XMFLOAT3& Pos, XMFLOAT3& Rot);

	public:
		void Initialize();
		void Update(XMFLOAT3& Pos, XMFLOAT3& Rot, bool& Enf);
		void SetNormalAttackF(bool f) { NormalAttackF = f; }
		bool GetAttackF() { return NormalAttackF; }
		void SetAngle(float val) { RePosAngle = val; }void Remove(XMFLOAT3& Pos, XMFLOAT3& Scl, bool Enf);
		inline void SetreposAngle() {
			RotStartPos.x = Player::GetInstance()->GetPosition().x + sinf(RePosAngle * (3.14f / 180.0f)) * 10.0f;
			RotStartPos.z = Player::GetInstance()->GetPosition().z + cosf(RePosAngle * (3.14f / 180.0f)) * 10.0f;
		}

		void ColPlayer(XMFLOAT3& Pos);
	private:
		float EaseT;
		int RandKnock;
		float KnockVal;
	};
	ChargeAttack _charge;
	NormalAttak _normal;
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

private:

	bool GoAway;
	float AwayRotEaseT;
	float RushOldRotY;
	bool RushRotationF;


	float RushMoveEaseT;
	void Move_Away();

	XMVECTOR m_move = { 0.f,0.f, 0.1f, 0.0f };

	XMMATRIX m_matRot;

private:
	XMFLOAT3 BeforeShakePos;
	bool shakeend;
	float RemovePosEaseT;

	void DebRot();
	//このやり方ひどくない？？
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

	float ColChangeEaseT;
	bool DamColSetF;

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
	void ColPlayer_Def();
};
