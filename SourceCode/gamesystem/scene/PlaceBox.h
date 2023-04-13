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

private:
	std::vector<std::unique_ptr<IKEObject3d>> boxes;
};

