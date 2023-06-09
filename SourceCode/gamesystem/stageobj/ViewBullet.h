#pragma once
#include "InterBullet.h"
//チャージ中の可視化できる弾のクラス
class ViewBullet :
	public InterBullet {
public:
	ViewBullet();

	bool Initialize() override;//初期化

	/// <summary>
	/// ImGui描画
	/// </summary>
	void ImGui_Origin() override;

	void Draw_Origin() override;
private:
	//弾の動く処理
	void Action() override;

public:
	//描画処理これだけ分ける
	void View_Draw();
public:
	//gettersetter
	
private:

};
