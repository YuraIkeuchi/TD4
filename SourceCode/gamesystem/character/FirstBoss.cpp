#include "FirstBoss.h"
#include "ModelManager.h"
#include "Helper.h"
#include <any>

#include "Collision.h"
#include "CsvLoader.h"
#include "ImageManager.h"
#include "Input.h"
#include "Shake.h"
//生成
FirstBoss::FirstBoss() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Tyuta);

	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}

bool FirstBoss::Initialize() {
	m_Position = { 0.0f,0.0f,30.0f };
	m_Scale = { 2.5f,2.4f,2.5f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	m_Rotation.y = 90.f;
	RTime = 1;
	m_Position.x = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss.csv", "pos")));
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss.csv", "hp1")));
	MoveCount = 1;
	_phaseN = Phase_Normal::NON;
	_normal.Initialize();
	_cattack.Initialize();
	ActionTimer = 1;
	return true;
}
//行動
void FirstBoss::Action() {

	/*^^^^^^上下運動^^^^^^^*/
	float OldsMov = 0;
	if (!_cattack.GetAttackF() && !_normal.GetAttackF()) {
		PosYMovingT++;
		YmovEaseT = 0.f;
		OldsMov = PosYMovingT;
	} else
	{
		YmovEaseT += 0.03f;
		PosYMovingT = Easing::EaseOut(YmovEaseT, OldsMov, 0.f);
	}

	Helper::GetInstance()->FloatClamp(YmovEaseT, 0.f, 1.f);

	if (YmovEaseT <= 1.f)
		m_Position.y = 10.f + sinf(PI * 2.f / 120.f * PosYMovingT) * -5.f;

	/*^^^^^^^^^^^^^^^^^^*/


	/*クルッと回るやつ　タイマー*/
	if (RTime % 100 == 0) { isRot = true; }
	RTime++;

	if (!Recv)
	{
		OldRotY = m_Rotation.y;
		AwayRotEaseT = 0.f;
		GoAway = false;

	}
	/*^^^^^^^^^^^^^^^^^^^^^*/


	/*^^^^当たり判定^^^^*/
	//弾とボスの当たり判定
	vector<InterBullet*> _playerBulA = Player::GetInstance()->GetBulllet_attack();
	CollideBul(_playerBulA);

	//通常時の当たり判定
	if (!_normal.GetAttackF() && !_cattack.GetAttackF())
	{
		ColPlayer_Def();
	}
	/*^^^^^^^^^^^^^^^^^*/


	/*^^^^^攻撃判定^^^^^*/
	//非戦闘時の動き
	bool noAction = (!_normal.GetAttackF() && !_cattack.GetAttackF()&&!SummobnStop);
	if (!BattleStartF) {
		Recv = false;
		NoBattleMove();
	} else {

		RotEaseTime_noBat = 0.f;
		EaseT_BatStart = 0.f;
		//タイマーカウンタ
		if (noAction&& !Recv)ActionTimer++;

		if (!_normal.GetAttackF()&&!SummonF && ActionTimer % 60 == 0) {
			ResF = true;
			SummobnStop = true;
			SummonF = true;

		}

		//通常攻撃
		if (!SummobnStop&&!_normal.GetAttackF() && ActionTimer % 135 == 0)
			_normal.SetNormalAttackF(true);
		//ため攻撃
		if (!_cattack.GetAttackF() && ActionTimer % 340 == 0)
			_cattack.SetAttackF(true);


		//タイマーリセット(通常攻撃とため攻撃が重ならないように)
		if (ActionTimer >= 800)
		{
			ActionTimer = 0;
		}
		//通常移動（円運動）
		Move();

		//通常攻撃
		_normal.Update(m_Position, m_Rotation, EncF);
		//ため攻撃
		_cattack.Update(m_Position, m_Rotation);


		Move_Away(); _normal.Remove(m_Position, m_Scale, EncF); DamAction();
	}
	/*^^^^^^^^^^^^^^^^^^^*/


	_normal.ColPlayer(m_Position);

	_normal.SetreposAngle();

	//OBJのステータスのセット
	Obj_SetParam();


	//リミット制限
	Helper::GetInstance()->FloatClamp(m_Position.x, -55.0f, 65.0f);
	Helper::GetInstance()->FloatClamp(m_Position.z, -60.0f, 60.0f);
}
//ポーズ
void FirstBoss::Pause() {


}
void FirstBoss::NormalAttak::Update(XMFLOAT3& Pos, XMFLOAT3& Rot, bool& Enf)
{

	switch (_phaseN)
	{
	case Phase_Normal::ROTPLAYER_0:

		Rot.z -= RotSpeed;
		NormalAttak::ShakeAction(Pos, Rot);
		break;
	case Phase_Normal::PHASE_ONE:

		Rot.z -= RotSpeed;
		NormalAttak::Attack(Pos, Rot);
		break;
	case Phase_Normal::ROTPLAYER_1:

		Rot.z -= RotSpeed;
		NormalAttak::ShakeAction(Pos, Rot);
		break;
	case Phase_Normal::PHASE_TWO:

		Rot.z -= RotSpeed;
		NormalAttak::Attack(Pos, Rot);
		break;
	case Phase_Normal::ROTPLAYER_2:

		Rot.z -= RotSpeed;
		NormalAttak::ShakeAction(Pos, Rot);
		break;
	case Phase_Normal::PHASE_THREE:

		Rot.z -= RotSpeed;
		NormalAttak::Attack(Pos, Rot);
		break;
	case Phase_Normal::ROTPLAYER_3:

		Rot.z -= RotSpeed;
		NormalAttak::ShakeAction(Pos, Rot);
		break;
	case Phase_Normal::NON:
	{
		RushRotationF = false;
		BeforeShakePos = Pos;
		RemovePosEaseT = 0.f;
		BackSpeed = 4.f;
		shake->SetShakeTimer(0); shake->SetShakeStart(false);
		shakeend = false;
		RotEaseTime = 0.f;
		Enf = true;
		RushRotationF = false;
		StayF = false;
		RushMoveEaseT = 0.f;
		RemovePosEaseT = 0.f;
		StayCount = 0;
	}
	if (NormalAttackF) {
		BackSpeed = 4.f;
		RotEaseTime = 0.f;
		RushRotationF = true;
		RushOldPos = Pos;
		_phaseN = Phase_Normal::ROTPLAYER_0;
	}
	break;

	case Phase_Normal::STIFF:
		NormalAttak::Idle(Pos, Rot, Enf);
		break;
	}
}

void FirstBoss::EffecttexDraw(DirectXCommon* dxCommon)
{

	IKETexture::PreDraw2(dxCommon, ImageManager::IMPACT);
	_cattack.Draw();
	IKETexture::PostDraw();
}

void FirstBoss::Rot()
{
	if (Recv)return;
	if (_normal.GetAttackF())return;
	//if (!isRot)return;
	if (isRot) {
		RotEaseT += 0.03f;

		m_Rotation.y = Easing::EaseOut(RotEaseT, 180.f, 180.f + 360.f);

		if (RotEaseT >= 1.f)
		{
			isRot = false;
		}
	} else {
		//	m_Rotation.y = 180.f;
		RotEaseT = 0.f;
	}

}


void FirstBoss::Move()
{

	XMFLOAT3 l_player = Player::GetInstance()->GetPosition();

	if (BattleStartF && !Recv && !_normal.GetAttackF() && !_cattack.GetAttackF() && !SummobnStop) {

		//角度の取得 プレイヤーが敵の索敵位置に入ったら向きをプレイヤーの方に
		XMVECTOR PositionA = { l_player.x,l_player.y,l_player.z };
		XMVECTOR PositionB = { m_Position.x,m_Position.y,m_Position.z };
		//プレイヤーと敵のベクトルの長さ(差)を求める
		XMVECTOR SubVector = XMVectorSubtract(PositionB, PositionA); // positionA - positionB;
		//回転軸をプレイヤーの方に
			//向きかえる

		float RotY = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);

		m_Rotation.y = RotY * 60.f + 90.f;


		BossAngle++;

		m_Position.x = l_player.x + sinf(BossAngle * (PI / 180.0f)) * 20.0f;
		m_Position.z = l_player.z + cosf(BossAngle * (PI / 180.0f)) * 20.0f;

	}

}


