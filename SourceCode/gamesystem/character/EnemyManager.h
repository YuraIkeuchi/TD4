#pragma once
#include "NormalEnemy.h"
#include"FirstBoss.h"
#include "SecondBoss.h"
#include "Player.h"
#include<array>
#include<vector>
//敵の管理系クラス
class EnemyManager {
private:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	EnemyManager(const std::string& sceneName);
	//更新
	void Update();
	//描画
	void Draw(DirectXCommon* dxCommon);
	//ImGui
	void ImGuiDraw();

	void FinishCheck();
public://getter setter
	//敵関係getで取る
	InterBoss* GetBoss() { return enemy.get(); }
	const XMFLOAT3& GetEnemyPosition() { return enemy.get()->GetPosition(); }
	const bool GetEnemyCheck() { return enemy.get()->GetCheck(); }

protected: //静的メンバ変数
private:
	Player* player = Player::GetInstance();
	unique_ptr<InterBoss> enemy;

	static const int firstEnemyMax = 3;
	std::vector<InterEnemy*>bulletenemy;
	static const int tutorialEnemyMax = 5;
	std::array<unique_ptr<InterEnemy>, tutorialEnemyMax>tuatorialenemy;

public:
	//敵の死亡処理
	bool BossDestroy();
};