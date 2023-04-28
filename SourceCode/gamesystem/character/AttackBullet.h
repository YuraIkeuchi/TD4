#pragma once
#include "InterBullet.h"
//言霊クラス
class AttackBullet :
	public InterBullet {
public:
	AttackBullet();

	bool Initialize() override;//初期化

	/// <summary>
	/// ImGui描画
	/// </summary>
	void ImGui_Origin() override;
private:
	//弾の動く処理
	void Action() override;
};
