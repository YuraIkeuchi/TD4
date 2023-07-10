#pragma once
#include "InterBoss.h"
#include "Shake.h"
#include "Poltergeist.h"
#include "FireBoll.h"
#include "DamageBlock.h"
class AvatarBoss :
	public InterBoss {
public:
	AvatarBoss();

	bool Initialize() override;//������

	void SkipInitialize() override;//�X�L�b�v���̏�����

	void Pause() override;//�|�[�Y

	void Action() override;//�s��

	void AppearAction() override;//�{�X�o��̌ŗL�̏���

	void DeadAction() override;//�{�X���j�̌ŗL�̏���

	void DeadAction_Throw() override;//�{�X���j�̌ŗL�̏��� �X���[

	void ImGui_Origin() override;//�{�X���ꂼ���ImGui

	void EffecttexDraw(DirectXCommon* dxCommon) override;

	void Draw(DirectXCommon* dxCommon) override;//�`��

	void InitAwake() override;//�{�X�̊o��
private:
	void CSVLoad();

	//�e�{�X�̍s��
	void InterValMove();//�C���^�[�o��
	void Polter();//�|���^�[�K�C�X�g
	void ThrowBound();//�o�E���h�e
	void FireAttack();//�΂̋ʍU��
	void BirthFire();//������
	void BlockAttack();//�_���[�W�u���b�N�̐���
	void BirthBlock();
	void BirthPolter(const std::string& PolterName);//�|���^�[�K�C�X�g�̐���

	//�A�o�^�[�̃^�C�v
	void AvatarNormal();
	void AvatarAround();
	void AvatarRight();
	void AvatarLeft();
public:
private:
	//�L�����̏��
	enum CharaState
	{
		STATE_INTER,
		STATE_POLTER,
		STATE_BOUND,
		STATE_FIRE,
		STATE_BLOCK
	}_charaState;

	//�֐��|�C���^
	static void(AvatarBoss::* stateTable[])();
	static void(AvatarBoss::* avatarTable[])();
private:
	static const int POLTER_NUM = 2;
	static const int FIRE_NUM = 4;
	static const int BLOCK_NUM = 3;
private:
	vector<FireBoll*> fireboll;//�΂̋�
	vector<Poltergeist*> poltergeist;//�|���^�[�K�C�X�g
	vector<DamageBlock*> damageblock;//�_���[�W�u���b�N
	int m_InterVal = {};

	int m_MoveTimer = {};

	//�U���̗���
	int m_AttackRand = {};

	//�C�[�W���O��̈ʒu
	XMFLOAT3 m_AfterPos = {};

	bool m_Return = false;

	enum ReturnState {
		RETURN_SET,
		RETURN_PLAY,
		RETURN_END,
	}_ReturnState;
	//���������鎞��
	float m_VanishFrame = {};
	//���x
	float m_AfterAlpha = {};

	float m_SaveSpeed = {};

	enum AvatarType {
		AVATAR_ONE,
		AVATAR_SECOND,
		AVATAR_THIRD,
		AVATAR_FOURTH
	};
};
