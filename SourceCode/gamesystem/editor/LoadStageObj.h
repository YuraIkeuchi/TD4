#pragma once
#include<DirectXMath.h>
#include<memory>
#include<fstream>
#include<string>
#include<sstream>
#include <iostream>
#include "Ghost.h"
#include "Food.h"
#include "LoadManager.h"
#include "Player.h"
using namespace DirectX;
class LoadStageObj :public LoadManager
{

public:
	//ゴーストロード
	void GhostLoad();
	//食べ物ロード
	void FoodLoad();
	//すべてのロード
	void AllLoad(Player* player);
	//初期化
	void Initialize()override;
	//更新
	void Update()override;
	//描画
	void Draw(DirectXCommon* dxCommon)override;
	//ImGui
	void ImGuiDraw();
private:
	//食料の検索
	void SearchFood();
	//食料とゴーストの当たり判定
	void CollideFood();
	//ゴーストが消える
	void VanishGhost();
	string format(float f, int digits);
private:
	//当たり判定
	void Collide();
private:
	std::vector<Ghost*> ghosts;
	std::vector<Food*> foods;
	//ゴーストを消す処理
	float m_VanishCount = 0.0f;
	bool m_Vanish = false;
};

