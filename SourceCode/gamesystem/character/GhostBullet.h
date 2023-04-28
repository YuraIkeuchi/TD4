#pragma once
#include "InterBullet.h"
//Œ¾—ìƒNƒ‰ƒX
class GhostBullet :
	public InterBullet {
public:
	GhostBullet();

	bool Initialize() override;//‰Šú‰»

	/// <summary>
	/// ImGui•`‰æ
	/// </summary>
	void ImGui_Origin() override;
private:
	//’e‚Ì“®‚­ˆ—
	void Action() override;

};
