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
	&ShotAttack::Phase_Idle,//待機
	&ShotAttack::Phase_Shot,//移動
	&ShotAttack::Phase_End,//移動
};

void ShotAttack::Upda()
{
	//状態移行(charastateに合わせる)
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


#pragma region 行動の中身
void ShotAttack::Phase_Idle()
{
	for(auto i=0;i<BulSize;i++)
	{
		ShotObj[i]->SetPosition(boss->GetPosition());
		//攻撃カウンタ
		AttackTimer++;
	}
	FollowPlayer();

	//次フェーズ
	bool next = AttackTimer > 120;

	if (next)_phase = Phase::SHOT;
}

void ShotAttack::Phase_Shot()
{
	
}

void ShotAttack::Phase_End()
{
	
}




