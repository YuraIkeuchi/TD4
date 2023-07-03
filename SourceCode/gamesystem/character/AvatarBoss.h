#pragma once
#include "InterBoss.h"
#include "Shake.h"
#include "Poltergeist.h"

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
private:
	void CSVLoad();

	//�e�{�X�̍s��
	void InterValMove();//�C���^�[�o��
	void Polter();//�|���^�[�K�C�X�g
	void ThrowBound();//�o�E���h�e

	void BirthPolter(const std::string& PolterName);//�|���^�[�K�C�X�g�̐���
private:
	//�L�����̏��
	enum CharaState
	{
		STATE_INTER,
		STATE_POLTER,
		STATE_BOUND,
	}_charaState;

	//�֐��|�C���^
	static void(AvatarBoss::* stateTable[])();
private:
	static const int POLTER_NUM = 2;
private:
	vector<Poltergeist*> poltergeist;//�|���^�[�K�C�X�g

	int m_InterVal = {};

	int m_MoveTimer = {};

	//�U���̗���
	int m_AttackRand = {};
};
