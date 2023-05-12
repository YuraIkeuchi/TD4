#pragma once
#include "NormalEnemy.h"
#include"FirstBoss.h"
#include "SecondBoss.h"
#include "Player.h"
#include<array>
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
	EnemyManager(const std::string& sceneName);
	//�X�V
	void Update();
	//�`��
	void Draw(DirectXCommon* dxCommon);
	//ImGui
	void ImGuiDraw();


public://getter setter
	//static void SetPlayer(Player* player) { EnemyManager::player = player; }
protected: //�ÓI�����o�ϐ�
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
	//3�̃I�u�W�F�N�g�ʒu�ݒ�
	inline void SetEpos_A(XMFLOAT3* pos) { for (auto i = 0; i < 3; i++)EPos[i] = pos[i]; }

	//�G�̎��S����
	bool BossDestroy();
};