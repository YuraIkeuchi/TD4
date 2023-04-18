#pragma once
#include"DirectXCommon.h"
#include <DirectXMath.h>
using namespace DirectX;
class CharactorManager
{
public:
	CharactorManager(XMFLOAT3 StartPos = { 0.f,0.f,0.f });
	virtual ~CharactorManager(){};

public:
	//gettersetter
	const XMFLOAT3& GetPosition() { return Position; }
	const XMFLOAT3& GetRotation() { return Rotation; }
	const XMFLOAT3& GetScale() { return Scale; }
	void SetPosition(const XMFLOAT3& position) { Position = position; }
	void SetRotation(const XMFLOAT3& rotation) { Rotation = rotation; }
	void SetScale(const XMFLOAT3& scale) { Scale = scale; }

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

