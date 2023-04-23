#pragma once
#include "InterBoss.h"
class FirstBoss :
	public InterBoss {
public:
	FirstBoss();
	bool Initialize() override;//初期化

	void Pause() override;//ポーズ
private:
	void Action() override;//行動
};