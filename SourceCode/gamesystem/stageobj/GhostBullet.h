#pragma once
#include "InterBullet.h"

enum Bullettype {
	BULLET_FORROW,//�Ǐ]
	BULLET_SEARCH,//�T��
};
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

private:
	void VanishBullet();

private:
	float m_Frame = 0.0f;
	float m_AddFrame = {};
};
