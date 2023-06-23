#pragma once
#include <DirectXMath.h>
#include <memory>
#include <string>
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
	void TutorialUpdate();
	//�X�V
	void FirstUpdate();
	//�X�V
	void SecondUpdate();
	//�X�V
	void ThirdUpdate();
	//�X�V
	void FourthUpdate();

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
	//�T�[�h�{�X�Ɋւ���֐����܂Ƃ߂Ă��܂�
	void ThirdBossAction();
	//�S�[�X�g���G�l�~�[�ɕϊ����܂�
	void ChangeGhost2Enemy();
	//�S�[�X�g���G�l�~�[�ɕϊ����܂�
	void ChangeGhost2Hyper();
	//�ߏ�̃S�[�X�g�����b�N���܂�
	void LockVerseGhost();
	//�����̃S�[�X�g�����b�N���܂�
	void LockAllGhost();

	//�S�[�X�g���폜���܂��B
	void NonVerseGhost();
	//�Q�Ƃ���Ă���S�[�X�g�����݂��邩�`�F�b�N���܂��B
	bool CheckReferGhost();
	//��ɓ���Ă���S�[�X�g���O�̂قǏ����܂�
	void SubHunger();
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
	//
	static const int kStopGhostMax = 10;
	array<Ghost*, kStopGhostMax> stopGhosts;
	std::vector<Food*> foods;
	std::vector<Heart*> hearts;
	//�S�[�X�g����������
	float m_VanishCount = 0.0f;
	bool m_Vanish = false;

	float m_Division = 0.0f;

	int kStopGhorstMax = 3;

	//�ۉe
	float circleShadowDir[3] = { 0,-1,0 };
	float circleShadowPos[3] = { 1,2,0 };
	float circleShadowAtten[3] = { 0.5f,0.6f,0.0f };
	float circleShadowFactorAngle[2] = { 0.0f, 0.8f };

	string m_SceneName;

	float m_Alpha = {};

	//�Q��Q�[�W�����炷
	bool m_SubHunger = false;
	float m_Frame = 0.0f;

	float m_LimitHunger = {};
};

