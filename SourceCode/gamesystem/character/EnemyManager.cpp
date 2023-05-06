#include "EnemyManager.h"

#include "Helper.h"
Player* EnemyManager::player = nullptr;
EnemyManager::EnemyManager(Player* _player) {
	player = _player;


	enemy.reset(new FirstBoss());
	enemy->Initialize();
	enemy->SetPlayer(player);

	for (auto i = 0; i < bulletenemy.size(); i++)
	{
		bulletenemy[i].reset(new NormalEnemy());
		bulletenemy[i]->Initialize();
		bulletenemy[i]->SetPlayer(player);
	}
	for (auto i = 0; i < bulletenemy_2.size(); i++)
	{
		bulletenemy_2[i].reset(new NormalEnemy());
		bulletenemy_2[i]->Initialize();
		bulletenemy_2[i]->SetPlayer(player);


	}

}
//�X�V
void EnemyManager::Update() {
	enemy->Update();

	if (Input::GetInstance()->TriggerButton(Input::A))Shot_3 = true;
	if (Input::GetInstance()->TriggerButton(Input::X))Shot_8 = true;

	if (!Shot_3) {
		for (auto i = 0; i < bulletenemy.size(); i++) {
			bulletenemy[i]->SetPosition(EPos[i]);
		}
	}

	constexpr float PosInter = 20.f;

	if (!Shot_8) {
		for (auto i = 0; i < bulletenemy_2.size(); i++)
		{
			enemyAngle[i] = static_cast<float>(i) * PI_360 / bulletenemy_2.size();

			bulletenemy_2[i]->SetPosition({
				sinf(static_cast<float>(i) * (PI_360 / static_cast<float>(bulletenemy_2.size())) * (PI / PI_180)) * PosInter,
				0.f,

				cosf(static_cast<float>(i) * (PI_360 / static_cast<float>(bulletenemy_2.size())) * (PI / PI_180)) * PosInter
				});
		}
	}
	ShotAttack_A();
	ShotAttack_B();

	XMFLOAT3 l_pos = player->GetPosition();

}
//�`��
void EnemyManager::Draw(DirectXCommon* dxCommon) {
	enemy->Draw(dxCommon);
	for (auto i = 0; i < bulletenemy.size(); i++)
	{
		bulletenemy[i]->Draw(dxCommon);
	}

	for (auto i = 0; i < bulletenemy_2.size(); i++)
	{
		bulletenemy_2[i]->Draw(dxCommon);
	}
}
//ImGui
void EnemyManager::ImGuiDraw() {
	//enemy->ImGuiDraw();
}

void EnemyManager::ShotAttack_A()
{
	if (!Shot_3)return;
	for (auto i = 0; i < bulletenemy.size(); i++)
	{
		bulletenemy[i]->Update();
	}

	bulletenemy[0]->SetShotF(true);

	for (auto i = 1; i < bulletenemy.size(); i++) {
		if (bulletenemy[i - 1]->GetShotCount() > 90)
		{
			bulletenemy[i]->SetShotF(true);
		}
	}
}

void EnemyManager::ShotAttack_B()
{
	if (!Shot_8)return;
	for (auto i = 0; i < bulletenemy_2.size(); i++)
	{
		bulletenemy_2[i]->Update();
	}


	for (auto i = 0; i < bulletenemy_2.size(); i++)
	{
		bulletenemy_2[i]->SetShotF(true);
	}
}

