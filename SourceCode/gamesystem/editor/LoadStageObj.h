#pragma once
#include<DirectXMath.h>
#include<memory>
#include<fstream>
#include<string>
#include<sstream>
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
	void Collide();
private:
	std::vector<Ghost*> ghosts;
	std::vector<Food*> foods;
};

