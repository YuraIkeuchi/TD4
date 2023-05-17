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
	//���t�H���g
	struct SpriteData {
		//�摜
		std::unique_ptr<IKESprite> Tex;
		//��b�p�����[�^
		XMFLOAT2 Position = { 0,0 };
		XMFLOAT2 Size = { 100,100 };
		float Rotation = 0.f;
		XMFLOAT4 Color = { 1,1,1,1 };
		//�\���ȃt���O
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

	//�X�v���C�g
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
	//������
	void Initialize();
	//�X�V
	void Update();
	//�`��
	void Draw();
private:
	//
	SpriteData CreateUi(UINT texNumber, XMFLOAT2 pos, XMFLOAT2 size, XMFLOAT4 color);

};

