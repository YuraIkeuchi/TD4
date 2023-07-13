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
	&ShotAttack::Phase_Idle,//待機
	&ShotAttack::Phase_Shot,//移動
	&ShotAttack::Phase_End,//移動
};

void ShotAttack::Upda()
{
	m_Position = boss->GetPosition();

	for (auto i = 0; i < boss->GetGhost().size(); i++) {
		if (boss->GetGhost()[i]->GetState() == Ghost::STATE_DARKOTI)continue;
		if (boss->GetGhost()[i]->GetScale().x <= 0.f)continue;
		for (auto k = 0; k < 3; k++) {

			boss->GetGhost()[i]->SetFivePos(boss->GetPosition());

			if (Collision::GetLength(BulPos[k], boss->GetGhost()[i]->GetPosition()) < 5.f)
			{
				if (!BulAlive[k])continue;
				if (boss->GetGhost()[i]->JugNONE()) {
					DarkCount++;
					boss->SetGhostSize(boss->GetGhostSize() + 1);
					CanRand = rand() % 100;
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
	if (_phase != SHOT && boss->GetGhostSize() > 0)
	{
		//ダメージを喰らい攻撃可能なら
		if (boss->GetRecv())
		{
			_phase = NON;
			ActionEnd = true;
			IdleRecv = true;
		}
	}

	//状態移行(charastateに合わせる)
	(this->*stateTable[_phase])();

	for (auto i = 0; i < BulSize; i++)
	{
		if (!BulAlive[i])
			BulAlpha[i] = 0.f;

		ShotObj[i]->SetColor({ 1.f,1.f,1.f ,BulAlpha[i] });
		ShotObj[i]->SetScale({ 1.f,1.f,1.f });
		ShotObj[i]->SetRotation(BulRot[i]);
		ShotObj[i]->SetPosition(BulPos[i]);
		ShotObj[i]->Update();
	}
	boss->SetPosition(m_Position);

	//攻撃終了時の初期化周り
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


#pragma region 行動の中身
void ShotAttack::Phase_Idle()
{
	AttackTimer++;
	FollowPlayerAct();

	bool next = Collision::GetLength(boss->GetGhost()[TargetGhost]->GetPosition(), boss->GetPosition()) < 15;
	//次フェーズ

	m_Rotation = boss->GetRotation();
	if (TargetGhost == 0) {
		AttackTimer++;
		if (AttackTimer > 120)
		{
			for (auto i = 0; i < BulSize; i++) {
				BulPos[i] = boss->GetPosition();
			}

			mt19937 mt{ std::random_device{}() };
			uniform_int_distribution<int> l_Rand(0, (int)boss->GetGhost().size() - 1);
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

			_phase = Phase::SHOT;
		}

	}
}
void ShotAttack::Phase_Shot()
{
	//弾の向きをプレイヤーに
	RottoPlayer();
	//向いた方向に進む
	constexpr float SummonSpeed = 4.f;

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
		//進行スピード
		BulPos[i].x += move[i].m128_f32[0] * SummonSpeed;
		BulPos[i].z += move[i].m128_f32[2] * SummonSpeed;
		//弾を薄く
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

void ShotAttack::Phase_End()
{
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
		//プレイヤーと敵のベクトルの長さ(差)を求める
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

	float SummonSpeed = 6.f;
	m_Position = {
					m_Position.x + move.m128_f32[0] * SummonSpeed,
				m_Position.y,
				m_Position.z + move.m128_f32[2] * SummonSpeed
	};

	boss->SetPosition(m_Position);
}
