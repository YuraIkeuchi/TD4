#include "ShotAttack.h"
#include <random>
#include"Easing.h"
#include "Collision.h"
#include "CsvLoader.h"
#include "Helper.h"
#include "Player.h"

void ShotAttack::Init()
{
	for (auto i = 0; i < BulSize; i++) {
		ShotObj[i].reset(new IKEObject3d());
		ShotObj[i]->Initialize();
		ShotObj[i]->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::GhostBullet));

		BulAlpha[i] = 1.f;
	}
	WalkSpeed = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/five/Fiveboss.csv", "WalkSpeed")));


}
void (ShotAttack::* ShotAttack::stateTable[])() = {
	&ShotAttack::Phase_Idle,//�ҋ@
	&ShotAttack::Phase_Shot,//�ړ�
	&ShotAttack::Phase_End,//�ړ�
};

void ShotAttack::Upda()
{
	m_Position = boss->GetPosition();
	for (auto i = 0; i < boss->GetGhost().size(); i++) {
		boss->GetGhost()[i]->SetFivePos(boss->GetPosition());
	}
	for (auto i = 0; i < 3; i++) {
		obb[i].SetParam_Pos(ShotObj[i]->GetPosition());
		obb[i].SetParam_Rot(ShotObj[i]->GetMatrot());
		obb[i].SetParam_Scl({ 2.f,2.f,5.f });
	}
	if (_phase == SHOT) {
		for (auto i = 0; i < boss->GetGhost().size(); i++) {
			if (boss->GetGhost()[i]->GetState() != Ghost::STATE_NONE)continue;
			if (boss->GetGhost()[i]->GetScale().x <= 0.f)continue;
			if (!boss->GetGhost()[i]->JugNONE())continue;
			for (auto k = 0; k < 3; k++) {
				if ( boss->GetGhost()[i]->CollideDarkBul(obb[k]))
				{
					if (!BulAlive[k])continue;
					
						DarkCount++;
					boss->GetGhost()[i]->SetCollide(true);
						boss->SetGhostSize(boss->GetGhostSize() + 1);
						CanRand = rand() % 100;
						

						BulAlive[k] = false;
					
					continue;
				}
			}
		}
	}
	{
		for (auto i = 0; i < boss->GetGhost().size(); ++i) {
			for (auto j = 0; j < boss->GetGhost().size(); ++j) {
				XMFLOAT3 ghostpos = boss->GetGhost()[i]->GetPosition();
				XMFLOAT3 ghostpos2 = boss->GetGhost()[j]->GetPosition();
				if ((i == j)) { continue; }
				if (boss->GetGhost()[i]->GetState() != Ghost::STATE_DARKOTI)continue;
				if (boss->GetGhost()[j]->GetState() != Ghost::STATE_DARKOTI)continue;

				if ((!boss->GetGhost()[i]->GetAlive()) || (!boss->GetGhost()[j]->GetAlive())) { continue; }
				if ((!boss->GetGhost()[i]->GetCollide()) || (!boss->GetGhost()[j]->GetCollide())) { continue; }
				if (Collision::SphereCollision(ghostpos, 1.5f, ghostpos2, 1.5f)) {
					boss->GetGhost()[i]->GhostCollision(ghostpos2);
					boss->GetGhost()[j]->GhostCollision(ghostpos);
				}
			}
		}
	}
	//��Ԉڍs(charastate�ɍ��킹��)
	(this->*stateTable[_phase])();
BulRot[0].y = boss->GetRotation().y - 30.f+90;
		BulRot[1].y = boss->GetRotation().y + 90;
		BulRot[2].y = boss->GetRotation().y + 30.f + 90;
	for (auto i = 0; i < BulSize; i++)
	{
		if (!BulAlive[i])
			BulAlpha[i] = 0.f;
		
		ShotObj[i]->SetColor({ 1.f,0.3f,1.f ,BulAlpha[i] });
		ShotObj[i]->SetScale({ 1.f,1.f,1.f });
		ShotObj[i]->SetRotation(BulRot[i]);
		ShotObj[i]->SetPosition(BulPos[i]);
		ShotObj[i]->Update();
	}
	boss->SetPosition(m_Position);

	//�U���I�����̏���������
	if (ActionEnd)
	{
		RotEaseTime = 0;
		for (auto i = 0; i < BulSize; i++)
			BulAlpha[i] = 1.f;
		PhaseCount = 0;
		AttackTimer = 0;
		boss->SetRecv(false);
		_phase = NON;

	}
}

void ShotAttack::Draw(DirectXCommon* dxCommon)
{
	if (_phase != SHOT)return;
	IKEObject3d::PreDraw();
	for (auto i = 0; i < BulSize; i++) {
		if (BulAlpha[i] <= 0.f)continue;
		if (!BulAlive[i])continue;
		ShotObj[i]->Draw();
	}
	IKEObject3d::PostDraw();


}

void ShotAttack::SpriteDraw()
{
	IKESprite::PreDraw();
	for (auto i = 0; i < 4; i++)
		ShotArea[i]->Draw();
	IKESprite::PostDraw();
}


