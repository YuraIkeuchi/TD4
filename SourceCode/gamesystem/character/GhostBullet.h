#pragma once
#include "InterBullet.h"
//����N���X
class GhostBullet :
	public InterBullet {
public:
	GhostBullet();

	bool Initialize() override;//������

	/// <summary>
	/// ImGui�`��
	/// </summary>
	void ImGui_Origin() override;
private:
	//�e�̓�������
	void Action() override;

};
