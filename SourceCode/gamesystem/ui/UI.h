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
		XMFLOAT2 Position;
		XMFLOAT2 Scale;
		float Rotation;
		XMFLOAT4 Color;
		//適当なフラグ
		bool OtherF;
	};
	enum  SpriteName {
		UnderStatusHp = 0,
		StatusHp,
		UnderStatusGauge,
		StatusGauge,
		UiMax
	};

	//仮
	std::array<SpriteData, UiMax> sprites={};
	std::vector<SpriteData>TexList;
public:
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();
	void ArrowRota();
};

