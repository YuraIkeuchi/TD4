#pragma once
#include "Note.h"
#include "InterBoss.h"
#include "Shake.h"
#include "Player.h"

class FourthBoss :
	public InterBoss {
public:
	FourthBoss();

	bool Initialize() override;//������

	void SkipInitialize() override;//�X�L�b�v���̏�����

	void Pause() override;//�|�[�Y

	void Action() override;//�s��

	void AppearAction() {};//�{�X�o��̌ŗL�̏���

	void DeadAction() {};//�{�X���j�̌ŗL�̏���

	void DeadAction_Throw() {};//�{�X���j�̌ŗL�̏��� �X���[

	void ImGui_Origin() {};//�{�X���ꂼ���ImGui

	void EffecttexDraw(DirectXCommon* dxCommon) override;

	void Draw(DirectXCommon* dxCommon) override;//�`��
private:
	enum class commandState : int {
		WaitCommand = 0,
		MoveCommand,
		ControlCommand,
	};

	//�֐��|�C���^
	static void(FourthBoss::* stateTable[])();
	//�����o�֐�
	void WaitUpdate();
	void MoveUpdate();
	void ControlUpdate();

	unique_ptr<Note> note;
};
