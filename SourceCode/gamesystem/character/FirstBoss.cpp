#include "FirstBoss.h"
#include "ModelManager.h"
#include "Helper.h"
#include <any>

#include "Collision.h"
#include "CsvLoader.h"
#include "ImageManager.h"
#include "Input.h"
//����
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
//�s��
void FirstBoss::Action() {


	float OldsMov = 0;
	if (!_normal.GetAttackF()) {
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

	XMFLOAT3 l_player = _player->GetPosition();

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
	//����������
	ImpactAttack();
	if (RTime % 100 == 0) { isRot = true; }
	RTime++;

	if (!Recv)
	{
		OldRotY = m_Rotation.y;
		AwayRotEaseT = 0.f;
		GoAway = false;

	}

	vector<InterBullet*> _playerBulA = _player->GetBulllet_attack();
	vector<InterBullet*> _playerBulG = _player->GetBulllet_ghost();
		CollideBul(_playerBulA);
		CollideBul(_playerBulG);
	

	//Rot();
	if (!BattleStartF) {
		NoBattleMove();
	} else {
		if (!_normal.GetAttackF() && !ImpactF && !Recv) {
			actiontimer++;
		}
		if (!_normal.GetAttackF() && actiontimer % 180 == 0)
		{
			_normal.SetNormalAttackF(true);
		}
		if (!ImpactF && actiontimer % 300 == 0)
		{
			ImpactF = true;
		}
		Move();
		if(_normal.GetAttackF())
		{
			actiontimer++;
		}
		_normal.Update(_player, m_Position, m_Rotation, EncF);
		Move_Away(); _normal.Remove(m_Position,m_Scale,EncF); DamAction();
		m_Color = Col;
	}
	_normal.ColPlayer(_player, m_Position);
	_normal.SetreposAngle(_player);
	//NormalAttack();

	//OBJ�̃X�e�[�^�X�̃Z�b�g
	Obj_SetParam();
}
//�|�[�Y
void FirstBoss::Pause() {


}
void FirstBoss::NormalAttak::Update(Player* player, XMFLOAT3& Pos, XMFLOAT3& Rot,bool &Enf)
{
	switch (_phaseN)
	{
	case Phase_Normal::ROTPLAYER_0:
		NormalAttak::Shake(player, Pos, Rot);
		break;
	case Phase_Normal::PHASE_ONE:
		NormalAttak::Attack(player, Pos, Rot);
		break;
	case Phase_Normal::ROTPLAYER_1:
		NormalAttak::Shake(player, Pos, Rot);
		break;
	case Phase_Normal::PHASE_TWO:
		NormalAttak::Attack(player, Pos, Rot);
		break;
	case Phase_Normal::ROTPLAYER_2:
		NormalAttak::Shake(player, Pos, Rot);
		break;
	case Phase_Normal::PHASE_THREE:
		NormalAttak::Attack(player, Pos, Rot);
		break;
	case Phase_Normal::ROTPLAYER_3:
		NormalAttak::Shake(player,Pos,Rot);
		break;
	case Phase_Normal::NON:
	{
		Enf = true;
		RotEaseTime = 0.f;
		RushRotationF = false;
		StayF = false;
		shaketime = 80;
		RushMoveEaseT = 0.f;
		RemovePosEaseT = 0.f;

		RushRotationF = false;
		BeforeShakePos = Pos;
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
		BackSpeed = 4.f;
		RotEaseTime = 0.f;
		RushRotationF = true;
		RushOldPos = Pos;
		_phaseN = Phase_Normal::ROTPLAYER_0;
	}
	break;

	case Phase_Normal::STIFF:
		NormalAttak::Idle(player, Pos, Rot, Enf);
		break;
	}
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

	XMFLOAT3 l_player = _player->GetPosition();

	if (!Recv && !_normal.GetAttackF()) {

		//�p�x�̎擾 �v���C���[���G�̍��G�ʒu�ɓ�������������v���C���[�̕���
		XMVECTOR PositionA = { l_player.x,l_player.y,l_player.z };
		XMVECTOR PositionB = { m_Position.x,m_Position.y,m_Position.z };
		//�v���C���[�ƓG�̃x�N�g���̒���(��)�����߂�
		XMVECTOR SubVector = XMVectorSubtract(PositionB, PositionA); // positionA - positionB;
		//��]�����v���C���[�̕���
			//����������

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

}


void FirstBoss::Move_Away()
{
	if (_normal.GetAttackF())return;
	if (!Recv)return;
	///if(NormalAttackF)
	XMFLOAT3 l_player = _player->GetPosition();

	//�p�x�̎擾 �v���C���[���G�̍��G�ʒu�ɓ�������������v���C���[�̕���
	XMVECTOR PositionA = { l_player.x,l_player.y,l_player.z };
	XMVECTOR PositionB = { m_Position.x,m_Position.y,m_Position.z };
	//�v���C���[�ƓG�̃x�N�g���̒���(��)�����߂�
	XMVECTOR SubVector = XMVectorSubtract(PositionB, PositionA); // positionA - positionB;
	//��]�����v���C���[�̕���
		//����������

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
		//�����������ɓ�����
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

void FirstBoss::NormalAttak::Idle(Player* player, XMFLOAT3& Pos, XMFLOAT3 Rot,bool& Enf)
{
	StayCount++;
	if (StayCount >= 180)
	{
		RushRotationF = true;
		NormalAttak::Rot(player, Pos, Rot);
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

void FirstBoss::NormalAttak::Attack(Player* player, XMFLOAT3& Pos, XMFLOAT3& Rot)
{
	XMFLOAT3 l_player = player->GetPosition();

	//ColPlayer(player, Pos);

	RushRotationF = false;
	BeforeShakePos = Pos;
	//shaketime = 60.0f;
	RemovePosEaseT = 0.f;
	BackSpeed = 4.f;
	shaketime = 60.0f;
	shakeend = false;

	RotEaseTime = 0.f;


	m_move = { 0.f,0.f, 0.1f, 0.0f };
	m_matRot = XMMatrixRotationY(XMConvertToRadians(Rot.y + 90.f));
	m_move = XMVector3TransformNormal(m_move, m_matRot);
	RushMoveEaseT += 0.05f;
	Pos.x = Easing::EaseOut(RushMoveEaseT, RushOldPos.x, RushOldPos.x + m_move.m128_f32[0] * 300.f);
	Pos.z = Easing::EaseOut(RushMoveEaseT, RushOldPos.z, RushOldPos.z + m_move.m128_f32[2] * 300.f);


	if (RushMoveEaseT >= 1.f)
	{
		RushRotationF = true;

		if (_phaseN == Phase_Normal::PHASE_ONE)_phaseN = Phase_Normal::ROTPLAYER_1;
		if (_phaseN == Phase_Normal::PHASE_TWO)_phaseN = Phase_Normal::ROTPLAYER_2;
		if (_phaseN == Phase_Normal::PHASE_THREE)_phaseN = Phase_Normal::STIFF;
	}
	RushOldRotY = Rot.y;

	Helper::GetInstance()->FloatClamp(RushMoveEaseT, 0.f, 1.f);
	Helper::GetInstance()->FloatClamp(BackSpeed, 0.f, 8.f);
	Helper::GetInstance()->FloatClamp(DamageAreaAlpha, 0.f, 0.5f);
	Helper::GetInstance()->FloatClamp(RotEaseTime, 0.f, 1.f);
}

void FirstBoss::NormalAttak::Shake(Player* player, XMFLOAT3& Pos, XMFLOAT3& Rot)
{
	//��������
	{
		RushMoveEaseT = 0.f;
	}
	XMVECTOR move = { 0.f,0.f, 0.1f, 0.0f };

	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(Rot.y + 270.f));

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
			Pos.x += shakex / 12.00f;
			Pos.z += shakey / 12.00f;
			//�V�F�C�N�l���O��
		}

		if (shaketime <= 0.0f && !shakeend) {
			RemovePosEaseT += 0.05f;
			Pos.x = Easing::EaseOut(RemovePosEaseT, Pos.x, BeforeShakePos.x);
			Pos.z = Easing::EaseOut(RemovePosEaseT, Pos.z, BeforeShakePos.z);
			if (RemovePosEaseT >= 1.f) {
				shakeend = true;
			}
		}


		if (shakeend)
		{
			shakex = 0.0f;
			shakey = 0.0f;
			BackSpeed -= 0.25f;
			Pos.x = Pos.x + move.m128_f32[0] * BackSpeed;
			Pos.z = Pos.z + move.m128_f32[2] * BackSpeed;
		}

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
		NormalAttak::Rot(player,Pos,Rot);
	}

	if (Collision::CircleCollision(Pos.x, Pos.z, 1.f, player->GetPosition().x, player->GetPosition().z, 3.f))
	{
		player->isOldPos();
	}
}

void FirstBoss::NormalAttak::Rot(Player* player, XMFLOAT3& Pos, XMFLOAT3 &Rot)
{
	XMFLOAT3 l_player = player->GetPosition();
	//�p�x�̎擾 �v���C���[���G�̍��G�ʒu�ɓ�������������v���C���[�̕���
	XMVECTOR PositionA = { l_player.x,l_player.y,l_player.z };
	XMVECTOR PositionB = { Pos.x,Pos.y,Pos.z };
	//�v���C���[�ƓG�̃x�N�g���̒���(��)�����߂�
	XMVECTOR SubVector = XMVectorSubtract(PositionB, PositionA); // positionA - positionB;

	if (!RushRotationF)
	{
		RotEaseTime = 0.f;
		RushOldRotY = Rot.y;
	} else {
		//�p�x�̎擾 �v���C���[���G�̍��G�ʒu�ɓ�������������v���C���[�̕���
		XMVECTOR PositionA = { l_player.x,l_player.y,l_player.z };
		XMVECTOR PositionB = { Pos.x,Pos.y,Pos.z };
		//�v���C���[�ƓG�̃x�N�g���̒���(��)�����߂�
		XMVECTOR SubVector = XMVectorSubtract(PositionB, PositionA); // positionA - positionB;
		//��]�����v���C���[�̕���
			//����������

		float RotY = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);

		//�C�[�W���O�J�E���^�{�{
		RotEaseTime += 0.02f;

		//Rotation���f
		Rot.y = Easing::EaseOut(RotEaseTime, RushOldRotY, RotY * 60.f + 90.f);


	}
}

void FirstBoss::DebRot()
{
	XMFLOAT3 l_player = _player->GetPosition();
	//�p�x�̎擾 �v���C���[���G�̍��G�ʒu�ɓ�������������v���C���[�̕���
	XMVECTOR PositionA = { l_player.x,l_player.y,l_player.z };
	XMVECTOR PositionB = { m_Position.x,m_Position.y,m_Position.z };
	//�v���C���[�ƓG�̃x�N�g���̒���(��)�����߂�
	XMVECTOR SubVector = XMVectorSubtract(PositionB, PositionA); // positionA - positionB;

	if (!RushRotationF)
	{
		RotEaseTime = 0.f;
		RushOldRotY = m_Rotation.y;
	} else {
		//�p�x�̎擾 �v���C���[���G�̍��G�ʒu�ɓ�������������v���C���[�̕���
		XMVECTOR PositionA = { l_player.x,l_player.y,l_player.z };
		XMVECTOR PositionB = { m_Position.x,m_Position.y,m_Position.z };
		//�v���C���[�ƓG�̃x�N�g���̒���(��)�����߂�
		XMVECTOR SubVector = XMVectorSubtract(PositionB, PositionA); // positionA - positionB;
		//��]�����v���C���[�̕���
			//����������

		float RotY = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);

		//�C�[�W���O�J�E���^�{�{
		RotEaseTime += 0.02f;

		//Rotation���f
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


void FirstBoss::NormalAttak::Remove(XMFLOAT3& Pos, XMFLOAT3& Scl,bool Enf)
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

	Scl.x = Easing::EaseOut(SPosMoveEaseT, 1.f, 0.f);
	Scl.y = Easing::EaseOut(SPosMoveEaseT, 1.f, 0.f);
	Scl.z = Easing::EaseOut(SPosMoveEaseT, 1.f, 0.f);

	Helper::GetInstance()->FloatClamp(SPosMoveEaseT, 0.f, 1.f);
	Helper::GetInstance()->FloatClamp(Scl.x, 0.f, 1.f);
	Helper::GetInstance()->FloatClamp(Scl.y, 0.f, 1.f);
	Helper::GetInstance()->FloatClamp(Scl.z, 0.f, 1.f);
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

	XMFLOAT3 l_player = _player->GetPosition();

	constexpr float MoveSpeed = 4.f;

	constexpr float EaseAddVal = 0.04f;

	bool SearchF = Collision::GetLength(m_Position, l_player) < 20.f;

	//�p�x�̎擾 �v���C���[���G�̍��G�ʒu�ɓ�������������v���C���[�̕���
	XMVECTOR PositionA = { l_player.x + sinf(0 * (PI / 180.0f)) * 10.0f,l_player.y, l_player.z + cosf(0 * (PI / 180.0f)) * 10.0f };
	XMVECTOR PositionB = { m_Position.x,m_Position.y,m_Position.z };
	//�v���C���[�ƓG�̃x�N�g���̒���(��)�����߂�
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

void FirstBoss::CollideBul(vector<InterBullet*> bullet)
{
	constexpr float BulRad = 1.f;

	constexpr float BossRad = 5.f;

	for (InterBullet* _bullet : bullet) {
		if (_bullet != nullptr) {
			if(Collision::CircleCollision(_bullet->GetPosition().x, _bullet->GetPosition().z,BulRad,m_Position.x, m_Position.z,BossRad))
			{
				Audio::GetInstance()->PlayWave("Resources/Sound/SE/Attack_Normal.wav", VolumManager::GetInstance()->GetSEVolum());
				Recv = true;
				_bullet->SetAlive(false);
			}
		}
	}
}

void FirstBoss::NormalAttak::ColPlayer(Player* player, XMFLOAT3& Pos)
{
	
	if (HitF)
	{
		EaseT += 0.05f;

		player->SetPosition({Easing::EaseOut(EaseT,ColPos.x,ColPos.x+15.f),Pos.y, Easing::EaseOut(EaseT,ColPos.z,ColPos.z+15.f) });
		if (EaseT>=1.f)
		{
			HitF = false;
		}
	}
	else
	{
		EaseT = 0.f;
		//���b�V�������肠��
		if (RushMoveEaseT<1.f&&Collision::CircleCollision(Pos.x, Pos.z, 5.f, player->GetPosition().x, player->GetPosition().z, 1.f))
		{
			ColPos = Pos;
			HitF = true;
		}
	}
	Helper::GetInstance()->FloatClamp(EaseT, 0.f, 1.f);

}
