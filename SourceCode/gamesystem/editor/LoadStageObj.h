#pragma once
#include<DirectXMath.h>
#include<memory>
#include<fstream>
#include<string>
#include<sstream>
#include <iostream>
#include "Ghost.h"
#include "Food.h"
#include "LoadManager.h"
#include "EnemyManager.h"
using namespace DirectX;
class LoadStageObj :public LoadManager
{

public:
	//�S�[�X�g���[�h
	void GhostLoad();
	//�H�ו����[�h
	void FoodLoad(const std::string& sceneName);
	//���ׂẴ��[�h
	void AllLoad(const std::string& sceneName);
	//������
	void Initialize()override;
	//�X�V
	void FirstUpdate()override;
	//�X�V
	void SecondUpdate()override;
	//�`��
	void Draw(DirectXCommon* dxCommon)override;
	//ImGui
	void ImGuiDraw();
private:
	//�H���̌���
	void SearchFood();
	//�H���ƃS�[�X�g�̓����蔻��
	void CollideFood();
	//�S�[�X�g��������
	void VanishGhost();
	//���ʂ̍X�V
	void CommonUpdate();
public:
	static void SetEnemyManager(EnemyManager* enemy) { LoadStageObj::enemy = enemy; }
protected:
	static EnemyManager* enemy;

private:
	//�����蔻��
	void Collide();
private:
	std::vector<Ghost*> ghosts;
	std::vector<Food*> foods;
	//�S�[�X�g����������
	float m_VanishCount = 0.0f;
	bool m_Vanish = false;

	float m_Division = 0.0f;
};

