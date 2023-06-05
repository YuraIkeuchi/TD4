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
public:
	static void SetEnemyManager(EnemyManager* boss) { LoadStageObj::boss = boss; }
protected:
	static EnemyManager* boss;

private:
	//�����蔻��
	void Collide();
private:
	LightGroup* lightgroup = nullptr;
	std::vector<Ghost*> ghosts;
	std::vector<Food*> foods;
	std::vector<Heart*> hearts;
	//�S�[�X�g����������
	float m_VanishCount = 0.0f;
	bool m_Vanish = false;

	float m_Division = 0.0f;

	//�ۉe
	float circleShadowDir[3] = { 0,-1,0 };
	float circleShadowPos[3] = { 1,2,0 };
	float circleShadowAtten[3] = { 0.5f,0.6f,0.0f };
	float circleShadowFactorAngle[2] = { 0.0f, 0.8f };

	string m_SceneName;

	int m_HeartCount = 2;
};

