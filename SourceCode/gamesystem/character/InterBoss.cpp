#include "InterBoss.h"
#include"Collision.h"
#include "Helper.h"
#include "ParticleEmitter.h"
#include "VariableCommon.h"
#include <Helper.h>
//çXêV
void InterBoss::Update() {
	//çsìÆ
	Action();
	
}
//ï`âÊ
void InterBoss::Draw(DirectXCommon* dxCommon) {
}
//ImGuiï`âÊ
void InterBoss::ImGuiDraw() {
	ImGui_Origin();//ÇªÇÍÇºÇÍÇÃImGui
}

float InterBoss::HpPercent() {

	float temp = m_HP / m_MaxHp;
	Helper::GetInstance()->FloatClamp(temp, 0.0f, 1.0f);
	return temp;
}

//íeÇ∆ÇÃìñÇΩÇËîªíË
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
			}
		}
	}
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
	//âºÇÃäiî[îzóÒ
	bool tempList[3];
	//ëSïîî≠éÀèÛë‘Ç»ÇÁ
	for (auto i = 0; i < _countof(tempList); i++) {
		if (enemy[i] == nullptr)continue;
		tempList[i] = enemy[i]->GetShotF();
	}
	if (SummobnStop) {
		//è¢ä´èÛë‘âèúÅ@â~â^ìÆçƒäJ
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