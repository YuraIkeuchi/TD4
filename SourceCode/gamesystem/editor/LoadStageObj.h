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
	//�S�[�X�g���[�h
	void GhostLoad();
	//�H�ו����[�h
	void FoodLoad();
	//���ׂẴ��[�h
	void AllLoad(Player* player);
	//������
	void Initialize()override;
	//�X�V
	void Update()override;
	//�`��
	void Draw(DirectXCommon* dxCommon)override;
	//ImGui
	void ImGuiDraw();

private:
	void Collide();
private:
	std::vector<Ghost*> ghosts;
	std::vector<Food*> foods;
};

