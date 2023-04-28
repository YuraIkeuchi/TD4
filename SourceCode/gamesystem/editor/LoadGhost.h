#pragma once
#include<DirectXMath.h>
#include<memory>

#include<fstream>
#include<string>
#include<sstream>
#include "Ghost.h"
#include "LoadManager.h"
#include "Player.h"
using namespace DirectX;
class LoadGhost :public LoadManager
{
private:

	std::vector<std::string> Name;
	std::string line;
	std::stringstream popcom;
	std::ifstream file;

public:
	void Load(Player* player);

	void Initialize()override;

	void Update()override;

	void Draw(DirectXCommon* dxCommon)override;

	void ImGuiDraw();

private:
	void Collide();
private:
	std::vector<Ghost*> ghosts;
	bool a = false;
};

