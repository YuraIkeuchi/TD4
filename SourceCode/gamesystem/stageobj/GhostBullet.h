#pragma once
#include "InterBullet.h"

//言霊クラス
class GhostBullet :
	public InterBullet {
public:
	GhostBullet();

	bool Initialize() override;//初期化

	/// <summary>
	/// ImGui描画
	/// </summary>
	void ImGui_Origin() override;

	void Draw_Origin() override;
private:
	//弾の動く処理
	void Action() override;

private:
	void VanishBullet();

private:
	float m_Frame = 0.0f;
	float m_AddFrame = {};
	float m_StartScale = {};
	float m_EndScale = {};
};
