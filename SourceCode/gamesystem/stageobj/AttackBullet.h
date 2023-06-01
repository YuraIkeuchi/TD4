#pragma once
#include "InterBullet.h"
//Œ¾—ìƒNƒ‰ƒX
class AttackBullet :
	public InterBullet {
public:
	AttackBullet();

	bool Initialize() override;//‰Šú‰»

	/// <summary>
	/// ImGui•`‰æ
	/// </summary>
	void ImGui_Origin() override;
private:
	//’e‚Ì“®‚­ˆ—
	void Action() override;
};
