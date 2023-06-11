#pragma once
#include "ObjCommon.h"
#include "ViewBullet.h"
#include "GhostBullet.h"
#include "AttackBullet.h"
#include "PlayerAttach.h"
#include "CollisionPrimitive.h"
#include "BreakEffect.h"
#include <any>
using namespace DirectX;
class Player:public ObjCommon
{
public:
	static Player* GetInstance();

private:

	static void (Player::* stateTable[])();
public:
	void InitState(const XMFLOAT3& pos);
	//������
	bool Initialize()override;
	//�X�V
	void Update()override;
	//�`��
	void Draw(DirectXCommon* dxCommon)override;
	//ImGui
	void ImGuiDraw();
	//�{�X�o��V�[���̓���
	void AppearUpdate();
	//�{�X���j�V�[���̓���
	void DeathUpdate();
	//�L�����̏��
	enum CharaState
	{
		STATE_IDLE,
		STATE_RUN,
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
	//�e�̐���
	void BirthShot(const std::string& bulletName, bool Super);
private:
	void Idle();
	//�C���^�[�o���Ǘ�
	void InterVal();
	//�e�̃��Z�b�g
	void ResetBullet();
	//�e�̊Ǘ�
	void Bullet_Management();
	//�e�̍X�V
	void BulletUpdate(std::vector<InterBullet*> bullets);
	//�e�̕`��
	void BulletDraw(std::vector<InterBullet*> bullets, DirectXCommon* dxCommon);
	//�e�̏���
	void SutoponUpdate();
	//�_���[�W�p�[�e�B�N��
	void BirthParticle();
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
	bool PlayerCollide(const XMFLOAT3& pos);//�v���C���[�Ƃ̓����蔻��
	//�v���C���[�̓��������u��
	void PlayerHit(const XMFLOAT3& pos);
	//�e����鏈��
	void ReBound();
public:
	//gettersetter
	const int& GetBulletType() { return m_BulletType; }
	const int& GetDamageInterVal() { return m_DamageInterVal; }
	const bool& GetIsShotNow() { return isShotNow; }


	void SetHP(float hp) { m_HP = hp; };
	float GetHP() { return m_HP; }
	float GetMaxHP() { return m_MaxHP; }

	void SetCanShot(const bool m_canShot) {this->m_canShot = m_canShot;}


private://�e�N���X
	vector<InterBullet*> ghostbullets;//����
	vector<InterBullet*> attackbullets;//�U��
	unique_ptr<InterBullet> viewbullet;//�����̒e
	unique_ptr<PlayerAttach> playerattach;//�v���C���[�̑���
	vector<InterEffect*> effects;//�G�t�F�N�g
	//�e�֌W�̕ϐ�
	int m_BulletType = {};//�e�̎��
	int m_InterVal = {};//�e�̔��˂̃C���^�[�o��
	int m_RigidityTime = {};//�d������
	int m_TargetInterVal = {};//�C���^�[�o���̖ڕW����
	int m_TargetRigidityTime = {};//�d�����Ԃ̖ڕW����
	//�̗�
	float m_HP = 0.0f;
	float m_MaxHP = 0.0f;

	XMFLOAT3 m_FollowPos = {};

	int m_ShotTimer = {};//�V���b�g�̃`���[�W����
	bool m_canShot = true;
	//�_���[�W
	bool m_Damage = false;
	int m_DamageInterVal = {};

	XMFLOAT2 m_BoundPower = {};
	
	//�e�̐�
	int m_BulletNum = 0;

public:
	vector<InterBullet*>GetBulllet_ghost() { return ghostbullets; }
	vector<InterBullet*>GetBulllet_attack() { return attackbullets; }

private:
	bool isStop;
	bool isShotNow=false;
	XMFLOAT3 OldPos;
public:
	void MoveStop(bool f) { isStop = f; }
	void isOldPos();

	void RecvDamage(float Damage);

	//�e�̑S�폜
	void BulletDelete();
};

