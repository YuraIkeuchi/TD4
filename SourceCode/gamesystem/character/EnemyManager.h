#pragma once
#include "NormalEnemy.h"
#include"FirstBoss.h"
#include "SecondBoss.h"
#include "Player.h"
#include<array>
//GÌÇnNX
class EnemyManager {
private:
	// DirectX::ðÈª
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	EnemyManager(const std::string& sceneName);
	//XV
	void Update();
	//`æ
	void Draw(DirectXCommon* dxCommon);
	//ImGui
	void ImGuiDraw();

	void FinishCheck();
public://getter setter
	//GÖWgetÅæé
	InterBoss* GetBoss() { return enemy.get(); }
	const XMFLOAT3& GetEnemyPosition() { return enemy.get()->GetPosition(); }
	const bool GetEnemyCheck() { return enemy.get()->GetCheck(); }

protected: //ÃIoÏ
private:
	Player* player = Player::GetInstance();
	unique_ptr<InterBoss> enemy;
	static const int firstEnemyMax = 3;
	std::array<unique_ptr<InterEnemy>, firstEnemyMax>bulletenemy;
	static const int tutorialEnemyMax = 5;
	std::array<unique_ptr<InterEnemy>, tutorialEnemyMax>tuatorialenemy;

public:
	//GÌS
	bool BossDestroy();
};