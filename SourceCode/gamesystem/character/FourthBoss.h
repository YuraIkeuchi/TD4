#pragma once
#include "InterBoss.h"
#include "Shake.h"
#include "Note.h"

class FourthBoss :
	public InterBoss {
public:
	FourthBoss();

	bool Initialize() override;//������

	void SkipInitialize() override;//�X�L�b�v���̏�����

	void Pause() override;//�|�[�Y

	void Action() override;//�s��

	void AppearAction() override;//�{�X�o��̌ŗL�̏���

	void DeadAction() {};//�{�X���j�̌ŗL�̏���

	void DeadAction_Throw() {};//�{�X���j�̌ŗL�̏��� �X���[

	void ImGui_Origin() override;//�{�X���ꂼ���ImGui

	void EffecttexDraw(DirectXCommon* dxCommon) override;

	void Draw(DirectXCommon* dxCommon) override;//�`��
private:
	//�����̑I��
	void Choice();
	//�Ǐ]�̉���
	void Follow();
	//�g�U�̒e
	void Diffusion();
	//�����̏��
	void Confusion();
	//CSV�ǂݍ��݌n
	void CSVLoad();
private:
	//�e�N���X
	unique_ptr<Note> note;
	//�L�����̏��
	enum CharaState
	{
		STATE_CHOICE,
		STATE_FOLLOW,
		STATE_DIFF,
		STATE_CONFU,
	}_charaState;

	//��~����
	int m_StopTimer = 0;
	//�ǂ̍s���ɂ��邩
	int m_MoveState = {};

	//�֐��|�C���^
	static void(FourthBoss::* stateTable[])();
	

	//CSV�n
	int m_ChoiceInterval = {};
};