void FirstBoss::Move_Away()
{
	if (_normal.GetAttackF() || _cattack.GetAttackF())return;
	if (!Recv)return;
	///if(NormalAttackF)
	XMFLOAT3 l_player = Player::GetInstance()->GetPosition();

	//角度の取得 プレイヤーが敵の索敵位置に入ったら向きをプレイヤーの方に
	XMVECTOR PositionA = { l_player.x,l_player.y,l_player.z };
	XMVECTOR PositionB = { m_Position.x,m_Position.y,m_Position.z };
	//プレイヤーと敵のベクトルの長さ(差)を求める
	XMVECTOR SubVector = XMVectorSubtract(PositionB, PositionA); // positionA - positionB;
	//回転軸をプレイヤーの方に
		//向きかえる

	float RotY = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);

	XMVECTOR move = { 0.f,0.f, 0.1f, 0.0f };

	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y + 90.f));

	move = XMVector3TransformNormal(move, matRot);


	if (!GoAway)
	{
		AwayRotEaseT += 0.02f;
		m_Rotation.y = Easing::EaseOut(AwayRotEaseT, OldRotY, RotY * 60.f + 270.f);

		if (AwayRotEaseT >= 1.f)
		{

			GoAway = true;
		}
	} else {
		//向いた方向に逃げる
		m_Position.x = m_Position.x + move.m128_f32[0] * 6.f;
		m_Position.z = m_Position.z + move.m128_f32[2] * 6.f;

		AwayRotEaseT = 0.f;
		if (Collision::GetLength(l_player, m_Position) > 50.f)
		{
			BattleStartF = false;
		}
	}
}

