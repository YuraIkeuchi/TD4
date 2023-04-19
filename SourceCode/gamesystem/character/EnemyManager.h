#pragma once
#include "NormalEnemy.h"
#include "Player.h"
//�G�̊Ǘ��n�N���X
class EnemyManager {
private:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	EnemyManager(Player* _player);
	//�X�V
	void Update();
	//�`��
	void Draw(DirectXCommon* dxCommon);
	//ImGui
	void ImGuiDraw();

	
public://getter setter
	//static void SetPlayer(Player* player) { EnemyManager::player = player; }
protected: //�ÓI�����o�ϐ�
	static Player* player;
private:
	unique_ptr<InterEnemy> enemy;
};