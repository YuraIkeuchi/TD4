#pragma once
#include "NormalEnemy.h"
#include"FirstBoss.h"
#include "SecondBoss.h"
#include "Player.h"
#include<array>
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


public://getter setter
	//static void SetPlayer(Player* player) { EnemyManager::player = player; }
protected: //静的メンバ変数
private:
	Player* player = Player::GetInstance();
	unique_ptr<InterBoss> enemy;
	std::array<unique_ptr<InterEnemy>, 3>bulletenemy;

	std::array<unique_ptr<InterEnemy>, 8>bulletenemy_2;
	std::array<float, 8>enemyAngle;

	bool Shot_3;
	bool Shot_8;

	void ShotAttack_A();
	void ShotAttack_B();


	std::array<XMFLOAT3, 3> EPos;



public:
	//3つのオブジェクト位置設定
	inline void SetEpos_A(XMFLOAT3* pos) { for (auto i = 0; i < 3; i++)EPos[i] = pos[i]; }

	//敵の死亡処理
	bool BossDestroy();
};