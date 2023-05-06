#include "FirstBoss.h"
#include "ModelManager.h"
#include "Helper.h"
#include <any>

#include "Collision.h"
#include "CsvLoader.h"
#include "ImageManager.h"
#include "Input.h"
//生成
FirstBoss::FirstBoss() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Tyuta);

	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}

bool FirstBoss::Initialize() {
	IKETexture::LoadTexture(8, L"Resources/2d/effect/impact.png");
	//	impact1.reset(new IKETexture();
	impact1.reset(IKETexture::Create(ImageManager::IMPACT2, { 0,0,0 }, { 10,10,10 }, { 1,1,1,1 }));
	impact2.reset(IKETexture::Create(ImageManager::IMPACT, { 0,0,0 }, { 0,0,0 }, { 1,1,1,1 }));
	DamageArea.reset(IKETexture::Create(ImageManager::IMPACT, { 0,0,0 }, { 0,0,0 }, { 1,1,1,1 }));


	for (auto i = 0; i < 3; i++)
	{
		AttackTex[i].Tex.reset(IKETexture::Create(ImageManager::IMPACT, { 0,0,0 }, { 0,0,0 }, { 1,1,1,1 }));
		AttackTex[i].Tex->TextureCreate();
	}

	impact1->TextureCreate();
	impact2->TextureCreate();
	DamageArea->TextureCreate();


	for (auto i = 0; i < 2; i++)
	{
		texalpha[i] = 1.f;
		texscl[i] = { 0.f,0.f,0.f };
	}

	m_Position = { 0.0f,0.0f,30.0f };
	m_Scale = { 1.f,1.f,1.f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	m_Rotation.y = 90.f;
	RTime = 1;
	m_Position.x = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss.csv", "pos")));
	MoveCount = 1;
	_phaseN = Phase_Normal::NON;

	SearchtexAlpha = 0.f;
	actiontimer = 1;
	return true;
}
//行動
void FirstBoss::Action() {


	float OldsMov = 0;
	if (!NormalAttackF) {
		PosYMovingT++;
		YmovEaseT = 0.f;
		OldsMov = PosYMovingT;
	} else
	{
		YmovEaseT += 0.03f;
		PosYMovingT = Easing::EaseOut(YmovEaseT, OldsMov, 0.f);
	}
	Helper::GetInstance()->FloatClamp(YmovEaseT, 0.f, 1.f);

	m_Position.y = 10.f + sinf(3.14f * 2.f / 120.f * PosYMovingT) * -5.f;

	XMFLOAT3 l_player = player->GetPosition();

	//

	impact1->SetPosition({ m_Position.x,8.f,m_Position.z });
	impact2->SetPosition({ m_Position.x, 8.f,m_Position.z });

	impact1->SetIsBillboard(false);
	impact2->SetIsBillboard(false);
	//	impact1->SetAnchorPoint({ 0.5f,0.5f });
		//impact2->SetAnchorPoint({ 0.5f,0.5f });

	if (!ImpactF)
	{
		for (auto i = 0; i < 2; i++)
		{
			texscl[i] = { 0.f,0.f,0.f };
			texalpha[i] = 1.f;
		}
	}
	impact1->SetScale({ texscl[0].x,texscl[0].y,5.f });
	impact2->SetScale({ texscl[1].x,texscl[1].y ,5.f });

	impact1->SetRotation({ 90,0,0 });
	impact2->SetRotation({ 90,0,0 });
	impact1->SetColor({ 1,1,1,texalpha[0] });
	impact2->SetColor({ 1,1,1,texalpha[1] });

	impact1->Update();
	impact2->Update();


	DamageArea->SetColor({ 1,1,1,DamageAreaAlpha });
	DamageArea->SetRotation({ 90,0,0 });
	DamageArea->SetIsBillboard(false);
	DamageArea->Update();

	for (auto i = 0; i < 3; i++)
	{
		AttackTex[i].Tex->SetColor({ 1,1,1,AttackTex[i].alpha });
		AttackTex[i].Tex->SetPosition(AttackTex[i].Pos);
		AttackTex[i].Tex->SetIsBillboard(false);
		AttackTex[i].Tex->Update();
	}
	//向きかえる
	ImpactAttack();
	if (RTime % 100 == 0) { isRot = true; }
	RTime++;

	if (!Recv)
	{
		OldRotY = m_Rotation.y;
		AwayRotEaseT = 0.f;
		GoAway = false;

	}
	if (Input::GetInstance()->TriggerButton(Input::X))
	{
		Recv = true;
	}


	if (Input::GetInstance()->TriggerButton(Input::B))
	{
		NormalAttackF = true;
	}
	//Rot();
	if (!BattleStartF) {
		NoBattleMove();
	} else {
		if (!NormalAttackF && !ImpactF && !Recv) {
			actiontimer++;
		}
		if (!NormalAttackF && actiontimer % 180 == 0)
		{
			NormalAttackF = true;
		}
		if (!ImpactF && actiontimer % 300 == 0)
		{
			ImpactF = true;
		}
		Move();
		switch (_phaseN)
		{
		case Phase_Normal::ROTPLAYER_0:
			DebShake();
			break;
		case Phase_Normal::PHASE_ONE:
			DebAttack();
			break;
		case Phase_Normal::ROTPLAYER_1:
			DebShake();
			break;
		case Phase_Normal::PHASE_TWO:
			DebAttack();
			break;
		case Phase_Normal::ROTPLAYER_2:
			DebShake();
			break;
		case Phase_Normal::PHASE_THREE:
			DebAttack();
			break;
		case Phase_Normal::ROTPLAYER_3:
			DebShake();
			break;
		case Phase_Normal::NON:
		{
			EncF = true;
			RotEaseTime = 0.f;
			RushRotationF = false;
			StayF = false;
			shaketime = 80;
			RushMoveEaseT = 0.f;
			RemovePosEaseT = 0.f;

			RushRotationF = false;
			BeforeShakePos = m_Position;
			//shaketime = 60.0f;
			RemovePosEaseT = 0.f;
			BackSpeed = 4.f;
			shaketime = 60.0f;
			//shakeend = false;
			StayCount = 0;
			RotEaseTime = 0.f;
			//shakeend = true;
		}
		if (NormalAttackF) {
			actiontimer++;
			BackSpeed = 4.f;
			RotEaseTime = 0.f;
			RushRotationF = true;
			RushOldPos = m_Position;
			_phaseN = Phase_Normal::ROTPLAYER_0;
		}
		break;

		case Phase_Normal::STIFF:
			StayMove();
			break;
		}
		Move_Away(); RemovePos(); DamAction();
		m_Color = Col;
	}


	//NormalAttack();

	//OBJのステータスのセット
	Obj_SetParam();
}
//ポーズ
void FirstBoss::Pause() {


}

void FirstBoss::EffecttexDraw(DirectXCommon* dxCommon)
{
	IKETexture::PreDraw2(dxCommon, ImageManager::IMPACT);
	impact1->Draw2(dxCommon);
	impact2->Draw2(dxCommon);

	for (auto i = 0; i < 3; i++)
	{
		AttackTex[i].Tex->Draw();
	}
	DamageArea->Draw2(dxCommon);

	IKETexture::PostDraw();
}

void FirstBoss::Rot()
{
	if (Recv)return;
	if (NormalAttackF)return;
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


void FirstBoss::ImpactAttack()
{
	if (!ImpactF)return;


	texscl[0].x += 0.1f;
	texscl[0].y += 0.1f;

	if (texscl[0].x > 2.f)
	{
		texscl[1].x += 0.1f;
		texscl[1].y += 0.1f;
	}

	for (auto i = 0; i < 2; i++)
	{
		if (texscl[i].x > 0.1f)texalpha[i] -= 0.035f;
	}

	if (texalpha[0] < 0.f && texalpha[1] < 0.f)
	{
		ImpactF = false;
	}
}

void FirstBoss::Move()
{

	XMFLOAT3 l_player = player->GetPosition();

	if (!Recv && !NormalAttackF) {

		//角度の取得 プレイヤーが敵の索敵位置に入ったら向きをプレイヤーの方に
		XMVECTOR PositionA = { l_player.x,l_player.y,l_player.z };
		XMVECTOR PositionB = { m_Position.x,m_Position.y,m_Position.z };
		//プレイヤーと敵のベクトルの長さ(差)を求める
		XMVECTOR SubVector = XMVectorSubtract(PositionB, PositionA); // positionA - positionB;
		//回転軸をプレイヤーの方に
			//向きかえる

		float RotY = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);

		m_Rotation.y = RotY * 60.f + 90.f;


		if (SearchPlayer) {
			XMVECTOR move = { 0.f,0.f, 0.1f, 0.0f };

			XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y + 60.f));

			move = XMVector3TransformNormal(move, matRot);

			//m_Position = {
			//		m_Position.x + move.m128_f32[0] * 2.f,
			//	m_Position.y,
			//	m_Position.z + move.m128_f32[2] * 2.f
			//};
		}

		BossAngle++;
		m_Position.x = l_player.x + sinf(BossAngle * (PI / 180.0f)) * 10.0f;
		m_Position.z = l_player.z + cosf(BossAngle * (PI / 180.0f)) * 10.0f;
	}




	RotStartPos.x = l_player.x + sinf(BossAngle * (PI / 180.0f)) * 10.0f;
	RotStartPos.z = l_player.z + cosf(BossAngle * (PI / 180.0f)) * 10.0f;
}


