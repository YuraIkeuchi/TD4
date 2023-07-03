#include "ShotAttack.h"

#include "Player.h"

void ShotAttack::Init()
{
	for (auto i = 0; i < BulSize; i++) {
		ShotObj[i].reset(new IKEObject3d());
		ShotObj[i]->Initialize();
		ShotObj[i]->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::Bullet));
	}

}
void (ShotAttack::* ShotAttack::stateTable[])() = {
	&ShotAttack::Phase_Idle,//�ҋ@
	&ShotAttack::Phase_Shot,//�ړ�
	&ShotAttack::Phase_End,//�ړ�
};

void ShotAttack::Upda()
{
	if (_phase == Phase::NON)_phase = Phase::SHOT;
	//��Ԉڍs(charastate�ɍ��킹��)
	(this->*stateTable[_phase])();

	for(auto i=0;i<BulSize;i++)
	{
		ShotObj[i]->SetColor({ 1.f,1.f,1.f ,1.f});
		ShotObj[i]->SetScale({ 1.f,1.f,1.f });
		ShotObj[i]->SetRotation(BulRot[i]);
		ShotObj[i]->SetPosition(BulPos[i]);
		ShotObj[i]->Update();
	}
}

void ShotAttack::Draw()
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
	for(auto i=0;i<BulSize;i++)
	{
		BulPos[i]=(boss->GetPosition());
		//�U���J�E���^
		
	}
	AttackTimer++;
	FollowPlayer();

	//���t�F�[�Y
	bool next = AttackTimer > 120;

	m_Rotation = boss->GetRotation();
	if (next)_phase = Phase::SHOT;
}

void ShotAttack::Phase_Shot()
{

	//m_Rotation.y++;
//	boss->SetRotation(m_Rotation);
	//�e�̌������v���C���[��
	RottoPlayer();
	//�����������ɐi��
	constexpr float SummonSpeed = 2.f;

	for (auto i = 0; i < BulSize; i++)
	{
		move[i] = { 0.f,0.f,0.1f,0.f };
		matRot[i] = XMMatrixRotationY(XMConvertToRadians(boss->GetRotation().y +90+ (static_cast<float>(i * 30.f - 30.f))));
		move[i] = XMVector3TransformNormal(move[i], matRot[i]);

	}
	for (auto i = 0; i < BulSize; i++)
	{
	//�i�s�X�s�[�h
		BulPos[i] = {
			BulPos[i].x + move[i].m128_f32[0] * SummonSpeed,
			BulPos[i].y,
			BulPos[i].z + move[i].m128_f32[2] * SummonSpeed
		};
		//�e�𔖂�
		//BulAlpha[i] -= 0.02f;
	}

	for (auto i = 0; i < BulSize;)
	{
		if (BulAlpha[i] == 0.f)i++;
		else continue;

		

	}
}

void ShotAttack::Phase_End()
{
	AttackTimer = 0;
}


void ShotAttack::RottoPlayer()
{
	//FollowPlayer();

	
	
}


