#pragma once
#include "InterBoss.h"
class FirstBoss :
	public InterBoss {
public:
	FirstBoss();
	bool Initialize() override;//������

	void Pause() override;//�|�[�Y
private:
	void Action() override;//�s��
};