void FirstBoss::Move_Away()
{
	if (!Recv)return;
	///if(NormalAttackF)
	XMFLOAT3 l_player = player->GetPosition();

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
		if (Collision::GetLength(l_player, m_Position) > 100.f)
		{
			Recv = false;
		}
	}

	//

}

void FirstBoss::FrontAttack()
{

}


void FirstBoss::RushAttack()
{
}

void FirstBoss::StayMove()
{
	StayCount++;
	if (StayCount >= 180)
	{
		RushRotationF = true;
		DebRot();
		if (RotEaseTime >= 1.f)
		{
			EncF = false;
			if (EncF) {
				actiontimer += 2;
				NormalAttackF = false;
				_phaseN = Phase_Normal::NON;
			}
		}
	} else
	{
		StayF = true;
	}
}



void FirstBoss::DebAttack()
{
	XMFLOAT3 l_player = player->GetPosition();

	RushRotationF = false;
	BeforeShakePos = m_Position;
	//shaketime = 60.0f;
	RemovePosEaseT = 0.f;
	BackSpeed = 4.f;
	shaketime = 60.0f;
	shakeend = false;

	RotEaseTime = 0.f;


	m_move = { 0.f,0.f, 0.1f, 0.0f };
	m_matRot = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y + 90.f));
	m_move = XMVector3TransformNormal(m_move, m_matRot);
	RushMoveEaseT += 0.05f;
	m_Position.x = Easing::EaseOut(RushMoveEaseT, RushOldPos.x, RushOldPos.x + m_move.m128_f32[0] * 300.f);
	m_Position.z = Easing::EaseOut(RushMoveEaseT, RushOldPos.z, RushOldPos.z + m_move.m128_f32[2] * 300.f);


	if (RushMoveEaseT >= 1.f)
	{
		RushRotationF = true;

		if (_phaseN == Phase_Normal::PHASE_ONE)_phaseN = Phase_Normal::ROTPLAYER_1;
		if (_phaseN == Phase_Normal::PHASE_TWO)_phaseN = Phase_Normal::ROTPLAYER_2;
		if (_phaseN == Phase_Normal::PHASE_THREE)_phaseN = Phase_Normal::STIFF;
	}RushOldRotY = m_Rotation.y;

	Helper::GetInstance()->FloatClamp(RushMoveEaseT, 0.f, 1.f);
	Helper::GetInstance()->FloatClamp(BackSpeed, 0.f, 8.f);
	Helper::GetInstance()->FloatClamp(DamageAreaAlpha, 0.f, 0.5f);
	Helper::GetInstance()->FloatClamp(RotEaseTime, 0.f, 1.f);
}

