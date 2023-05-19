#include "EnemyManager.h"
#include "Helper.h"
#include "Input.h"
EnemyManager::EnemyManager(const std::string& sceneName) {
	
	//�V�[���ɂ���ēǂݍ��ރ{�X���Ⴄ
	if (sceneName == "FIRSTSTAGE") {
		enemy.reset(new FirstBoss());
		enemy->Initialize();
		for (auto i = 0; i < bulletenemy.size(); i++) {
			bulletenemy[i].reset(new NormalEnemy());
			bulletenemy[i]->Initialize();
		}

	}
	else if (sceneName == "SECONDSTAGE") {
		enemy.reset(new SecondBoss());
		enemy->Initialize();
		for (auto i = 0; i < bulletenemy.size(); i++) {
			bulletenemy[i].reset(new NormalEnemy());
			bulletenemy[i]->Initialize();
		}
	}
	else if (sceneName == "TUTORIAL") {
		for (auto i = 0; i < tuatorialenemy.size(); i++) {
			tuatorialenemy[i].reset(new NormalEnemy());
			tuatorialenemy[i]->Initialize();
		}
	}
	else {
		assert(0);
	}

}
//�X�V
void EnemyManager::Update() {
	enemy->Update();
}
//�`��
void EnemyManager::Draw(DirectXCommon* dxCommon) {
	enemy->Draw(dxCommon);
	for (auto i = 0; i < bulletenemy.size(); i++)
	{
		bulletenemy[i]->Draw(dxCommon);
	}
}
//ImGui
void EnemyManager::ImGuiDraw() {
	//enemy->ImGuiDraw();
	enemy->ImGuiDraw();
}


//�G�̎��S���u
bool EnemyManager::BossDestroy() {
	if (enemy->GetHP() <= 0.0f) {
		return true;
	}
	else {
		return false;
	}

	return false;
}

//�G�̃`�F�b�N�I��
void EnemyManager::FinishCheck() {
	enemy->SetCheck(false);
}