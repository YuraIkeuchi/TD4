#include "InterBoss.h"
#include"Collision.h"
#include "Helper.h"
#include "ParticleEmitter.h"
#include "VariableCommon.h"
//�X�V
void InterBoss::Update() {
	//�s��
	Action();
}
//�`��
void InterBoss::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
	EffecttexDraw(dxCommon);
}
//ImGui�`��
void InterBoss::ImGuiDraw() {
	ImGui_Origin();//���ꂼ���ImGui
}

//�e�Ƃ̓����蔻��
void InterBoss::CollideBul(vector<InterBullet*> bullet)
{
	constexpr float BulRad = 1.f;

	constexpr float BossRad = 3.f;

	for (InterBullet* _bullet : bullet) {
		if (_bullet != nullptr) {
			if (Collision::CircleCollision(_bullet->GetPosition().x, _bullet->GetPosition().z, BulRad, m_Position.x, m_Position.z, BossRad))
			{
				Audio::GetInstance()->PlayWave("Resources/Sound/SE/Attack_Normal.wav", VolumManager::GetInstance()->GetSEVolum());
				Recv = true;
				_bullet->SetAlive(false);
				if (_bullet->GetScale().x == 1.0f) {
					m_HP--;
				} else {
					m_HP -= 2.0f;
				}
			}
		}
	}
}


void InterBoss::SummonEnemyInit(InterEnemy* enemy)
{

}

void InterBoss::SummonEnemyUpda(std::vector<InterEnemy*> enemy)
{

	std::vector<XMVECTOR> move(enemy.size());
	std::vector<XMMATRIX>matRot(enemy.size());

	for (auto i = 0; i < enemy.size(); i++) {
		move[i] = { 0.f,0.f, 0.1f, 0.0f };
	}
	if (!SummonF) {
		for (auto i = 0; i < enemy.size(); i++) {
			move[i] = { 0.f,0.f, 0.1f, 0.0f };
			matRot[i] = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y + (0.f + static_cast<float>(i) * 45.f)));
			move[i] = XMVector3TransformNormal(move[i], matRot[i]);

			enemy[i]->SetPosition({ m_Position.x + move[i].m128_f32[0] * 90.f,m_Position.y,	m_Position.z + move[i].m128_f32[2] * 90.f });
		}
	}

	if (SummonF) {
		for (auto i = 0; i < enemy.size(); i++)
		{
			enemy[i]->Update();
		}

		enemy[0]->SetShotF(true);

		for (auto i = 1; i < enemy.size(); i++) {
			if (enemy[i - 1]->GetShotCount() > 10)
			{
				enemy[i]->SetShotF(true);
			}
		}
		
	}
	EndSummon(enemy);
}

void InterBoss::EndSummon(std::vector<InterEnemy*> enemy)
{
	//���̊i�[�z��
	bool tempList[3];
	//�S�����ˏ�ԂȂ�
	for (auto i = 0; i < _countof(tempList); i++)
		tempList[i] = enemy[i]->GetShotF();

	if (SummobnStop) {
		//������ԉ����@�~�^���ĊJ
		if (Helper::GetInstance()->All_Of(tempList, _countof(tempList))) {
			ActionTimer++;
			SummobnStop = false;
		}
	}
}

void InterBoss::SummonEnemyDraw(std::vector<InterEnemy*> enemy, DirectXCommon* dxcomn)
{
	for (auto i = 0; i < enemy.size(); i++)
	{
		enemy[i]->Draw(dxcomn);
	}
}