#pragma once
#include"IKESprite.h"
#include<memory>
#include<DirectXMath.h>
#include <array>
using namespace DirectX;
class UI {
public:
	UI() {};
	~UI();

private:
	//仮フォント
	struct SpriteData {
		//画像
		std::unique_ptr<IKESprite> Tex;
		//基礎パラメータ
		XMFLOAT2 Position = { 0,0 };
		XMFLOAT2 Size = { 100,100 };
		float Rotation = 0.f;
		XMFLOAT4 Color = { 1,1,1,1 };
		//表示なフラグ
		bool IsVisible = true;
	};
	enum  SpriteName {
		HeartOne=0,
		HeartTwo,
		HeartThree,
		HeartFour,
		HeartFive,
		UnderStatusGauge,
		StatusGauge,
		TopStatusGauge,
		UiMax
	};

	//スプライト
	std::array<SpriteData, UiMax> sprites = {};

	std::array<XMFLOAT2, UnderStatusGauge> pos = {
	XMFLOAT2(0.f,620.f),
	XMFLOAT2(90.f,620.f),
	XMFLOAT2(180.f,620.f),
	XMFLOAT2(270.f,620.f),
	XMFLOAT2(360.f,620.f)
	};

	std::vector<SpriteData>TexList;
public:
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();
private:
	//
	SpriteData CreateUi(UINT texNumber, XMFLOAT2 pos, XMFLOAT2 size, XMFLOAT4 color);

};

