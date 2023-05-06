#pragma once
#include <any>
#include"IKEFBXObject3d.h"
#include<memory>
#include "ObjCommon.h"

#include "GhostBullet.h"
#include "AttackBullet.h"
using namespace DirectX;
class Player:public ObjCommon
{
public:
	Player(XMFLOAT3 StartPos = {0.f,0.f,0.f});
	~Player()override;

private:
	//�̗�
	static int HP;

	//�A�j���[�V�����Ǘ��p
	bool m_LoopFlag = true;
	int m_AnimationSpeed = 1;
	bool m_StopFlag=false;

	static void (Player::* stateTable[])();
public:
	//������
	bool Initialize()override;
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
		STATE_GHOST,
		STATE_ATTACKSHOT,
		STATE_SUPERSHOT,
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
	//�e��ł���(�S�[�X�g��߂܂���)
	void GhostShot();
	//�e��ł���(�U��)
	void AttackShot();
	//�e��ł���(���ߍU��)
	void SuperShot();
private:
	void Idle();
	//�C���^�[�o���Ǘ�
	void InterVal();
	//�C���^�[�o���̃Z�b�g
	void SetInterVal();
	//�e��I��
	void SelectBullet();
	//�e�̍X�V
	void BulletUpdate();
private:
	//�e�A�j���[�V����
	enum class AnimeName
	{
		IDLE=7,
		WALK=5,
	}_animeName;

	void AnimationControl(AnimeName name, const bool& loop, int speed);
public:
	//�����蔻��n
	bool BulletCollide(const XMFLOAT3& pos,const bool Catch);//�e�Ƃ̓����蔻��
	bool PlayerCollide(const XMFLOAT3& pos);//�v���C���[�Ƃ̓����蔻��
public:
	const int& GetBulletType() { return m_BulletType; }
private://�e�N���X
	vector<InterBullet*> ghostbullets;//����
	vector<InterBullet*> attackbullets;//�U��
	//�e�֌W�̕ϐ�
	int m_BulletType = {};//�e�̎��
	int m_InterVal = {};//�e�̔��˂̃C���^�[�o��
	int m_RigidityTime = {};//�d������
	int m_TargetInterVal = {};//�C���^�[�o���̖ڕW����
	int m_TargetRigidityTime = {};//�d�����Ԃ̖ڕW����

	XMFLOAT3 m_FollowPos = {};

	int m_ShotTimer = {};//�V���b�g�̃`���[�W����

public:
	vector<InterBullet*>GetBulllet_ghost() { return ghostbullets; }
	vector<InterBullet*>GetBulllet_attack() { return attackbullets; }

};

