#pragma once
#include "IKETexture.h"
#include "Player.h"
class Predict {
public:
	void Initialize(const XMFLOAT3& pos);
	void Update();
	void Draw(DirectXCommon* dxCommon);
	void ImGuiDraw();
private:
	
public:
	bool GetAlive() { return m_Alive; }
private:
	IKETexture* tex = nullptr;
	XMFLOAT3 m_Position = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_Rotation = { 0.0f,0.0f,0.0f };
	XMFLOAT4 m_Color = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT3 m_Scale = { 1.0f,1.0f,1.0f };
	bool m_Alive = true;
	float m_Frame = 0.0f;
};

