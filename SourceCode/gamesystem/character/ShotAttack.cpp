#include "ShotAttack.h"

#include <random>

#include"Easing.h"
#include "Collision.h"
#include "Helper.h"
#include "Player.h"

void ShotAttack::Init()
{
	for (auto i = 0; i < BulSize; i++) {
		ShotObj[i].reset(new IKEObject3d());
		ShotObj[i]->Initialize();
		ShotObj[i]->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::Bullet));

		BulAlpha[i] = 1.f;
	}

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
		for (auto k = 0; k < 3; k++) {
			//
			boss->GetGhost()[i]->SetFivePos(boss->GetPosition());

			if (Collision::GetLength(BulPos[k], boss->GetGhost()[i]->GetPosition()) < 5.f)
			{
				if (!BulAlive[k])continue;
				if (boss->GetGhost()[i]->JugNONE()) {
					DarkCount++;
					boss->GetGhost()[i]->SetCollide(true);
				BulAlive[k] = false;
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
				if ((!boss->GetGhost()[i]->GetAlive()) || (!boss->GetGhost()[j]->GetAlive())) { continue; }
				if ((!boss->GetGhost()[i]->GetCollide()) || (!boss->GetGhost()[j]->GetCollide())) { continue; }
				if (Collision::SphereCollision(ghostpos, 1.5f, ghostpos2, 1.5f)) {
					boss->GetGhost()[i]->GhostCollision(ghostpos2);
					boss->GetGhost()[j]->GhostCollision(ghostpos);
				}
			}
		}
	}


	if (_phase == Phase::NON&&!ActionEnd) {
		_phase = Phase::SHOT;
	}
		//��Ԉڍs(charastate�ɍ��킹��)
	(this->*stateTable[_phase])();

	for(auto i=0;i<BulSize;i++)
	{
		if (!BulAlive[i])
			BulAlpha[i] = 0.f;

		ShotObj[i]->SetColor({ 1.f,1.f,1.f ,BulAlpha[i]});
		ShotObj[i]->SetScale({ 1.f,1.f,1.f });
		ShotObj[i]->SetRotation(BulRot[i]);
		ShotObj[i]->SetPosition(BulPos[i]);
		ShotObj[i]->Update();
	}
	boss->SetPosition(m_Position);

	//�U���I�����̏���������
	if(ActionEnd)
	{
		for(auto i=0;i<BulSize;i++)
		BulAlpha[i] = 1.f;
		PhaseCount = 0;
		AttackTimer = 0;
		_phase = NON;

	}
}

void ShotAttack::Draw(DirectXCommon* dxCommon)
{
	IKEObject3d::PreDraw();
	for (auto i = 0; i < BulSize; i++) {
		if (BulAlpha[i] <= 0.f)continue;
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

	//���t�F�[�Y
	bool next = AttackTimer > 120;

	m_Rotation = boss->GetRotation();
	if (next) {
		for (auto i = 0; i < BulSize; i++) {
			BulAlive[i] = true;
			BulPos[i] = boss->GetPosition();
		}
		_phase = Phase::SHOT;
	}
	}

void ShotAttack::Phase_Shot()
{
	//�e�̌������v���C���[��
	RottoPlayer();
	//�����������ɐi��
	constexpr float SummonSpeed = 4.f;

	for (auto i = 0; i < BulSize; i++)
	{
		move[i] = { 0.f,0.f,0.1f,0.f };
		matRot[i] = XMMatrixRotationY(XMConvertToRadians(boss->GetRotation().y -0.f+ (static_cast<float>(i * 30.f - 30.f))));
		move[i] = XMVector3TransformNormal(move[i], matRot[i]);

	}
	for (auto i = 0; i < BulSize; i++)
	{
		if(BulAlpha[i]>=1.f)BulPos[i] = boss->GetPosition();
	//�i�s�X�s�[�h
		BulPos[i].x += move[i].m128_f32[0] * SummonSpeed;
		BulPos[i].z += move[i].m128_f32[2] * SummonSpeed;
		//�e�𔖂�
		BulAlpha[i] -= 0.01f;
		
	}

	if (
		BulAlpha[0] <= 0.f) {
		OldRot=boss->GetRotation();
		PhaseCount++;
		_phase = END;
	}
	RotEaseTime = 0.f;

}

void ShotAttack::Phase_End()
{
	for(auto i=0;i<BulSize;i++)
	{
		BulAlive[i] = true;
		BulAlpha[i] = 1.f;
		BulPos[i] = boss->GetPosition();
	}
	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_RandRot(-60, 60);

	Helper::GetInstance()->FrameCheck(RotEaseTime, 0.05f);

	boss->SetRotation({ boss->GetRotation().x,
	Ease(In,Quad,RotEaseTime,OldRot.y,OldRot.y+l_RandRot(mt)),
	boss->GetRotation().z });
	if (PhaseCount < 4) {
		if(RotEaseTime>=1.0f)
		_phase = NON;
	}
	else ActionEnd = true;
}


void ShotAttack::RottoPlayer()
{
	//FollowPlayer();

	
	
}


void ShotAttack::FollowPlayerAct()
{
	FollowPlayer();
	XMVECTOR move = { 0.f,0.f, 0.1f, 0.0f };

	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(boss->GetRotation().y));

	move = XMVector3TransformNormal(move, matRot);

	float SummonSpeed = 2.f;
	m_Position = {
					m_Position.x + move.m128_f32[0] * SummonSpeed,
				m_Position.y,
				m_Position.z + move.m128_f32[2] * SummonSpeed
	};

	boss->SetPosition(m_Position);
}
