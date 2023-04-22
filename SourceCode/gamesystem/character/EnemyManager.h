#pragma once
#include "NormalEnemy.h"
#include "Player.h"
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
	EnemyManager(Player* _player);
	//更新
	void Update();
	//描画
	void Draw(DirectXCommon* dxCommon);
	//ImGui
	void ImGuiDraw();

	
public://getter setter
	//static void SetPlayer(Player* player) { EnemyManager::player = player; }
protected: //静的メンバ変数
	static Player* player;
private:
	unique_ptr<InterEnemy> enemy;
};