#pragma once
#include"IKEObject3d.h"
#include<DirectXMath.h>
#include<memory>

#include<fstream>
#include<string>
#include<sstream>
#include "DirectXCommon.h"
using namespace DirectX;
class PlaceManager
{
protected:
	std::unique_ptr<IKEObject3d>Obj;
	std::unique_ptr<IKEObject3d>SampleObj;
	std::vector<XMFLOAT3> Pos;
	std::vector < XMFLOAT3> Scl;
	std::vector < XMFLOAT3> Rot;

	XMFLOAT3 S_Pos;
	XMFLOAT3 S_Scl={2.f,2.f,2.f};
	XMFLOAT3 S_Rot;

public:
	virtual void Initialize()=0;
	//
	virtual void Update() = 0;
	//
	virtual void Draw(DirectXCommon* DxCommon) = 0;

	virtual void FileWriting() = 0;

	virtual void ImGui_Draw() = 0;

	void SetP(XMFLOAT3 pos) { S_Pos = pos; }
protected:
	int Quantity;
	std::vector<int> Number;
	std::ifstream file;
	std::stringstream popcom;
	//配置フラグ
	bool ArgmentFlag;
	//削除フラグ
	bool DeleteFlag;

};

