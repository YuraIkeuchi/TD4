#include "ShotAttack.h"

#include "Collision.h"
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
		//
		boss->GetGhost()[i]->SetFivePos(boss->GetPosition());

		if (Collision::GetLength(BulPos[0], boss->GetGhost()[i]->GetPosition()) < 5.f ||
			Collision::GetLength(BulPos[1], boss->GetGhost()[i]->GetPosition()) < 5.f ||
			Collision::GetLength(BulPos[2], boss->GetGhost()[i]->GetPosition()) < 5.f)
		{
			if (boss->GetGhost()[i]->JugNONE()) {
				boss->GetGhost()[i]->SetCollide(true);
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
	for (auto i = 0; i < BulSize; i++)
		ShotObj[i]->Draw();
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
	FollowPlayer();

	//���t�F�[�Y
	bool next = AttackTimer > 120;

	m_Rotation = boss->GetRotation();
	if (next) {
		for (auto i = 0; i < BulSize; i++)
			BulPos[i] = boss->GetPosition();

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

}

void ShotAttack::Phase_End()
{
	for(auto i=0;i<BulSize;i++)
	{
		BulAlpha[i] = 1.f;
		BulPos[i] = boss->GetPosition();
	}
	boss->SetRotation({ boss->GetRotation().x,
	OldRot.y + 90.f,
	boss->GetRotation().z });
	if (PhaseCount < 4)_phase = SHOT;
	else ActionEnd = true;
}


void ShotAttack::RottoPlayer()
{
	//FollowPlayer();

	
	
}


