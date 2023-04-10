#pragma once
#include"DirectXCommon.h"
#include <DirectXMath.h>
using namespace DirectX;
class CharactorManager
{
public:
	CharactorManager(XMFLOAT3 StartPos = { 0.f,0.f,0.f });
	virtual ~CharactorManager(){};

protected:
	//��b�p�����[�^
	XMFLOAT3 Position;
	XMFLOAT3 Rotation;
	XMFLOAT3 Scale;

public:
	//������
	virtual void Initialize()=0;
	//�X�V
	virtual void Update()=0;
	//�`��
	virtual void Draw(DirectXCommon* dxCommon)=0;
};

