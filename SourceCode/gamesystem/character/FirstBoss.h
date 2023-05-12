#pragma once
#include"IKESprite.h"
#include "InterBoss.h"
#include "Player.h"
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
		//ため攻撃
		struct ChargeAttack
		{
		private:
			//ため時間
			int ChargeTime;
			int Damage;

			//範囲テクスチャ
			array<unique_ptr<IKETexture>,2>impacttex;
			array<float, 2>texAlpha;
			array<XMFLOAT2, 2>texScl;

			bool AttackF;
		public:
			void Initialize();
			void Update(XMFLOAT3 Pos);
			void Attack();
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
			
			int StayCount;
			bool StayF;

			XMFLOAT3 OldPos_Remove;
			float SPosMoveEaseT;
			float shaketime;
			float shake, shakex, shakey;
			XMFLOAT3 BeforeShakePos;
			bool shakeend;
			float RemovePosEaseT;
			float RotEaseTime;
			bool NormalAttackF;
			float BackEaseT;
			bool RushF;
			float BackSpeed;
			int RushCount;
			int AfterTime;
			float DamageAreaAlpha;
			float RePosAngle;

			bool HitF;
			XMFLOAT3 ColPos;
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
			}_phaseN=Phase_Normal::NON;
		private:
			
			void Idle(XMFLOAT3& Pos, XMFLOAT3 Rot,bool &Enf);
			void Shake(XMFLOAT3& Pos, XMFLOAT3& Rot);
			void Rot(XMFLOAT3& Pos, XMFLOAT3& Rot);
			void Attack();
			void Attack(XMFLOAT3 &Pos,XMFLOAT3& Rot);
			
			public:
			void Update(XMFLOAT3& Pos, XMFLOAT3& Rot,bool &Enf);
			void SetNormalAttackF(bool f) { NormalAttackF = f; }
				bool GetAttackF() { return NormalAttackF; }
				void SetAngle(float val) { RePosAngle = val; }void Remove(XMFLOAT3& Pos, XMFLOAT3& Scl,bool Enf);
			inline void SetreposAngle(){
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
	void ColPlayer_Def();
};