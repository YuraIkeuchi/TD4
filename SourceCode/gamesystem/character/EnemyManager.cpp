#include "EnemyManager.h"
#include "Helper.h"
#include "Input.h"
EnemyManager::EnemyManager(const std::string& sceneName) {

	//シーンによって読み込むボスが違う
	if (sceneName == "FIRSTSTAGE") {
		enemy.reset(new FirstBoss());
		enemy->Initialize();
	} else if (sceneName == "SECONDSTAGE") {

		enemy.reset(new SecondBoss());
		enemy->Initialize();
	} else {
		assert(0);
	}

	bulletenemy.resize(3);
	for (auto i = 0; i < bulletenemy.size(); i++)
	{
		bulletenemy[i] = (new NormalEnemy());
		bulletenemy[i]->Initialize();
	}
	for (auto i = 0; i < bulletenemy_2.size(); i++)
	{
		bulletenemy_2[i].reset(new NormalEnemy());
		bulletenemy_2[i]->Initialize();
	}
}
//更新
void EnemyManager::Update() {
	enemy->Update();

	if (Input::GetInstance()->TriggerButton(Input::A))Shot_3 = true;
	if (Input::GetInstance()->TriggerButton(Input::X))Shot_8 = true;

	enemy->SummonEnemyUpda(bulletenemy);

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
	ShotAttack_B();

	XMFLOAT3 l_pos = player->GetPosition();

}
//描画
void EnemyManager::Draw(DirectXCommon* dxCommon) {
	enemy->Draw(dxCommon);
	enemy->SummonEnemyDraw(bulletenemy, dxCommon);
	for (auto i = 0; i < bulletenemy_2.size(); i++)
	{
		bulletenemy_2[i]->Draw(dxCommon);
	}
}
//ImGui
void EnemyManager::ImGuiDraw() {
	//enemy->ImGuiDraw();
	enemy->ImGuiDraw();
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
//敵の死亡処置
bool EnemyManager::BossDestroy() {
	if (enemy->GetHP() <= 0.0f) {
		return true;
	} else {
		return false;
	}

	return false;
}

//敵のチェック終了
void EnemyManager::FinishCheck() {
	enemy->SetCheck(false);
}