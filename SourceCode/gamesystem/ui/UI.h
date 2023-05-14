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
		XMFLOAT2 Position;
		XMFLOAT2 Scale;
		float Rotation;
		XMFLOAT4 Color;
		//�K���ȃt���O
		bool OtherF;
	};
	enum  SpriteName {
		UnderStatusHp = 0,
		StatusHp,
		UnderStatusGauge,
		StatusGauge,
		UiMax
	};

	//��
	std::array<SpriteData, UiMax> sprites={};
	std::vector<SpriteData>TexList;
public:
	//������
	void Initialize();
	//�X�V
	void Update();
	//�`��
	void Draw();
	void ArrowRota();
};