void FirstBoss::NormalAttak::Idle(XMFLOAT3& Pos, XMFLOAT3 Rot, bool& Enf)
{
	StayCount++;
	if (StayCount >= 180)
	{
		RushRotationF = true;
		NormalAttak::Rot(Pos, Rot);
		if (RotEaseTime >= 1.f)
		{
			Enf = false;
			if (Enf) {
				NormalAttackF = false;
				_phaseN = Phase_Normal::NON;
			}
		}
	} else
	{
		StayF = true;
	}
}

void FirstBoss::NormalAttak::Attack(XMFLOAT3& Pos, XMFLOAT3& Rot)
{
	XMFLOAT3 l_player = Player::GetInstance()->GetPosition();

	RushRotationF = false;
	BeforeShakePos = Pos;
	RemovePosEaseT = 0.f;
	BackSpeed = 4.f;
	shake->SetShakeTimer(0); shake->SetShakeStart(false);
	shakeend = false;
	RotEaseTime = 0.f;


	m_move = { 0.f,0.f, 0.1f, 0.0f };
	m_matRot = XMMatrixRotationY(XMConvertToRadians(Rot.y + 90.f));
	m_move = XMVector3TransformNormal(m_move, m_matRot);
	RushMoveEaseT += 0.05f;
	Pos.x = Easing::EaseOut(RushMoveEaseT, RushOldPos.x, RushOldPos.x + m_move.m128_f32[0] * 600.f);
	Pos.z = Easing::EaseOut(RushMoveEaseT, RushOldPos.z, RushOldPos.z + m_move.m128_f32[2] * 600.f);


	if (RushMoveEaseT >= 1.f)
	{
		RotSpeed = 0.f;
		Rot.z = 0;
		RushRotationF = true;

		if (_phaseN == Phase_Normal::PHASE_ONE)_phaseN = Phase_Normal::ROTPLAYER_1;
		if (_phaseN == Phase_Normal::PHASE_TWO)_phaseN = Phase_Normal::ROTPLAYER_2;
		if (_phaseN == Phase_Normal::PHASE_THREE)_phaseN = Phase_Normal::STIFF;
	}
	RushOldRotY = Rot.y;

	Helper::GetInstance()->FloatClamp(RushMoveEaseT, 0.f, 1.f);
	Helper::GetInstance()->FloatClamp(BackSpeed, 0.f, 8.f);
	Helper::GetInstance()->FloatClamp(RotEaseTime, 0.f, 1.f);
}

