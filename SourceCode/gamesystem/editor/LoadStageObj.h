#pragma once
#include<DirectXMath.h>
#include<memory>
#include<string>
#include "Ghost.h"
#include "Food.h"
#include "Heart.h"
#include "LoadManager.h"
#include "EnemyManager.h"
#include "LightGroup.h"
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
	void TutorialUpdate()override;
	//�X�V
	void FirstUpdate()override;
	//�X�V
	void SecondUpdate()override;
	//�X�V
	void ThirdUpdate();

	//�`��
	void Draw(DirectXCommon* dxCommon)override;
	//ImGui
	void ImGuiDraw();
	//���C�g�̃Z�b�g
	void LightSet(LightGroup* light);
	//���C�g�̍X�V
	void LightUpdate();
private:
	//�H���̌���
	void SearchFood();
	//�H���ƃS�[�X�g�̓����蔻��
	void CollideFood();
	//�S�[�X�g��������
	void VanishGhost();
	//���ʂ̍X�V
	void CommonUpdate();
	//�n�[�g�̐���
	void BirthHeart();
	//���C�g�̃T�C�h����
	void LightReturn();
	//�ߏ�̃S�[�X�g�����b�N���܂�
	void LockVerseGhost();
	//�S�[�X�g���폜���܂��B
	void NonVerseGhost();
	//�G�𐶐����܂��B
	void SpawnThirdEnemy();

public:
	static void SetEnemyManager(EnemyManager* m_EnemyManager) { LoadStageObj::m_EnemyManager = m_EnemyManager; }
protected:
	static EnemyManager* m_EnemyManager;
private:
	//�����蔻��
	void Collide();
private:
	LightGroup* lightgroup = nullptr;
	std::vector<Ghost*> ghosts;
	static const int kStopGhostMax = 5;
	array<Ghost*, kStopGhostMax> stopGhosts;
	std::vector<Food*> foods;
	std::vector<Heart*> hearts;
	//�S�[�X�g����������
	float m_VanishCount = 0.0f;
	bool m_Vanish = false;

	float m_Division = 0.0f;

	int nowStopGhorst = 0;

	//�ۉe
	float circleShadowDir[3] = { 0,-1,0 };
	float circleShadowPos[3] = { 1,2,0 };
	float circleShadowAtten[3] = { 0.5f,0.6f,0.0f };
	float circleShadowFactorAngle[2] = { 0.0f, 0.8f };

	string m_SceneName;

	float m_Alpha = {};
};

