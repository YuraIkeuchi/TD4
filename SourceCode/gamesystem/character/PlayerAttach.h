#pragma once
#include "ObjCommon.h"
using namespace std;         //  ¼OóÔwè

//eÌîêNX
class PlayerAttach :
	public ObjCommon {
protected:
	// DirectX::ðÈª
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	PlayerAttach();
	//ú»
	bool Initialize()override;
	//XV
	void Update()override;
	//`æ
	void Draw(DirectXCommon* dxCommon)override;
	//ImGuiùè
	void ImGuiDraw();

public:
	//gettersetter
	void SetAfterAngle(const XMFLOAT2& AfterAngle) { m_AfterAngle = AfterAngle; }
	//void SetAfterRot(const float AfterRotY) { m_AfterRotY = AfterRotY; }
public:
	XMFLOAT2 m_AfterAngle = {};//eÌpx
	XMFLOAT2 m_Angle = {};//eÌpx
	float m_AfterRotY = {};
	float m_AddSpeed = {};//Á¬x
	float m_SinAngle = {};
	float m_SinAngle2 = {};
};