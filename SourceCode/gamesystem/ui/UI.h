#pragma once
#include"IKESprite.h"
#include<memory>
#include<DirectXMath.h>
using namespace DirectX;
class UI
{
public:
	UI(){};
	~UI();

private:
	//仮フォント
	struct Debug
	{
		//画像
		std::unique_ptr<IKESprite> Tex;
		//基礎パラメータ
		XMFLOAT2 Position;
		XMFLOAT2 Scale;
		float Rotation;
		XMFLOAT4 Color;
		//適当なフラグ
		bool OtherF;
	};
	//仮
	Debug TexA;

	std::vector<Debug>TexList;
public:
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();
};

