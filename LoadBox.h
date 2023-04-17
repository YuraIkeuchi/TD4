#pragma once
#include<DirectXMath.h>
#include"IKEObject3d.h"
#include<memory>

#include<fstream>
#include<string>
#include<sstream>

using namespace DirectX;
class LoadBox
{
private:
	//XMFLOAT3 Pos;
	XMFLOAT3 Rot;

	std::vector<std::unique_ptr< IKEObject3d>>boxes;
	std::string line;
	std::stringstream popcom;
	std::ifstream file;

public:
	void Load();

	void Initialize();

	void Update();

	void Draw();

	IKEObject3d* GetBox(int i) { return boxes[i].get(); }
};

