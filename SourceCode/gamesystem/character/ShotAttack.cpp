#include "ShotAttack.h"

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
	//��Ԉڍs(charastate�ɍ��킹��)
	(this->*stateTable[static_cast<int>(_phase)])();
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
		ShotObj[i]->SetPosition(boss->GetPosition());
		//�U���J�E���^
		AttackTimer++;
	}
	FollowPlayer();

	//���t�F�[�Y
	bool next = AttackTimer > 120;

	if (next)_phase = Phase::SHOT;
}

void ShotAttack::Phase_Shot()
{
	
}

void ShotAttack::Phase_End()
{
	
}