void FirstBoss::DebShake()
{
	//初期化部
	{
		RushMoveEaseT = 0.f;
	}
	XMVECTOR move = { 0.f,0.f, 0.1f, 0.0f };

	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y + 270.f));

	move = XMVector3TransformNormal(move, matRot);

	if (RotEaseTime >= 1.f) {
		if (shaketime != 0)
		{
			shake = static_cast<float>(rand() % 2 - 1);
			shakex = static_cast<float>(rand() % 2 - 1);
			shakey = static_cast<float>(rand() % 2 - 1);
			shakex -= shake / 12.00f;
			shakey -= shake / 12.00f;
			shaketime--;
			//}
			m_Position.x += shakex / 12.00f;
			m_Position.z += shakey / 12.00f;
			//シェイク値を０に
		}

		if (shaketime <= 0.0f && !shakeend) {
			RemovePosEaseT += 0.05f;
			m_Position.x = Easing::EaseOut(RemovePosEaseT, m_Position.x, BeforeShakePos.x);
			m_Position.z = Easing::EaseOut(RemovePosEaseT, m_Position.z, BeforeShakePos.z);
			if (RemovePosEaseT >= 1.f) {
				shakeend = true;
			}
		}


		if (shakeend)
		{
			shakex = 0.0f;
			shakey = 0.0f;
			BackSpeed -= 0.25f;
			m_Position.x = m_Position.x + move.m128_f32[0] * BackSpeed;
			m_Position.z = m_Position.z + move.m128_f32[2] * BackSpeed;
		}

		if (BackSpeed <= 0.f)
		{

			RushOldPos = m_Position;
			if (_phaseN == Phase_Normal::ROTPLAYER_0)_phaseN = Phase_Normal::PHASE_ONE;
			if (_phaseN == Phase_Normal::ROTPLAYER_1)_phaseN = Phase_Normal::PHASE_TWO;
			if (_phaseN == Phase_Normal::ROTPLAYER_2)_phaseN = Phase_Normal::PHASE_THREE;
			///if (_phaseN == Phase_Normal::ROTPLAYER_2)_phaseN = Phase_Normal::PHASE_THREE;
		}
	} else
	{
		DebRot();
	}
}

