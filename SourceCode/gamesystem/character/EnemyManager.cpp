#include "EnemyManager.h"
Player* EnemyManager::player = nullptr;
EnemyManager::EnemyManager(Player* _player) {
	player = _player;
	enemy.reset(new NormalEnemy());
	enemy->Initialize();

	enemy->SetPlayer(player);
}
//XV
void EnemyManager::Update() {
	enemy->Update();

	XMFLOAT3 l_pos = player->GetPosition();

}
//•`‰æ
void EnemyManager::Draw(DirectXCommon* dxCommon) {
	enemy->Draw(dxCommon);
}
//ImGui
void EnemyManager::ImGuiDraw() {
	enemy->ImGuiDraw();
}
