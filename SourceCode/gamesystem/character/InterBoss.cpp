#include "InterBoss.h"
#include"Collision.h"
#include "Helper.h"
#include "ParticleEmitter.h"
#include "VariableCommon.h"
#include <Helper.h>
//�X�V
void InterBoss::Update() {
	//�s��
	Action();
	
	//�G�t�F�N�g
	for (InterEffect* effect : effects) {
		if (effect != nullptr) {
			effect->Update();
		}
	}

	//�}�[�N�̍폜
	for (int i = 0; i < effects.size(); i++) {
		if (effects[i] == nullptr) {
			continue;
		}

		if (!effects[i]->GetAlive()) {
			effects.erase(cbegin(effects) += i);
		}
	}
}
//�`��
void InterBoss::Draw(DirectXCommon* dxCommon) {
}
//ImGui�`��
void InterBoss::ImGuiDraw() {
	ImGui_Origin();//���ꂼ���ImGui
}

float InterBoss::HpPercent() {

	float temp = m_HP / m_MaxHp;
	Helper::GetInstance()->FloatClamp(temp, 0.0f, 1.0f);
	return temp;
}

//�e�Ƃ̓����蔻��
void InterBoss::CollideBul(vector<InterBullet*> bullet)
{
	if (ColChangeEaseT>0.f)return;
	constexpr float BulRad = 1.f;

	constexpr float BossRad = 3.f;

	for (InterBullet* _bullet : bullet) {
		if (_bullet != nullptr) {
			if (Collision::CircleCollision(_bullet->GetPosition().x, _bullet->GetPosition().z, BulRad, m_Position.x, m_Position.z, BossRad))
			{
				Audio::GetInstance()->PlayWave("Resources/Sound/SE/Attack_Normal.wav", VolumManager::GetInstance()->GetSEVolum());
				ActionTimer++;
				Recv = true;
				_bullet->SetAlive(false);
				if (_bullet->GetScale().x == 1.0f) {
					m_HP--;
				} else {
					m_HP -= 2.0f;
				}
				BirthEffect();
			}
		}
	}
}

//�G�t�F�N�g�̔���
void InterBoss::BirthEffect() {
	InterEffect* neweffect;
	neweffect = new BreakEffect();
	neweffect->Initialize();
	neweffect->SetPosition(m_Position);
	effects.push_back(neweffect);
}

void InterBoss::SummonEnemyInit(InterEnemy* enemy)
{

}

void InterBoss::isRespawn(std::vector<InterEnemy*> enemy)
{
	if(ResF)
	{
		for(auto i=0;i<3;i++)
		{
			NextActionInteval = 0;
			enemy[i]->SetHP(1);
			enemy[i]->SetScale({ 0,0,0});
			enemy[i]->SetColor({ 1,1,1,1 });
			enemy[i]->SetShotF(false);
			enemy[i]->SetcanRotandRush(false);
			enemy[i]->SetAlive(true);
			
		}
		ResF = false;
	}
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
			if (enemy[i] == nullptr)continue;
			move[i] = { 0.f,0.f, 0.1f, 0.0f };
			matRot[i] = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y + (0.f + static_cast<float>(i) * 45.f)));
			move[i] = XMVector3TransformNormal(move[i], matRot[i]);

			enemy[i]->SetPosition({ m_Position.x + move[i].m128_f32[0] * 90.f,m_Position.y,	m_Position.z + move[i].m128_f32[2] * 90.f });
		}
	}

	if (SummonF) {
		
		for (auto i = 0; i < enemy.size(); i++)
		{
			if (enemy[i] == nullptr)continue;
			enemy[i]->Update();
		}
		for (auto i = 0; i < enemy.size(); i++) {
			if (enemy[i] == nullptr)continue;
			enemy[i]->SetShotF(true);
		}
		
	}
	EndSummon(enemy);
}

void InterBoss::EndSummon(std::vector<InterEnemy*> enemy)
{
	//���̊i�[�z��
	bool tempList[3];
	//�S�����ˏ�ԂȂ�
	for (auto i = 0; i < _countof(tempList); i++) {
		if (enemy[i] == nullptr)continue;
		tempList[i] = enemy[i]->GetShotF();
	}
	if (SummobnStop) {
		//������ԉ����@�~�^���ĊJ
		if (Helper::GetInstance()->All_Of(tempList, _countof(tempList))) {
			
			NextActionInteval++;
			if (NextActionInteval > 180) {
				ActionTimer++;
				EndSummonRepos = true;
				SummobnStop = false;
			}
		}
	}
	else
	{
		NextActionInteval = 0;
	}
	bool tempList2[3];
	for (auto i = 0; i < _countof(tempList2); i++) {
		if (enemy[i] == nullptr)continue;
		tempList2[i] = !enemy[i]->GetisAlive() && enemy[i]->GEtAlpha() <= 0.f;
	}
	if(Helper::GetInstance()->All_Of(tempList2,_countof(tempList2)))
	{
		SummonF = false;
	}
}

void InterBoss::SummonEnemyDraw(std::vector<InterEnemy*> enemy, DirectXCommon* dxcomn)
{
	for (auto i = 0; i < enemy.size(); i++)
	{
		if (enemy[i] == nullptr)continue;
		enemy[i]->Draw(dxcomn);
	}
}