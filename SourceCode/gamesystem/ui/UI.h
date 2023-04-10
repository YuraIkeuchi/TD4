#pragma once
#include"IKESprite.h"
#include<memory>
#include<DirectXMath.h>
using namespace DirectX;
class UI
{
public:
	UI();
	~UI();

private:
	//���t�H���g
	struct Debug
	{
		//�摜
		IKESprite* Tex;
		//��b�p�����[�^
		XMFLOAT2 Position;
		XMFLOAT2 Scale;
		XMFLOAT2 Rotation;
		//�K���ȃt���O
		bool OtherF;
	};
	//��
	static std::unique_ptr<Debug>TexA;

public:
	//������
	void Initialize();
	//�X�V
	void Update();
	//�`��
	void Draw();
};