#pragma region �s���̒��g
void ShotAttack::Phase_Idle()
{
	AttackTimer++;
	FollowPlayerAct();
	boss->AnimationControl(InterBoss::AnimeName::WALK, true, 1);
	bool next = Collision::GetLength(boss->GetGhost()[TargetGhost]->GetPosition(), boss->GetPosition()) < 15;
	//���t�F�[�Y

	m_Rotation = boss->GetRotation();
	if (TargetGhost == 0) {
		AttackTimer++;
		if (AttackTimer > 120)
		{
			for (auto i = 0; i < BulSize; i++) {
				BulPos[i] = boss->GetPosition();
			}

			mt19937 mt{ std::random_device{}() };
			uniform_int_distribution<int> l_Rand(1, (int)boss->GetGhost().size() - 1);
			TargetGhost = l_Rand(mt);

			_phase = Phase::SHOT;
		}
	} else
	{
		AttackTimer++;
		if (next || AttackTimer > 120) {
			mt19937 mt{ std::random_device{}() };
			uniform_int_distribution<int> l_Rand(1, (int)boss->GetGhost().size() - 1);
			TargetGhost = l_Rand(mt);

			TriggerAttack = true;
			boss->AnimationControl(InterBoss::AnimeName::SHOT, false, 1);

			_phase = Phase::SHOT;
		}

	}
}
void ShotAttack::Phase_Shot()
{
	//�e�̌������v���C���[��
	RottoPlayer();
	//�����������ɐi��

	for (auto i = 0; i < BulSize; i++)
	{
		move[i] = { 0.f,0.f,0.1f,0.f };
		matRot[i] = XMMatrixRotationY(XMConvertToRadians(boss->GetRotation().y - 0.f + (static_cast<float>(i * 30.f - 30.f))));
		move[i] = XMVector3TransformNormal(move[i], matRot[i]);
		
	}
	for (auto i = 0; i < BulSize; i++)
	{
		BulAlive[i] = true;
		if (BulAlpha[i] >= 1.f)BulPos[i] = boss->GetPosition();
		//�i�s�X�s�[�h
		BulPos[i].x += move[i].m128_f32[0] * WalkSpeed;
		BulPos[i].z += move[i].m128_f32[2] * WalkSpeed;
		//�e�𔖂�
		BulAlpha[i] -= 0.01f;

	}

	if (BulAlpha[1] <= 0.f&&
		BulAlpha[2] <= 0.f&&
		BulAlpha[0] <= 0.f) {
		OldRot = boss->GetRotation();
		PhaseCount++;
		_phase = END;
	}
	RotEaseTime = 0.f;



}
void ShotAttack::Collide()
{
	
	

}


void ShotAttack::Phase_End()
{
	if (TriggerAttack) {
		if (boss->GetFbxobj()->GetCurrent() >= boss->GetFbxobj()->GetEndTime() - 1) {
			boss->GetFbxobj()->StopAnimation();
			boss->AnimationControl(InterBoss::AnimeName::IDLE, true, 1);
			TriggerAttack = false;
		}
	}
			if (RotEaseTime == 0.f) {
		mt19937 mt{ std::random_device{}() };
		uniform_int_distribution<int> l_RandRot(-100, 100);

		XMVECTOR PositionB = { boss->GetPosition().x,
			boss->GetPosition().y,
			boss->GetPosition().z,
		};

		XMVECTOR PositionA = { boss->GetGhost()[TargetGhost]->GetPosition().x
			,boss->GetGhost()[TargetGhost]->GetPosition().y,
			boss->GetGhost()[TargetGhost]->GetPosition().z };
		//�v���C���[�ƓG�̃x�N�g���̒���(��)�����߂�
		XMVECTOR SubVector = XMVectorSubtract(PositionB, PositionA); // positionA - positionB;


		RottoGhost = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);
	}
	for (auto i = 0; i < BulSize; i++)
	{
		BulAlive[i] = true;
		BulAlpha[i] = 1.f;
		BulPos[i] = boss->GetPosition();
	}


	Helper::GetInstance()->FrameCheck(RotEaseTime, 0.04f);

	boss->SetRotation({ boss->GetRotation().x,
	Ease(In,Quad,RotEaseTime,OldRot.y,RottoGhost * 60 + 180),
	boss->GetRotation().z });
	AttackTimer = 0;
	//if (PhaseCount < 4) {
	if (RotEaseTime >= 1.f)
		_phase = NON;
	//}

	//else ActionEnd = true;
}


void ShotAttack::RottoPlayer()
{
	//FollowPlayer();



}


void ShotAttack::FollowPlayerAct()
{
	
	XMVECTOR move = { 0.f,0.f, 0.1f, 0.0f };

	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(boss->GetRotation().y));

	move = XMVector3TransformNormal(move, matRot);
	
	m_Position = {
					m_Position.x + move.m128_f32[0] * WalkSpeed,
				m_Position.y,
				m_Position.z + move.m128_f32[2] * WalkSpeed
	};

	boss->SetPosition(m_Position);
}