void FirstBoss::NormalAttak::ShakeAction(XMFLOAT3& Pos, XMFLOAT3& Rot)
{
	//初期化部
	{
		//RotSpeed = 0.f;
		RushMoveEaseT = 0.f;
	}
	XMVECTOR move = { 0.f,0.f, 0.1f, 0.0f };

	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(Rot.y + 270.f));

	move = XMVector3TransformNormal(move, matRot);

	const int MaxShakeTime =60;

	if (RotEaseTime >= 1.f) {

		RotSpeed += 0.1f;
		if (shake->GetShakeTimer() < MaxShakeTime) {

			//shake->SetShakeStart(true);
			//shake->ShakePos(shakeX, 1, -1, MaxShakeTime, 12.f);
			//shake->ShakePos(shakeZ, 1, -1, MaxShakeTime, 12.f);

			////シェイク値足す
			//Pos.x += shakeX;
			//Pos.z += shakeZ;
		}
		if (RotSpeed >= 8.f&& !shakeend) {
			RemovePosEaseT += 0.05f;
			//Pos.x = Easing::EaseOut(RemovePosEaseT, pos.x, BeforeShakePos.x);
			//Pos.z = Easing::EaseOut(RemovePosEaseT, pos.z, BeforeShakePos.z);
			if (RemovePosEaseT >= 1.f) {
				shakeend = true;
			}
		}


		if (shakeend)
		{
			//Rot.z = 0.f;
			BackSpeed -= 0.25f;
			Pos.x = Pos.x + move.m128_f32[0] * BackSpeed;
			Pos.z = Pos.z + move.m128_f32[2] * BackSpeed;
		}

		//攻撃状態に遷移
		if (BackSpeed <= 0.f)
		{
			RushOldPos = Pos;
			if (_phaseN == Phase_Normal::ROTPLAYER_0)_phaseN = Phase_Normal::PHASE_ONE;
			if (_phaseN == Phase_Normal::ROTPLAYER_1)_phaseN = Phase_Normal::PHASE_TWO;
			if (_phaseN == Phase_Normal::ROTPLAYER_2)_phaseN = Phase_Normal::PHASE_THREE;
			///if (_phaseN == Phase_Normal::ROTPLAYER_2)_phaseN = Phase_Normal::PHASE_THREE;
		}
	} else
	{
		NormalAttak::Rot(Pos, Rot);
	}

	if (Collision::CircleCollision(Pos.x, Pos.z, 1.f, Player::GetInstance()->GetPosition().x, Player::GetInstance()->GetPosition().z, 3.f))
	{
		Player::GetInstance()->isOldPos();
	}
}