void FirstBoss::DebRot()
{
	XMFLOAT3 l_player = player->GetPosition();
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

	Col.y = Easing::EaseOut(ColChangeEaseT, 0.9f, 0.2f);
	Col.z = Easing::EaseOut(ColChangeEaseT, 0.9f, 0.2f);
	Col.x = 1.f;
	Col.w = 1.f;

	Helper::GetInstance()->FloatClamp(ColChangeEaseT, 0.f, 1.f);

}



void FirstBoss::RemovePos()
{
	if (!EncF) {
		SPosMoveEaseT += 0.05f;

		if (SPosMoveEaseT >= 1.f)
		{
			m_Position = RotStartPos;
			NormalAttackF = false;
			_phaseN = Phase_Normal::NON;
			//EncF = true;
		}
	}
	if (EncF)
	{
		SPosMoveEaseT -= 0.05f;
		OldPos_Remove = m_Position;
	}

	m_Scale.x = Easing::EaseOut(SPosMoveEaseT, 1.f, 0.f);
	m_Scale.y = Easing::EaseOut(SPosMoveEaseT, 1.f, 0.f);
	m_Scale.z = Easing::EaseOut(SPosMoveEaseT, 1.f, 0.f);

	Helper::GetInstance()->FloatClamp(SPosMoveEaseT, 0.f, 1.f);
	Helper::GetInstance()->FloatClamp(m_Scale.x, 0.f, 1.f);
	Helper::GetInstance()->FloatClamp(m_Scale.y, 0.f, 1.f);
	Helper::GetInstance()->FloatClamp(m_Scale.z, 0.f, 1.f);
}

void FirstBoss::NoBattleMove()
{
	XMVECTOR move = { 0.f,0.f, 0.1f, 0.0f };

	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y + 90.f));

	move = XMVector3TransformNormal(move, matRot);

	XMFLOAT3 l_player = player->GetPosition();

	constexpr float MoveSpeed = 4.f;

	constexpr float EaseAddVal = 0.04f;

	bool SearchF = Collision::GetLength(m_Position, l_player) < 20.f;

	//角度の取得 プレイヤーが敵の索敵位置に入ったら向きをプレイヤーの方に
	XMVECTOR PositionA = { l_player.x + sinf(0 * (PI / 180.0f)) * 10.0f,l_player.y, l_player.z + cosf(0 * (PI / 180.0f)) * 10.0f };
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
		Helper::GetInstance()->FloatClamp(m_Position.z, 20.f, 45.f);
		Helper::GetInstance()->FloatClamp(m_Position.x, -10.f, 10.f);

	}
	if (SearchF) {
		EaseT_BatStart += 0.02f;
		float RotY = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);
		m_Color = { 0,1,0,1 };
		if (EaseT_BatStart <= 0.1f) {
			m_Rotation.y = RotY * 60.f + 90.f;
		}

		m_Position.x = Easing::EaseOut(EaseT_BatStart, OldPos.x, l_player.x + sinf(1 * (PI / 180.0f)) * 10.0f);
		m_Position.z = Easing::EaseOut(EaseT_BatStart, OldPos.z, l_player.z + cosf(1 * (PI / 180.0f)) * 10.0f);

		if (EaseT_BatStart >= 1.f)
		{
			RushRotationF = true;
			DebRot();
			if (RotEaseTime >= 1.f)
			{
				RushRotationF = false;
				m_Color = { 1,1,1,1 };
				BattleStartF = true;
			}
		}
	}
	Helper::GetInstance()->FloatClamp(RotEaseTime_noBat, 0.f, 1.f);
	Helper::GetInstance()->FloatClamp(EaseT_BatStart, 0.f, 1.f);
}
