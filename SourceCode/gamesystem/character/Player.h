#pragma once
#include <any>
#include"IKEFBXObject3d.h"
#include<memory>
#include "CharactorManager.h"
#include "Input.h"
#include "Bullet.h"
using namespace DirectX;
class Player:public CharactorManager
{
public:
	Player(XMFLOAT3 StartPos = {0.f,0.f,0.f});
	~Player()override;

private:
	//�L�[�̃C���X�^���X�擾�p
	Input* input;
private:
	//�̗�
	static int HP;

	//�A�j���[�V�����Ǘ��p
	bool m_LoopFlag = true;
	int m_AnimationSpeed = 1;
	bool m_StopFlag=false;

	static void (Player::* stateTable[])();
private:
	std::unique_ptr<IKEFBXObject3d>Model;
public:
	//������
	void Initialize()override;
	//�X�V
	void Update()override;
	//�`��
	void Draw(DirectXCommon* dxCommon)override;
	//ImGui
	void ImGuiDraw();


	//�L�����̏��
	enum CharaState
	{
		STATE_IDLE,
		STATE_RUN,
		STATE_ATTACK,
		STATE_SHOT
	}_charaState;

private:
	//�����܂��͑�����
	float velocity;
	//�ړ������w��p
	float angle;
	//�ړ����Z�l
	float m_AddSpeed;
	//���̒l��CSV����ǂݍ��ނƂ��̊i�[�p
	std::any sp;

	//���邫����
	void Walk();
	XMFLOAT3 MoveVECTOR(XMVECTOR v, float angle);

private:
	//�U�����X
	void Attack();
	//�e��ł���
	void Shot();
private:
	void Idle();
private:
	//�e�A�j���[�V����
	enum class AnimeName
	{
		IDLE=7,
		WALK=5,
		ATTACK=0
	}_animeName;

	void AnimationControl(AnimeName name, const bool& loop, int speed);

private://�e�N���X
	vector<Bullet*> bullets;
};

