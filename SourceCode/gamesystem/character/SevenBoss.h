#pragma once
#include "InterBoss.h"
#include "Shake.h"
#include "Poltergeist.h"
#include "AvatarBoss.h"
#include "BossStunEffect.h"
class SevenBoss :
	public InterBoss {
public:
	SevenBoss();

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
private:
	void CSVLoad();
	
	//�e�{�X�̍s��
	void InterValMove();//�C���^�[�o��
	void Polter();//�|���^�[�K�C�X�g
	void ThrowBound();//�o�E���h�e
	void BirthAvatar();//�U���̃{�X
	void BulletCatch();//�e���z��
	void Stun();//�X�^��
	void BirthExplosion();

	void BirthPolter(const std::string& PolterName);//�|���^�[�K�C�X�g�̐���

	void CatchBul(vector<InterBullet*>bullet);
private:
	//�L�����̏��
	enum CharaState
	{
		STATE_INTER,
		STATE_POLTER,
		STATE_BOUND,
		STATE_AVATAR,
		STATE_CATCH,
		STATE_STUN,
	}_charaState;

	//�֐��|�C���^
	static void(SevenBoss::* stateTable[])();
private:
	static const int POLTER_NUM = 4;
	static const int AVATAR_NUM = 2;
private:
	vector<Poltergeist*> poltergeist;//�|���^�[�K�C�X�g
	vector<InterBoss*> avatarboss;//�U���̃{�X
	unique_ptr<BossStunEffect> bossstuneffect;
	int m_InterVal = {};

	int m_MoveTimer = {};
	int m_AvatarCount = {};

	//�U����
	int m_AttackCount = {};
	
	bool m_Stun = false;
};