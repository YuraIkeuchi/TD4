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
	const XMFLOAT3& GetPosition() { return m_Position; }
	const XMFLOAT3& GetRotation() { return m_Rotation; }
	const XMFLOAT3& GetScale() { return m_Scale; }
	void SetPosition(const XMFLOAT3& position) { m_Position = position; }
	void SetRotation(const XMFLOAT3& rotation) { m_Rotation = rotation; }
	void SetScale(const XMFLOAT3& scale) { m_Scale = scale; }

protected:
	//基礎パラメータ
	XMFLOAT3 m_Position;
	XMFLOAT3 m_Rotation;
	XMFLOAT3 m_Scale;

public:
	//初期化
	virtual void Initialize()=0;
	//更新
	virtual void Update()=0;
	//描画
	virtual void Draw(DirectXCommon* dxCommon)=0;
};

