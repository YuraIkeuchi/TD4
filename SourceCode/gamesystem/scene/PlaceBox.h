#pragma once
#include "PlaceManager.h"

class PlaceBox:public PlaceManager
{
public:
	void Initialize() override;

	void Update() override;

	void Draw(DirectXCommon* DxCommon) override;

	void ImGui_Draw() override;

	void FileWriting() override;
	ImVec4 p;
	void ArgmentObj(bool& aflag, IKEModel* model) override;;
private:
	std::vector<std::unique_ptr<IKEObject3d>> boxes;

	bool StoneArgment;
	bool OtherArgment;

	std::vector<bool> SelThis;
public:


};

