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
	//���t�H���g
	struct Debug
	{
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
	//��
	Debug TexA;

	std::vector<Debug>TexList;
public:
	//������
	void Initialize();
	//�X�V
	void Update();
	//�`��
	void Draw();
};