void FirstBoss::NormalAttak::Rot(XMFLOAT3& Pos, XMFLOAT3& Rot)
{
	XMFLOAT3 l_player = Player::GetInstance()->GetPosition();
	//角度の取得 プレイヤーが敵の索敵位置に入ったら向きをプレイヤーの方に
	XMVECTOR PositionA = { l_player.x,l_player.y,l_player.z };
	XMVECTOR PositionB = { Pos.x,Pos.y,Pos.z };
	//プレイヤーと敵のベクトルの長さ(差)を求める
	XMVECTOR SubVector = XMVectorSubtract(PositionB, PositionA); // positionA - positionB;

	if (!RushRotationF)
	{
		RotEaseTime = 0.f;
		RushOldRotY = Rot.y;
	} else {
		//角度の取得 プレイヤーが敵の索敵位置に入ったら向きをプレイヤーの方に
		XMVECTOR PositionA = { l_player.x,l_player.y,l_player.z };
		XMVECTOR PositionB = { Pos.x,Pos.y,Pos.z };
		//プレイヤーと敵のベクトルの長さ(差)を求める
		XMVECTOR SubVector = XMVectorSubtract(PositionB, PositionA); // positionA - positionB;
		//回転軸をプレイヤーの方に
			//向きかえる

		float RotY = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);

		//イージングカウンタ＋＋
		RotEaseTime += 0.02f;

		//Rotation反映
		Rot.y = Easing::EaseOut(RotEaseTime, RushOldRotY, RotY * 60.f + 90.f);


	}
}

void FirstBoss::DebRot()
{
	XMFLOAT3 l_player = Player::GetInstance()->GetPosition();
	//角度の取得 プレイヤーが敵の索敵位置に入ったら向きをプレイヤーの方に
	XMVECTOR PositionA = { l_player.x,l_player.y,l_player.z };
	XMVECTOR PositionB = { m_Position.x,m_Position.y,m_Position.z };
	//プレイヤーと敵のベクトルの長さ(差)を求める
	XMVECTOR SubVector = XMVectorSubtract(PositionB, PositionA); // positionA - positionB;

	if (!RushRotationF)
	{
		RotEaseTime = 0.f;
		RushOldRotY = m_Rotation.y;
	} else {
		//角度の取得 プレイヤーが敵の索敵位置に入ったら向きをプレイヤーの方に
		XMVECTOR PositionA = { l_player.x,l_player.y,l_player.z };
		XMVECTOR PositionB = { m_Position.x,m_Position.y,m_Position.z };
		//プレイヤーと敵のベクトルの長さ(差)を求める
		XMVECTOR SubVector = XMVectorSubtract(PositionB, PositionA); // positionA - positionB;
		//回転軸をプレイヤーの方に
			//向きかえる

		float RotY = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);

		//イージングカウンタ＋＋
		RotEaseTime += 0.02f;

		//Rotation反映
		m_Rotation.y = Easing::EaseOut(RotEaseTime, RushOldRotY, RotY * 60.f + 90.f);


	}
}

void FirstBoss::DamAction()
{
	if (Recv && ColChangeEaseT <= 0.f)
	{
		DamColSetF = true;
	}
	if (DamColSetF)
	{
		ColChangeEaseT += 0.03f;
		if (ColChangeEaseT >= 1.f)
		{
			DamColSetF = false;
		}
	} else
	{
		ColChangeEaseT -= 0.03f;

	}

	m_Color.y = Easing::EaseOut(ColChangeEaseT, 0.9f, 0.2f);
	m_Color.z = Easing::EaseOut(ColChangeEaseT, 0.9f, 0.2f);
	m_Color.x = 1.f;
	m_Color.w = 1.f;

	Helper::GetInstance()->FloatClamp(ColChangeEaseT, 0.f, 1.f);

}

void FirstBoss::NormalAttak::Initialize()
{
	shake = new Shake();
}

