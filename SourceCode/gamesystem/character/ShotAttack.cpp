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
	&ShotAttack::Phase_Idle,//待機
	&ShotAttack::Phase_Shot,//移動
	&ShotAttack::Phase_End,//移動
};

void ShotAttack::Upda()
{
	if (_phase == Phase::NON)_phase = Phase::SHOT;
	//状態移行(charastateに合わせる)
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


#pragma region 行動の中身
void ShotAttack::Phase_Idle()
{
	for(auto i=0;i<BulSize;i++)
	{
		BulPos[i]=(boss->GetPosition());
		//攻撃カウンタ
		
	}
	AttackTimer++;
	FollowPlayer();

	//次フェーズ
	bool next = AttackTimer > 120;

	m_Rotation = boss->GetRotation();
	if (next)_phase = Phase::SHOT;
}

void ShotAttack::Phase_Shot()
{

	//m_Rotation.y++;
//	boss->SetRotation(m_Rotation);
	//弾の向きをプレイヤーに
	RottoPlayer();
	//向いた方向に進む
	constexpr float SummonSpeed = 2.f;

	for (auto i = 0; i < BulSize; i++)
	{
		move[i] = { 0.f,0.f,0.1f,0.f };
		matRot[i] = XMMatrixRotationY(XMConvertToRadians(boss->GetRotation().y +90+ (static_cast<float>(i * 30.f - 30.f))));
		move[i] = XMVector3TransformNormal(move[i], matRot[i]);

	}
	for (auto i = 0; i < BulSize; i++)
	{
	//進行スピード
		BulPos[i] = {
			BulPos[i].x + move[i].m128_f32[0] * SummonSpeed,
			BulPos[i].y,
			BulPos[i].z + move[i].m128_f32[2] * SummonSpeed
		};
		//弾を薄く
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


