#pragma once
#include <any>

#include"DirectXCommon.h"
#include <DirectXMath.h>
#include"IKEFBXObject3d.h"
#include<memory>
#include "Input.h"
using namespace DirectX;
class Player
{
public:
	Player(XMFLOAT3 StartPos = {0.f,0.f,0.f});
	~Player();

private:
	//�L�[�̃C���X�^���X�擾�p
	Input* input;

private:
	//��b�p�����[�^
	XMFLOAT3 Position;
	XMFLOAT3 Rotation;
	XMFLOAT3 Scale;

	//�̗�
	static int HP;

	//�A�j���[�V�����Ǘ��p
	bool m_LoopFlag = true;
	int m_AnimationSpeed = 1;
	bool m_StopFlag=false;

private:
	std::unique_ptr<IKEFBXObject3d>Model;
public:
	void Init();
	void Upda();
	void Draw(DirectXCommon* dxCommon);

	//�L�����̏��
	enum CharaState
	{
		STATE_IDLE,
		STATE_RUN,
		STATE_ATTACK
	}_charaState;

	static void (Player::* stateTable[])();

private:
	//�����܂��͑�����
	float velocity;
	//�ړ������w��p
	float angle;
	//���邫����
	void Walk();
	XMFLOAT3 MoveVECTOR(XMVECTOR v, float angle);
	//�ړ����Z�l
	float m_AddSpeed;
	//���̒l��CSV����ǂݍ��ނƂ��̊i�[�p
	std::any sp;
private:
	//�U�����X
	void Attack();
private:
	void Idle();
private:
	//�e�A�j���[�V����
	enum class AnimeName
	{
		IDLE=7,
		ATTACK=0
	}_animeName;

	void AnimationControl(AnimeName name, const bool& loop, int speed);
};