void FirstBoss::NormalAttak::Remove(XMFLOAT3& Pos, XMFLOAT3& Scl, bool Enf)
{
	if (!Enf) {
		SPosMoveEaseT += 0.05f;

		if (SPosMoveEaseT >= 1.f)
		{
			Pos = RotStartPos;
			NormalAttackF = false;
			_phaseN = Phase_Normal::NON;
			//EncF = true;
		}
	}
	if (Enf)
	{
		SPosMoveEaseT -= 0.05f;
		OldPos_Remove = Pos;
	}

	Scl.x = Easing::EaseOut(SPosMoveEaseT, 2.5f, 0.f);
	Scl.y = Easing::EaseOut(SPosMoveEaseT, 2.5f, 0.f);
	Scl.z = Easing::EaseOut(SPosMoveEaseT, 2.5f, 0.f);

	Helper::GetInstance()->FloatClamp(SPosMoveEaseT, 0.f, 1.f);
	Helper::GetInstance()->FloatClamp(Scl.x, 0.f, 2.5f);
	Helper::GetInstance()->FloatClamp(Scl.y, 0.f, 2.5f);
	Helper::GetInstance()->FloatClamp(Scl.z, 0.f, 2.5f);
}

void FirstBoss::RemovePos()
{
	if (!EncF) {
		SPosMoveEaseT += 0.05f;

		if (SPosMoveEaseT >= 1.f)
		{
			m_Position = RotStartPos;
			_normal.SetNormalAttackF(false);
			_phaseN = Phase_Normal::NON;
			//EncF = true;
		}
	}
	if (EncF)
	{
		SPosMoveEaseT -= 0.05f;
		OldPos_Remove = m_Position;
	}

	m_Scale.x = Easing::EaseOut(SPosMoveEaseT, 2.5f, 0.f);
	m_Scale.y = Easing::EaseOut(SPosMoveEaseT, 2.5f, 0.f);
	m_Scale.z = Easing::EaseOut(SPosMoveEaseT, 2.5f, 0.f);

	Helper::GetInstance()->FloatClamp(SPosMoveEaseT, 0.f, 1.f);
	Helper::GetInstance()->FloatClamp(m_Scale.x, 0.f, 2.5f);
	Helper::GetInstance()->FloatClamp(m_Scale.y, 0.f, 2.5f);
	Helper::GetInstance()->FloatClamp(m_Scale.z, 0.f, 2.5f);
}

void FirstBoss::NoBattleMove()
{
	XMVECTOR move = { 0.f,0.f, 0.1f, 0.0f };

	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y + 90.f));

	move = XMVector3TransformNormal(move, matRot);

	XMFLOAT3 l_player = Player::GetInstance()->GetPosition();

	constexpr float MoveSpeed = 4.f;

	constexpr float EaseAddVal = 0.04f;

	bool SearchF = Collision::GetLength(m_Position, l_player) < 30.f;

	//角度の取得 プレイヤーが敵の索敵位置に入ったら向きをプレイヤーの方に
	XMVECTOR PositionA = { l_player.x + sinf(1 * (PI / 180.0f)) * 20.0f,l_player.y, l_player.z + cosf(1 * (PI / 180.0f)) * 20.0f };
	XMVECTOR PositionB = { m_Position.x,m_Position.y,m_Position.z };
	//プレイヤーと敵のベクトルの長さ(差)を求める
	XMVECTOR SubVector = XMVectorSubtract(PositionB, PositionA); // positionA - positionB;

	if (!SearchF) {
		if (MoveCount % 90 == 0) {
			YRotRandVal = m_Rotation.y + static_cast<float>(rand() % 90 + 45);
			RotChange = true;
		} else {
			if (!RotChange)
				YRotOld = m_Rotation.y;
		}

		if (RotChange) {
			MoveCount = 1;
			RotEaseTime_noBat += EaseAddVal;

			m_Rotation.y = Easing::EaseOut(RotEaseTime_noBat, YRotOld, YRotRandVal);
			if (RotEaseTime_noBat >= 1.f)
			{
				RotChange = false;
			}


		}

		else {
			MoveCount++;
			RotEaseTime_noBat = 0.f;
			m_Position.x = m_Position.x + move.m128_f32[0] * MoveSpeed;
			m_Position.z = m_Position.z + move.m128_f32[2] * MoveSpeed;
		}

		OldPos = m_Position;

	}
	if (SearchF) {
		EaseT_BatStart += 0.02f;
		float RotY = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);
		m_Color = { 0,1,0,1 };
		if (EaseT_BatStart <= 0.1f) {
			m_Rotation.y = RotY * 60.f + 90.f;
		}

		//円運動の初期位置に
		m_Position.x = Easing::EaseOut(EaseT_BatStart, OldPos.x, l_player.x + sinf(1 * (PI / 180.0f)) * 20.0f);
		m_Position.z = Easing::EaseOut(EaseT_BatStart, OldPos.z, l_player.z + cosf(1 * (PI / 180.0f)) * 20.0f);

		if (EaseT_BatStart >= 1.f)
		{
			RushRotationF = true;
			DebRot();
			if (RotEaseTime >= 1.f)
			{
				RushRotationF = false;
				m_Color = { 1,1,1,1 };
				BossAngle = 1;
				BattleStartF = true;
			}
		}
	}

	Helper::GetInstance()->FloatClamp(RotEaseTime_noBat, 0.f, 1.f);
	Helper::GetInstance()->FloatClamp(EaseT_BatStart, 0.f, 1.f);
}

