#pragma once
#include "InterBullet.h"
//����N���X
class AttackBullet :
	public InterBullet {
public:
	AttackBullet();

	bool Initialize() override;//������

	/// <summary>
	/// ImGui�`��
	/// </summary>
	void ImGui_Origin() override;
private:
	//�e�̓�������
	void Action() override;
};