void FirstBoss::NormalAttak::ColPlayer(XMFLOAT3& Pos)
{
	if (HitF)
	{
		EaseT += 0.09f;
		//	player->MoveStop(true);
		Player::GetInstance()->SetPosition({ Easing::EaseOut(EaseT,ColPos.x,ColPos.x + KnockVal),Player::GetInstance()->GetPosition().y, Easing::EaseOut(EaseT,ColPos.z,ColPos.z + KnockVal) });
		if (EaseT >= 1.f)
		{
			Player::GetInstance()->MoveStop(false);
			HitF = false;
		}
	} else
	{
		RandKnock = rand() % 100;
		if (RandKnock % 2 == 0)
			KnockVal = 15.f;
		else
			KnockVal = -15.f;
		EaseT = 0.f;
		//ラッシュ中判定あり
		if (RushMoveEaseT < 1.f && Collision::CircleCollision(Pos.x, Pos.z, 5.f, Player::GetInstance()->GetPosition().x, Player::GetInstance()->GetPosition().z, 1.f))
		{
			ColPos = Pos;
			HitF = true;
		}
	}


	Helper::GetInstance()->FloatClamp(EaseT, 0.f, 1.f);

}

void FirstBoss::ColPlayer_Def()
{
	if (RushMoveEaseT <= 0.f && Collision::CircleCollision(m_Position.x, m_Position.z, 5.f, Player::GetInstance()->GetPosition().x, Player::GetInstance()->GetPosition().z, 1.f))
	{
		Player::GetInstance()->isOldPos();
	}
}
//ImGui
void FirstBoss::ImGui_Origin() {
	ImGui::Begin("First");
	ImGui::Text("HP:%f", m_HP);
	ImGui::End();
}



void FirstBoss::ChargeAttack::Initialize()
{
	IKETexture::LoadTexture(8, L"Resources/2d/effect/impact.png");
	impacttex[0].reset(IKETexture::Create(ImageManager::IMPACT2, { 0,0,0 }, { 10,10,10 }, { 1,1,1,1 }));
	impacttex[1].reset(IKETexture::Create(ImageManager::IMPACT, { 0,0,0 }, { 0,0,0 }, { 1,1,1,1 }));

	//テクスチャ初期化
	for (auto i = 0; i < impacttex.size(); i++) {
		impacttex[i]->TextureCreate();
		texAlpha[i] = 1.f;
		texScl[i] = { 0.f,0.f };
	}
	shake = new Shake();
}

#define MaxShakeTime 90
void FirstBoss::ChargeAttack::Attack(XMFLOAT3& Pos, XMFLOAT3& Rot)
{
	switch (_phase)
	{
	case Phase_Charge::NON:
		for (auto i = 0; i < impacttex.size(); i++)
		{
			texAlpha[i] = 1.f;
			texScl[i] = { 0.f,0.f };
		}

		ChargeTime = 0;
		if (AttackF)_phase = Phase_Charge::CHARGE;
		break;

	case Phase_Charge::CHARGE:
		if (shake->GetShakeTimer() < MaxShakeTime) {

			shake->SetShakeStart(true);
			shake->ShakePos(shakeX, 1, -1, MaxShakeTime, 12.f);
			shake->ShakePos(shakeZ, 1, -1, MaxShakeTime, 12.f);

			////シェイク値足す
			Pos.x += shakeX;
			Pos.z += shakeZ;
		}

		JFrame = 0.f;
		if (shake->GetShakeTimer()>=MaxShakeTime) {
			shake->SetShakeStart(false);
			_phase = Phase_Charge::JUMP;
		}
		break;
	case Phase_Charge::JUMP:

		JumpAction(Pos);
		break;
	case Phase_Charge::ATTACK:
		//テクスチャ広がるやつ
		TexScling();
		break;

	case Phase_Charge::END:
		//終了時にフラグとフェーズ初期化
		AttackF = false;
		ChargeTime = 0;
		RotSpeed = 0.f;
		_phase = Phase_Charge::NON;
		break;
	}
}


void FirstBoss::ChargeAttack::JumpAction(XMFLOAT3& Pos)
{
	float SubPower = 0.001f;
	//落下の緩急
	constexpr float Distortion = 3.f;
	//地面の高さ
	constexpr float GroundY = 10.f;
	//ジャンプ高さ
	constexpr float Height = 10.f;

	//ドッスン挙動
	JFrame += 1.f / 60.f;
	Pos.y = GroundY + (1.0f - pow(1.0f - sin(PI * JFrame), Distortion)) * Height;

	if (JFrame >= 1.f)
	{
		_phase = Phase_Charge::ATTACK;
	}
	Helper::GetInstance()->FloatClamp(JFrame, 0.f, 1.f);
}
void FirstBoss::ChargeAttack::TexScling()
{
	constexpr float AddScling = 0.15f;
	bool flagOff = texAlpha[0] < 0.f && texAlpha[1] < 0.f;

	texScl[0].x += AddScling;
	texScl[0].y += AddScling;

	if (texScl[0].x > 3.f)
	{
		texScl[1].x += AddScling;
		texScl[1].y += AddScling;
	}

	for (auto i = 0; i < impacttex.size(); i++)
	{
		if (texScl[i].x > AddScling)texAlpha[i] -= 0.015f;
	}

	if (flagOff)_phase = Phase_Charge::END;
}

void FirstBoss::ChargeAttack::Update(XMFLOAT3& Pos, XMFLOAT3& Rot)
{
	constexpr float PosYVal = 8.f;

	for (auto i = 0; i < impacttex.size(); i++)
	{
		impacttex[i]->SetPosition({ Pos.x,PosYVal,Pos.z });
		impacttex[i]->SetIsBillboard(false);
	}

	if (!AttackF)
	{
		for (auto i = 0; i < 2; i++)
		{
			texScl[i] = { 0.f,0.f };
			texAlpha[i] = 1.f;
		}
	}

	Attack(Pos, Rot);

	//パラメータ更新
	for (auto i = 0; i < 2; i++)
	{
		impacttex[i]->SetScale({ texScl[i].x,texScl[i].y,5.f });
		impacttex[i]->SetRotation({ 90,0,0 });
		impacttex[i]->SetColor({ 1,1,1,texAlpha[i] });
		impacttex[i]->Update();
	}
}


void FirstBoss::ChargeAttack::Draw()
{
	for (auto i = 0; i < impacttex.size(); i++)
	{
		impacttex[i]->Draw();
	}
}



